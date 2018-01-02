#include "tcp_session.h"
#include "service/log_service/log_service.h"
#include "service/socket_service/tcp_session_message_handler_hub.h"
#include "structs/structs.h"
#include "protocol/protocol_structs.h"
#include "service/business_handle_service/business_model/device.h"
#include "service/business_handle_service/business_model/user.h"
#include "service/business_handle_service/business_handle_service.h"
#include <QMutexLocker>
#include <boost/bind.hpp>

int CTcpSession::headerBufLen = sizeof(T_MESSAGE_HEADER);
U8 CTcpSession::frame_header = 0x5A;
U8 CTcpSession::frame_tail[2] = {0x0D, 0x0A};



CTcpSession::CTcpSession(tcp::socket &socket, tcp_session_group_ptr room,
                         boost::asio::strand &strand_)
    : socket_(std::move(socket)),
    room_ptr_(room),
    strand_(strand_)
{
    boost::asio::socket_base::linger option(true, 0);
    socket_.set_option(option);

    m_pMutexForWriteMsgs = new QMutex(QMutex::Recursive);
    m_pMessageHeaderBuf = new U8[headerBufLen];
    m_pMessageBodyBuf = NULL;
    m_pSendBuf = NULL;
    m_entity = NULL;
    m_frameHeaderBuf = 0x00;
    memset(&m_frameTailBuf, 0x00, sizeof(m_frameTailBuf));

    m_u64LatestRecvTime = 0;
}

CTcpSession::~CTcpSession()
{
    if (m_pMessageHeaderBuf) {
        delete m_pMessageHeaderBuf;
        m_pMessageHeaderBuf = NULL;
    }

    if (m_pMessageBodyBuf) {
        delete m_pMessageBodyBuf;
        m_pMessageBodyBuf = NULL;
    }

    if (m_pSendBuf) {
        delete[] m_pSendBuf;
        m_pSendBuf = NULL;
    }

    m_pMutexForWriteMsgs->lock();
    while (!write_msgs_.empty()) {
        write_msgs_.pop_front();
    }
    m_pMutexForWriteMsgs->unlock();
}

void CTcpSession::start()
{
    LOG_DEBUG() << QStringLiteral("TCP SESSION START");
    room_ptr_->join(shared_from_this());
    m_u64LatestRecvTime = QDateTime::currentMSecsSinceEpoch();
    do_read_frame_header();
}

void CTcpSession::deliver(message_ptr msg)
{
    QMutexLocker lock(m_pMutexForWriteMsgs);
    bool write_in_progress = !write_msgs_.empty();
    write_msgs_.push_back(msg);
    if (!write_in_progress)
    {
        do_write_frame_header();
    }
}

void CTcpSession::set_entity(base_entity_ptr entity)
{
    m_entity = entity;
}

void CTcpSession::do_read_header()
{
    if (!socket_.is_open()) {
        CBusinessHandleService::GetInstance()->HandleTcpSessionClosed(shared_from_this());
        return;
    }

    memset(m_pMessageHeaderBuf, 0x00, headerBufLen);
  //  LOG_DEBUG() << QStringLiteral("WAIT TO READ HEADER. SESSION[%1:%2] ")
  //      .arg(socket_.remote_endpoint().address().to_string().c_str()).arg(socket_.remote_endpoint().port());
    auto self(shared_from_this());
    boost::asio::async_read(socket_, 
        boost::asio::buffer(m_pMessageHeaderBuf, headerBufLen),
        [this, self](boost::system::error_code ec, std::size_t /*length*/)
    {
        if (!ec)
        {
            m_pMessageRead = std::make_shared<CMessage>();// 产生的消息在消费完之后释放
            m_pMessageRead->AppendInputByteArray(m_pMessageHeaderBuf, headerBufLen);
            m_pMessageRead->ParseHeader();
           // LOG_DEBUG() << QStringLiteral("RECV HEADER[%1]").arg(m_pMessageRead->ToString());

            if (!m_pMessageRead->IsValidHeader()) {// 头部数据无效，需要主动断开连接
                // 主动断开连接
                LOG_WARING() << QStringLiteral("消息头部数据无效,主动关闭连接. SESSION[%1:%2] HEADER[%3]")
                    .arg(socket_.remote_endpoint().address().to_string().c_str())
                    .arg(socket_.remote_endpoint().port()).arg(m_pMessageRead->ToString());
                CBusinessHandleService::GetInstance()->HandleTcpSessionClosed(shared_from_this());
            } else if (m_pMessageRead->m_header.m_struct.bodyLength > 0) {
                do_read_body();
            } else if (m_pMessageRead->m_header.m_struct.bodyLength == 0) {
                do_read_frame_tail();
            } else {
                Q_ASSERT(false);// 不可能走到的分支
            }
        }
        else
        {
            LOG_WARING() << QStringLiteral("读取消息头部失败. SESSION[%1:%2] ERROR[%3]")
                .arg(socket_.remote_endpoint().address().to_string().c_str())
                .arg(socket_.remote_endpoint().port()).arg(ec.value());
            CBusinessHandleService::GetInstance()->HandleTcpSessionClosed(shared_from_this());
        }
    });
}

