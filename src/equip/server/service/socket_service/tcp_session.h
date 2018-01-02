/*****************************************************
** 文件名：  tcp_session.h
** 版  权：  XXX 版权所有
** 版  本：  
** 内容简述：TCP会话
** 创建日期：2015/07/15
** 创建人：  唐桥
** 修改记录：
   日期        版本      修改人    修改内容 
    
*****************************************************/
#ifndef _TCP_SESSION_H_
#define _TCP_SESSION_H_

#include <memory>
#include <deque>
#include <boost/asio.hpp>
#include <QString>
#include <QMutex>
#include <QDateTime>
#include "i_tcp_session_participant.h"
#include "tcp_session_group.h"


using boost::asio::ip::tcp;

class CTcpSession;
typedef std::shared_ptr<CTcpSession> tcp_session_ptr;

class CBaseEntity;
typedef std::shared_ptr<CBaseEntity> base_entity_ptr;

class CTcpSession
    : public ITcpSessionParticipant,
    public std::enable_shared_from_this<CTcpSession>
{
public:
    CTcpSession(tcp::socket &socket, tcp_session_group_ptr room, 
        boost::asio::strand &strand_);

    ~CTcpSession();

    void start();

    // 发布消息
    void deliver(message_ptr msg);

    void set_entity(base_entity_ptr entity);

    // 获取连接会话绑定的业务实体
    base_entity_ptr get_entity();

    // 断开连接会话
    void stop();

    // 获取显示字符串(ip:端口)
    QString get_display_text();

    // 获取最近一次的接收时间
    quint64 GetLatestRecvTime();

private:
    // 读取帧头开始符 0x5A
    void do_read_frame_header();

    void do_read_header();

    void do_read_body();

    // 读取帧尾结构符 0x0D 0x0A
    void do_read_frame_tail();

    void do_write();

    // 写帧头开始符 0x5A
    void do_write_frame_header();

    // 写帧尾结束符 0x0D 0x0A
    void do_write_frame_tail();

    // 关闭连接
    void do_close();

public:
    tcp_session_group_ptr   room_ptr_;
    base_entity_ptr         m_entity;

private:
    tcp::socket             socket_;
    boost::asio::strand     strand_;

    U8                    * m_pMessageHeaderBuf;
    U8                    * m_pMessageBodyBuf;
    U8                    * m_pSendBuf;
    U8                      m_frameHeaderBuf;
    U8                      m_frameTailBuf[2];
    QMutex                * m_pMutexForWriteMsgs;
    session_message_queue   write_msgs_;
    message_ptr             m_pMessageRead;
    quint64                 m_u64LatestRecvTime;// 最近一次接收数据时间

    static int              headerBufLen; // 包头长度固定
    static U8               frame_header; // 帧开始符 0x5A
    static U8               frame_tail[2];// 帧结构符 0x0D 0x0A
};


#endif//!_TCP_SESSION_H_