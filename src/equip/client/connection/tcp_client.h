#ifndef _CHAT_CLIENT_H_
#define _CHAT_CLIENT_H_

#include <memory>
#include <deque>
#include <iostream>
#include <boost/asio.hpp>
#include "message/message.h"
#include <QDebug>
#include <QQueue>
#include <boost/bind.hpp>
#include "message/message_body_factory.h"
#include "message_queue.h"
#include "logging.h"
#include "qconnectstate.h"

using namespace std;
using boost::asio::ip::tcp;
using namespace boost;

typedef CSyncCondQueue<CMessage*> session_message_queue;
//typedef QQueue<CMessage *> session_message_queue;

class CTcpClient : public std::enable_shared_from_this<CTcpClient>
{
public:
    CTcpClient(boost::asio::io_service& io_service,
        tcp::resolver::iterator endpoint_iterator)
        : io_service_(io_service),
        socket_(io_service)
    {
        do_connect(endpoint_iterator);
        pMessageHeaderBuf = NULL;
        pMessageBodyBuf = NULL;
    }

    void write(CMessage *  msg)
    {
        io_service_.post(
            [this, msg]()
        {
            qDebug() << "write " << msg->ToString();
            CLogging::GetInstance()->Log(LOG_INFO,"client write:", msg->ToString());
            bool write_in_progress = !write_msgs_.empty();
            write_msgs_.push_back(msg);
            if (!write_in_progress)
            {
                do_write_frame_header();
            }
        });
    }

    void read()
    {
        do_read_frame_header();
    }
    void close()
    {
        io_service_.post([this]() { socket_.close(); });
    }

private:
    void do_connect(tcp::resolver::iterator endpoint_iterator)
    {
        boost::asio::async_connect(socket_, endpoint_iterator,
            [this](boost::system::error_code ec, tcp::resolver::iterator)
        {
            if (!ec)
            {
                CMessageQueue::getInstance()->setConnectState( true ); 
				QConnectState::getInstance()->setConnectState( true );
                qDebug() << "connected.";
                CLogging::GetInstance()->Log(LOG_INFO,"client:",  "connected:");
                CMessageBodyFactory::GetInstance();
                do_read_frame_header();

            }
            else
            {
                CMessageQueue::getInstance()->setConnectState( false ); 
				QConnectState::getInstance()->setConnectState( false );
                qDebug() << "connect failed. error:" << ec.value();
                QString temp = QString("%1").arg( ec.value() );
                CLogging::GetInstance()->Log(LOG_INFO,"client failed. error:", temp);
            }
        });
    }

    void do_read_frame_header()
    {
        // 读取帧开始符
        boost::asio::async_read(socket_,
            boost::asio::buffer(&m_frameHeaderBuf, sizeof(m_frameHeaderBuf)),
            [this](boost::system::error_code ec, std::size_t /*length*/)
        {
            if (!ec)
            {
                if (m_frameHeaderBuf == frame_header) {
                    do_read_header();
                } else {
                    qDebug() << "读取到错误的帧头,关闭连接 error:" << ec.value();
                    QString temp = QStringLiteral("读取到错误的帧头,关闭连接 error:%1").arg( ec.value() );
                    CLogging::GetInstance()->Log(LOG_INFO,"client:", temp);
                    socket_.close();
                }
            } 
            else 
            {
                CMessageQueue::getInstance()->setConnectState( false ); 
				QConnectState::getInstance()->setConnectState( false );
                qDebug() << "do_read_header async_read error:" << ec.value();
                QString temp = QString("do_read_header async_read error:%1").arg( ec.value() );
                CLogging::GetInstance()->Log(LOG_INFO,"client:", temp);
            }
        });
    }