void CTcpSession::do_read_body()
{
    if (m_pMessageBodyBuf) {
        delete[] m_pMessageBodyBuf;
        m_pMessageBodyBuf = NULL;
    }

    if (!socket_.is_open()) {
        CBusinessHandleService::GetInstance()->HandleTcpSessionClosed(shared_from_this());
        return;
    }

    m_pMessageBodyBuf = new U8[m_pMessageRead->m_header.m_struct.bodyLength];
    memset(m_pMessageBodyBuf, 0x00, m_pMessageRead->m_header.m_struct.bodyLength);
   // LOG_DEBUG() << QStringLiteral("WAIT TO READ BODY. SESSION[%1:%2]. [%3] BYTES")
    //    .arg(socket_.remote_endpoint().address().to_string().c_str())
     //   .arg(socket_.remote_endpoint().port())
     //   .arg(m_pMessageRead->m_header.m_struct.bodyLength);
    auto self(shared_from_this());
    boost::asio::async_read(socket_, 
        boost::asio::buffer(m_pMessageBodyBuf, m_pMessageRead->m_header.m_struct.bodyLength),
        [this, self](boost::system::error_code ec, std::size_t length)
    {
       // LOG_DEBUG() << "READ BODY. WANT " << m_pMessageRead->m_header.m_struct.bodyLength
      //      << " BYTES. RECVED " << length << "BYTES";
        if (!ec) {
            m_pMessageRead->AppendInputByteArray(m_pMessageBodyBuf, m_pMessageRead->m_header.m_struct.bodyLength);
            if (m_pMessageRead->ParseBody()) {
                do_read_frame_tail();
				//LOG_WARING() << QStringLiteral("body数据:%1")
				//	.arg(m_pMessageRead->ToString());
            } else {
                // 关闭连接
                LOG_WARING() << QStringLiteral("解析消息体失败,主动关闭连接. SESSION[%1:%2] MSG[%3]")
                    .arg(socket_.remote_endpoint().address().to_string().c_str())
                    .arg(socket_.remote_endpoint().port())
                    .arg(m_pMessageRead->ToString());
                CBusinessHandleService::GetInstance()->HandleTcpSessionClosed(shared_from_this());
            }
        } else {
            LOG_WARING() << QStringLiteral("读取消息体失败. ERROR[%1]").arg(ec.value());
            CBusinessHandleService::GetInstance()->HandleTcpSessionClosed(shared_from_this());
        }
    });
}

