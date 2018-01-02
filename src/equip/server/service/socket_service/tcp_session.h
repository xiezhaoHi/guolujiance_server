/*****************************************************
** �ļ�����  tcp_session.h
** ��  Ȩ��  XXX ��Ȩ����
** ��  ����  
** ���ݼ�����TCP�Ự
** �������ڣ�2015/07/15
** �����ˣ�  ����
** �޸ļ�¼��
   ����        �汾      �޸���    �޸����� 
    
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

    // ������Ϣ
    void deliver(message_ptr msg);

    void set_entity(base_entity_ptr entity);

    // ��ȡ���ӻỰ�󶨵�ҵ��ʵ��
    base_entity_ptr get_entity();

    // �Ͽ����ӻỰ
    void stop();

    // ��ȡ��ʾ�ַ���(ip:�˿�)
    QString get_display_text();

    // ��ȡ���һ�εĽ���ʱ��
    quint64 GetLatestRecvTime();

private:
    // ��ȡ֡ͷ��ʼ�� 0x5A
    void do_read_frame_header();

    void do_read_header();

    void do_read_body();

    // ��ȡ֡β�ṹ�� 0x0D 0x0A
    void do_read_frame_tail();

    void do_write();

    // д֡ͷ��ʼ�� 0x5A
    void do_write_frame_header();

    // д֡β������ 0x0D 0x0A
    void do_write_frame_tail();

    // �ر�����
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
    quint64                 m_u64LatestRecvTime;// ���һ�ν�������ʱ��

    static int              headerBufLen; // ��ͷ���ȹ̶�
    static U8               frame_header; // ֡��ʼ�� 0x5A
    static U8               frame_tail[2];// ֡�ṹ�� 0x0D 0x0A
};


#endif//!_TCP_SESSION_H_