    void do_read_header()
    {
        if (!pMessageHeaderBuf) {
            pMessageHeaderBuf = new U8[sizeof(T_MESSAGE_HEADER)];
        }
        memset(pMessageHeaderBuf, 0x00, sizeof(T_MESSAGE_HEADER));

        boost::asio::async_read(socket_,
            boost::asio::buffer(pMessageHeaderBuf, sizeof(T_MESSAGE_HEADER)),
            [this](boost::system::error_code ec, std::size_t /*length*/)
        {
            if (!ec)
            {
                qDebug() << "read header succeed";
                QString temp = QString("read header succeed");
                CLogging::GetInstance()->Log(LOG_INFO,"client:", temp);
                pMessage = new CMessage();// 产生的消息在消费完之后释放
                pMessage->AppendInputByteArray(pMessageHeaderBuf, headerBufLen);
                pMessage->ParseHeader();
                qDebug() << QStringLiteral("recv header ") << pMessage->ToString();
                temp = QString("recv header:%1").arg(pMessage->ToString());
                CLogging::GetInstance()->Log(LOG_INFO,"client:", temp);
                if (!pMessage->IsValidHeader()) {
                    if (pMessageBodyBuf)
                    {
                        delete[] pMessageBodyBuf;
                        pMessageBodyBuf = NULL;
                    }
                    qDebug() << "invalid header. close connection.";
                    temp = QString("invalid header. close connection.");
                    CLogging::GetInstance()->Log(LOG_INFO,"client:", temp);
                    socket_.close();
                    delete pMessage;
                    pMessage = NULL;
                } else if (pMessage->m_header.m_struct.bodyLength > 0) {
                    do_read_body();
                    temp = QString("read_body");
                    CLogging::GetInstance()->Log(LOG_INFO,"client:", temp);
                    qDebug() << "read_body";
                } else if(pMessage->m_header.m_struct.bodyLength == 0){
                    if (pMessageBodyBuf)
                    {
                        delete[] pMessageBodyBuf;
                        pMessageBodyBuf = NULL;
                    }
                    do_read_frame_tail();
                }
            } 
            else 
            {
                if (pMessageBodyBuf)
                {
                    delete[] pMessageBodyBuf;
                    pMessageBodyBuf = NULL;
                }
                qDebug() << "do_read_header async_read error:" << ec.value();
                QString temp = QString("do_read_header async_read error:%1").arg( ec.value() );
                CLogging::GetInstance()->Log(LOG_INFO,"client:", temp);
                if (pMessage)
                {
                    delete pMessage;
                    pMessage = NULL;
                }
            }
        });
    }