void CTcpSession::do_write()
{
    if (!socket_.is_open()) {
        CBusinessHandleService::GetInstance()->HandleTcpSessionClosed(shared_from_this());
        return;
    }

    QMutexLocker lock(m_pMutexForWriteMsgs);
    message_ptr pMessage = write_msgs_.front();
    OutputByteArray out;
    pMessage->WriteToOutputByteArray(out);

   // LOG_DEBUG() << "WRITE" << out.Size() << "BYTES. MSG:" << pMessage->ToString();

    auto self(shared_from_this());
    boost::asio::async_write(socket_,
        boost::asio::buffer(out.Data(),
        out.Size()),
        [this, self](boost::system::error_code ec, std::size_t /*length*/)
    {
        if (!ec)
        {
            QMutexLocker lock(m_pMutexForWriteMsgs);
            Q_ASSERT(write_msgs_.front() != NULL);
           // LOG_DEBUG() << QStringLiteral("已发送 ") << write_msgs_.front()->ToString();

            do_write_frame_tail();
        }
        else
        {
            LOG_WARING() << QStringLiteral("发送数据失败. SESSION[%1:%2]. ERROR[%3]")
                .arg(socket_.remote_endpoint().address().to_string().c_str())
                .arg(socket_.remote_endpoint().port())
                .arg(ec.value());
            CBusinessHandleService::GetInstance()->HandleTcpSessionClosed(shared_from_this());
        }
    });
}

void CTcpSession::stop()
{
    if (socket_.is_open()) {
        LOG_DEBUG() << QStringLiteral("SESSION[%1:%2] CLOSED.")
            .arg(socket_.remote_endpoint().address().to_string().c_str())
            .arg(socket_.remote_endpoint().port());
        //socket_.close(); 
        strand_.post(bind(&CTcpSession::do_close, shared_from_this()));
    }

    if (room_ptr_) {
        room_ptr_->leave(shared_from_this());
        room_ptr_ = NULL;
    }

    set_entity(NULL);
}

base_entity_ptr CTcpSession::get_entity()
{
    return m_entity;
}

QString CTcpSession::get_display_text()
{
    if (!socket_.is_open()) {
        return QStringLiteral("N/A");
    } else {
        return QStringLiteral("%1:%2")
            .arg(socket_.remote_endpoint().address().to_string().c_str())
            .arg(socket_.remote_endpoint().port());
    }
}

void CTcpSession::do_read_frame_header()
{
    if (!socket_.is_open()) {
        CBusinessHandleService::GetInstance()->HandleTcpSessionClosed(shared_from_this());
        return;
    }

    m_frameHeaderBuf = 0x00;

    auto self(shared_from_this());
    boost::asio::async_read(socket_, 
        boost::asio::buffer(&m_frameHeaderBuf, sizeof(m_frameHeaderBuf)),
        [this, self](boost::system::error_code ec, std::size_t /*length*/)
    {
        if (!ec)
        {
            if (m_frameHeaderBuf == frame_header) {
                do_read_header();
            } else {
                // 帧头字符无效，关闭连接
                LOG_WARING() << QStringLiteral("帧开始符[%1]无效,主动关闭连接. SESSION[%2:%3]")
                    .arg(m_frameHeaderBuf)
                    .arg(socket_.remote_endpoint().address().to_string().c_str())
                    .arg(socket_.remote_endpoint().port());
				//do_read_header();
                CBusinessHandleService::GetInstance()->HandleTcpSessionClosed(shared_from_this());
            }
        }
        else
        {
            LOG_WARING() << QStringLiteral("读取帧开始符失败. SESSION[%1:%2] ERROR[%3]")
                .arg(socket_.remote_endpoint().address().to_string().c_str())
                .arg(socket_.remote_endpoint().port()).arg(ec.value());
            CBusinessHandleService::GetInstance()->HandleTcpSessionClosed(shared_from_this());
        }
    });
}

