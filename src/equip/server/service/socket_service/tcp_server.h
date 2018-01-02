/*****************************************************
** 文件名：  tcp_server.h
** 版  权：  XXX 版权所有
** 版  本：  
** 内容简述：TCP连接服务端
** 创建日期：2015/07/15
** 创建人：  唐桥
** 修改记录：
   日期        版本      修改人    修改内容 
    
*****************************************************/
#ifndef _TCP_SERVER_H_
#define _TCP_SERVER_H_

#include <iostream>
#include <memory>
#include <set>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <QString>
#include <QDateTime>
#include "tcp_session_group.h"
#include "tcp_session.h"
#include "service/log_service/log_service.h"

using namespace boost::asio;
using boost::asio::ip::tcp;

class CTcpServer
{
public:
    CTcpServer(boost::asio::io_service& io_service,
        const tcp::endpoint& endpoint)
        : acceptor_(io_service, endpoint),
        socket_(io_service), 
        strand_(io_service)
    {
        room_ptr_ = std::make_shared<CTcpSessionGroup>();

        LOG_INFO() << QStringLiteral("监听端口") << endpoint.port();
        do_accept();
    }

private:
    void do_accept()
    {
        acceptor_.async_accept(socket_,
            [this](boost::system::error_code ec)
        {
            if (!ec)
            {
                LOG_INFO() << QStringLiteral("ACCEPT [")
                    << QString(socket_.remote_endpoint().address().to_string().c_str())
                    << QStringLiteral(" : ")
                    << socket_.remote_endpoint().port()
                    << QStringLiteral("]");

                std::make_shared<CTcpSession>(std::move(socket_), room_ptr_, strand_)->start();
            } 
            else
            {
                LOG_WARING() << QStringLiteral("accept error. error:%1").arg(ec.value());
            }

            do_accept();
        });
        ClearInvalidPaticipants();
    }

    void ClearInvalidPaticipants()
    {
        // 清理掉所有超时未经过登录验证的连接
        quint64 current = QDateTime::currentMSecsSinceEpoch();
        std::set<i_tcp_session_participant_ptr> participants;
        room_ptr_->GetParticipants(participants);
        for (i_tcp_session_participant_ptr paticipant : participants)
        {
            tcp_session_ptr pSession = std::static_pointer_cast<CTcpSession>(paticipant);
            if (pSession)
            {
                quint64 elapsed = current - pSession->GetLatestRecvTime();
                quint64 sec = elapsed / 1000;
                if (sec >= 150)// 连接空闲150秒后，服务器自动关闭
                {
                    pSession->stop();
                }
            }
        }
        participants.clear();
    }

private:
    tcp::acceptor               acceptor_;
    tcp::socket                 socket_;
    tcp_session_group_ptr       room_ptr_;
    boost::asio::strand         strand_;
};

#endif//!_TCP_SERVER_H_