    void do_read_body()
    {
        if (pMessageBodyBuf)
        {
            delete[] pMessageBodyBuf;
            pMessageBodyBuf = NULL;
        }

        Q_ASSERT(pMessage);
        Q_ASSERT(pMessage->m_header.m_struct.bodyLength >= 0);

        if (pMessage->m_header.m_struct.cmd == 2)
        {
            qDebug() << "rep";

        }

        pMessageBodyBuf = new U8[pMessage->m_header.m_struct.bodyLength];

        //auto self(shared_from_this());
        boost::asio::async_read(socket_, 
            boost::asio::buffer(pMessageBodyBuf, pMessage->m_header.m_struct.bodyLength),
            [this/*, self*/](boost::system::error_code ec, std::size_t /*length*/)
        {
            if (!ec)
            {
                pMessage->AppendInputByteArray(pMessageBodyBuf, pMessage->m_header.m_struct.bodyLength);
                if (pMessage->ParseBody())
                {
                    if (pMessageBodyBuf)
                    {
                        delete[] pMessageBodyBuf;
                        pMessageBodyBuf = NULL;
                    }
                    do_read_frame_tail();
                } else {
                    if (pMessageBodyBuf)
                    {
                        delete[] pMessageBodyBuf;
                        pMessageBodyBuf = NULL;
                    }
                    // 关闭连接
                    socket_.close();
                    qDebug() << " parsebody failed.";
                    QString temp = QString("parsebody failed:%1").arg( pMessage->ToString() );
                    CLogging::GetInstance()->Log(LOG_INFO,"client:", temp);
                    if( pMessage )
                    {
                        delete pMessage;
                        pMessage = NULL;
                    }
                    
                }
            }
            else
            {
                if (pMessageBodyBuf)
                {
                    delete[] pMessageBodyBuf;
                    pMessageBodyBuf = NULL;
                }
                qDebug() << "do_read_body async_read error " << ec.value();
                QString temp = QString("do_read_body async_read error:%1").arg( ec.value() );
                CLogging::GetInstance()->Log(LOG_INFO,"client:", temp);
                // 关闭连接
                //socket_.close();
                if( pMessage )
                {
                    delete pMessage;
                    pMessage = NULL;
                }
            }
        });
    }
    void do_read_frame_tail()
    {
        // 读取帧结束符
        boost::asio::async_read(socket_, 
            boost::asio::buffer(&m_frameTailBuf, sizeof(m_frameTailBuf)),
            [this/*, self*/](boost::system::error_code ec, std::size_t /*length*/)
        {
            if (!ec)
            {
                // 读取消息成功 提交到处理接收数据的缓冲队列
                CMessageQueue::getInstance()->RecQueuePushBack(pMessage);               
                do_read_frame_header();
            }
            else
            {
                qDebug() << "do_read_body async_read error " << ec.value();
                QString temp = QString("do_read_body async_read error:%1").arg( ec.value() );
                CLogging::GetInstance()->Log(LOG_INFO,"client:", temp);
                // 关闭连接
                socket_.close();
                if( pMessage )
                {
                    delete pMessage;
                    pMessage = NULL;
                }
            }
        });
    }
    void do_write_frame_header()
    {
        boost::asio::async_write(socket_,
            boost::asio::buffer(&frame_header,
            sizeof(frame_header)),
            [this](boost::system::error_code ec, std::size_t /*length*/)
        {
            if (!ec)
            {
                do_write();
            }
            else
            {
                socket_.close();
                qDebug() << "async_write error:" << ec.value();
                QString temp = QString("async_write error:%1").arg( ec.value() );
                CLogging::GetInstance()->Log(LOG_INFO,"client:", temp);
            }
        });
    }
    void do_write()
    {
        CMessage * pMessage = write_msgs_.front();
        OutputByteArray out;
        pMessage->WriteToOutputByteArray(out);

        qDebug() << "will write " << out.Size() << "bytes";
        
        boost::asio::async_write(socket_,
            boost::asio::buffer(out.Data(),
            out.Size()),
            [this](boost::system::error_code ec, std::size_t /*length*/)
        {
            if (!ec)
            {
                CMessage * pMessage = write_msgs_.front();
                qDebug() << "write" << pMessage->Length() <<  "bytes succeed";
                QString temp = QString("write:%1 btyes succeed").arg( pMessage->Length() );
                CLogging::GetInstance()->Log(LOG_INFO,"client:", temp);
                write_msgs_.pop_front();
                delete pMessage;

                do_write_frame_tail();
            }
            else
            {
                CMessage * pMessage = write_msgs_.front();
                delete pMessage;
                socket_.close();
                qDebug() << "async_write error:" << ec.value();
                QString temp = QString("async_write error:%1").arg( ec.value() );
                CLogging::GetInstance()->Log(LOG_INFO,"client:", temp);
            }
        });
    }
    void do_write_frame_tail()
    {
        boost::asio::async_write(socket_,
            boost::asio::buffer(&frame_tail,
            sizeof(frame_tail)),
            [this](boost::system::error_code ec, std::size_t /*length*/)
        {
            if (!ec)
            {
                if (!write_msgs_.empty())
                {
                    do_write_frame_header();
                }
            }
            else
            {
                socket_.close();
                qDebug() << "async_write error:" << ec.value();
                QString temp = QString("async_write error:%1").arg( ec.value() );
                CLogging::GetInstance()->Log(LOG_INFO,"client:", temp);
            }
        });
    }

private:
    boost::asio::io_service&    io_service_;
    tcp::socket                 socket_;
    CMessage                    read_msg_;
    session_message_queue       write_msgs_;
    U8                        * pMessageHeaderBuf;
    U8                        * pMessageBodyBuf;
    U8                          m_frameHeaderBuf;
    U8                          m_frameTailBuf[2];
    static int                  headerBufLen;
    CMessage                  * pMessage;
    CMessage                  *tempMessage;
    static U8                   frame_header;
    static U8                   frame_tail[2];
};

__declspec(selectany) U8 CTcpClient::frame_tail[2] = {0x0D, 0x0A};
__declspec(selectany) U8 CTcpClient::frame_header = 0x5A;
__declspec(selectany) int CTcpClient::headerBufLen = sizeof(T_MESSAGE_HEADER);



#endif//!_CHAT_CLIENT_H_