void CTcpSession::do_read_frame_tail()
{
    if (!socket_.is_open()) {
        CBusinessHandleService::GetInstance()->HandleTcpSessionClosed(shared_from_this());
        return;
    }

    memset(&m_frameTailBuf, 0x00, sizeof(m_frameTailBuf));

    auto self(shared_from_this());
    boost::asio::async_read(socket_, 
        boost::asio::buffer(&m_frameTailBuf, sizeof(m_frameTailBuf)),
        [this, self](boost::system::error_code ec, std::size_t /*length*/)
    {
        if (!ec)
        {
            // 更新最近一次接收数据时间
            m_u64LatestRecvTime = QDateTime::currentMSecsSinceEpoch();

            if (m_frameTailBuf[0] == frame_tail[0]
             && m_frameTailBuf[1] == frame_tail[1]) {
                // 提交到处理队列
				if(m_pMessageRead->m_header.m_struct.bodyLength > 0)
					CTcpSessionMessageHandlerHub::GetInstance()->CommitSessionMessage(this->shared_from_this(), m_pMessageRead);
				//LOG_DEBUG() << QStringLiteral("完整数据包接受完成!");
				if (m_pMessageRead->m_header.m_struct.cmd == PC_CLIENT_CMD_EXIT
                    || m_pMessageRead->m_header.m_struct.cmd == DEVICE_CMD_EXIT) {
                    // 接收到退出命令，不再接收数据
                    LOG_DEBUG() << QStringLiteral("连接[%1]接收到退出命令[%2]，不再主动接收数据")
                        .arg(get_display_text()).arg(m_pMessageRead->m_header.m_struct.cmd);
                } else {
                    do_read_frame_header();
                }
            } else {
                // 帧结构符无效，关闭连接
                LOG_WARING() << QStringLiteral("帧结束符无效,主动关闭连接. SESSION[%1:%2]")
                    .arg(socket_.remote_endpoint().address().to_string().c_str())
                    .arg(socket_.remote_endpoint().port());

				LOG_DEBUG() << QStringLiteral("结尾数据为:%1,%2").arg(m_frameTailBuf[0]).arg(m_frameTailBuf[1]);

                CBusinessHandleService::GetInstance()->HandleTcpSessionClosed(shared_from_this());
            }
        }
        else
        {
            LOG_WARING() << QStringLiteral("读取帧结束符失败. SESSION[%1:%2] ERROR [%3]")
                .arg(socket_.remote_endpoint().address().to_string().c_str())
                .arg(socket_.remote_endpoint().port()).arg(ec.value());
            CBusinessHandleService::GetInstance()->HandleTcpSessionClosed(shared_from_this());
        }
    });
}

void CTcpSession::do_write_frame_header()
{
    if (!socket_.is_open()) {
        CBusinessHandleService::GetInstance()->HandleTcpSessionClosed(shared_from_this());
        return;
    }

    auto self(shared_from_this());
    boost::asio::async_write(socket_,
        boost::asio::buffer(&frame_header,
        sizeof(frame_header)),
        [this, self](boost::system::error_code ec, std::size_t /*length*/)
    {
        if (!ec)
        {
            do_write();
        }
        else
        {
            LOG_WARING() << QStringLiteral("写帧开始符失败. SESSION[%1:%2]. ERROR[%3]")
                .arg(socket_.remote_endpoint().address().to_string().c_str())
                .arg(socket_.remote_endpoint().port()).arg(ec.value());
            CBusinessHandleService::GetInstance()->HandleTcpSessionClosed(shared_from_this());
        }
    });
}

void CTcpSession::do_write_frame_tail()
{
    if (!socket_.is_open()) {
        CBusinessHandleService::GetInstance()->HandleTcpSessionClosed(shared_from_this());
        return;
    }

    auto self(shared_from_this());
    boost::asio::async_write(socket_,
        boost::asio::buffer(&frame_tail,
        sizeof(frame_tail)),
        [this, self](boost::system::error_code ec, std::size_t /*length*/)
    {
        if (!ec)
        {
            QMutexLocker lock(m_pMutexForWriteMsgs);
            write_msgs_.pop_front();
            if (!write_msgs_.empty())
            {
                do_write_frame_header();
            }
        }
        else
        {
            LOG_WARING() << QStringLiteral("写帧结束符失败. SESSION[%1:%2]. ERROR[%3]")
                .arg(socket_.remote_endpoint().address().to_string().c_str())
                .arg(socket_.remote_endpoint().port()).arg(ec.value());
            CBusinessHandleService::GetInstance()->HandleTcpSessionClosed(shared_from_this());
        }
    });
}

quint64 CTcpSession::GetLatestRecvTime()
{
    return m_u64LatestRecvTime;
}

void CTcpSession::do_close()
{
    try {    
        socket_.shutdown(tcp::socket::shutdown_both);
        socket_.close();
    } catch (std::exception& e) {
        LOG_WARING() << ("Connection") <<  e.what();
    }
}
