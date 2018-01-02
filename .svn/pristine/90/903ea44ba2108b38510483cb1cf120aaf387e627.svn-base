#ifndef _CHAT_CLIENT_H_
#define _CHAT_CLIENT_H_

#include <memory>
#include <deque>
#include <iostream>
#include <boost/asio.hpp>
#include "message/message.h"
#include <QDebug>
#include <QQueue>
#include <QMutex>
#include <QMutexLocker>
#include <boost/bind.hpp>
#include "message/message_body_factory.h"

using namespace std;
using boost::asio::ip::tcp;
using namespace boost;


typedef QQueue<CMessage *> session_message_queue;

class CTcpClient : public std::enable_shared_from_this<CTcpClient>
{
public:
    CTcpClient(boost::asio::io_service& io_service,
        tcp::resolver::iterator endpoint_iterator)
        : io_service_(io_service),
        socket_(io_service)
    {
        m_pMutexForWriteMsgs = new QMutex(QMutex::Recursive);
        do_connect(endpoint_iterator);
        pMessageHeaderBuf = NULL;
        pMessageBodyBuf = NULL;
    }

	/*
	测试func
	*/

	void test_write(CMessage* msg)
	{
		OutputByteArray out;
		msg->WriteToOutputByteArray(out);
		boost::system::error_code ec;
		socket_.write_some(boost::asio::buffer(&frame_header, sizeof(frame_header)), ec);
		socket_.write_some(boost::asio::buffer(out.Data(), out.Size()), ec);
		socket_.write_some(boost::asio::buffer(&m_frameTailBuf, sizeof(m_frameTailBuf)), ec);
		if (ec)
		{
			return;
		}
	}

    void write(CMessage *  msg)
    {
        io_service_.post(
            [this, msg]()
        {
            QMutexLocker lock(m_pMutexForWriteMsgs);
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
        do_read_header();
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
                qDebug() << "connected.";
                CMessageBodyFactory::GetInstance();
                do_read_frame_header();
                
            }
            else
            {
                qDebug() << "connect failed. error:" << ec.value();
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
                    socket_.close();
                }
            } 
            else 
            {
                qDebug() << "do_read_header async_read error:" << ec.value();
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
                pMessage = new CMessage();// 产生的消息在消费完之后释放
                pMessage->AppendInputByteArray(pMessageHeaderBuf, headerBufLen);
                pMessage->ParseHeader();
                if (!pMessage->IsValidHeader()) {
                    qDebug() << "invalid header";
                }
                if (pMessage->m_header.m_struct.bodyLength > 0) {
                    do_read_body();
                } else if(pMessage->m_header.m_struct.bodyLength == 0){
                    // 提交到处理队列
                    qDebug() << "RECV MSG:" <<  pMessage->ToString() << "\n";
                    delete pMessage;
                    pMessage = NULL;
                    do_read_frame_tail();
                }
            } 
            else 
            {
                qDebug() << "do_read_header async_read error:" << ec.value();
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
                    // TODO 读取消息成功 提交到处理接收数据的缓冲队列
                    // 
                    qDebug() << "RECV MSG:" <<  pMessage->ToString() << "\n";

                    delete pMessage;
                    pMessage = NULL;

                    do_read_frame_tail();
                } else {
                    // 关闭连接
                    socket_.close();
                    qDebug() << " parsebody failed.";
                }
            }
            else
            {
                qDebug() << "do_read_body async_read error " << ec.value();
                // 关闭连接
                socket_.close();
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
                // TODO 读取消息成功 提交到处理接收数据的缓冲队列
                do_read_frame_header();
            }
            else
            {
                qDebug() << "do_read_body async_read error " << ec.value();
                // 关闭连接
                socket_.close();
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
            }
        });
    }

    void do_write()
    {
        QMutexLocker lock(m_pMutexForWriteMsgs);
        CMessage * pMessage = write_msgs_.front();
        OutputByteArray out;
        pMessage->WriteToOutputByteArray(out);

        boost::asio::async_write(socket_,
            boost::asio::buffer(out.Data(),
            out.Size()),
            [this](boost::system::error_code ec, std::size_t /*length*/)
        {
            if (!ec)
            {
                QMutexLocker lock(m_pMutexForWriteMsgs);
                if (!write_msgs_.empty()) {
                    CMessage * pMessage = write_msgs_.front();
                    qDebug() << "SEND MSG:" << pMessage->Length() <<  "bytes succeed" << pMessage->ToString() << "\n";
                    write_msgs_.pop_front();
                    delete pMessage;
                }

                do_write_frame_tail();
            }
            else
            {
                socket_.close();
                qDebug() << "async_write error:" << ec.value();
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
                QMutexLocker lock(m_pMutexForWriteMsgs);
                if (!write_msgs_.empty())
                {
                    do_write_frame_header();
                }
            }
            else
            {
                socket_.close();
                qDebug() << "async_write error:" << ec.value();
            }
        });
    }

private:
    boost::asio::io_service&    io_service_;
    tcp::socket                 socket_;
    CMessage                    read_msg_;
    QMutex                    * m_pMutexForWriteMsgs;
    session_message_queue       write_msgs_;
    U8                          m_frameHeaderBuf;
    U8                          m_frameTailBuf[2];
    U8                        * pMessageHeaderBuf;
    U8                        * pMessageBodyBuf;
    static int                  headerBufLen;
    CMessage                  * pMessage;
    static U8                   frame_header;
    static U8                   frame_tail[2];
};

__declspec(selectany) U8 CTcpClient::frame_tail[2] = {0x0D, 0x0A};

__declspec(selectany) U8 CTcpClient::frame_header = 0x5A;

__declspec(selectany) int CTcpClient::headerBufLen = sizeof(T_MESSAGE_HEADER);


#endif//!_CHAT_CLIENT_H_
