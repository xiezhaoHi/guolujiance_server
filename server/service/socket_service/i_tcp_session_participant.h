/*****************************************************
** �ļ�����  i_tcp_session_participant.h
** ��  Ȩ��  XXX ��Ȩ����
** ��  ����  
** ���ݼ�����TCP�Ự��������
** �������ڣ�2015/07/15
** �����ˣ�  ����
** �޸ļ�¼��
   ����        �汾      �޸���    �޸����� 
    
*****************************************************/
#ifndef _I_TCP_SESSION_PARTICIPANT_H_
#define _I_TCP_SESSION_PARTICIPANT_H_

#include <memory>
#include "message\message.h"



class ITcpSessionParticipant
{
public:

    virtual ~ITcpSessionParticipant() {}

    virtual void deliver(message_ptr msg) = 0;
};

typedef std::shared_ptr<ITcpSessionParticipant> i_tcp_session_participant_ptr;


#endif//!_I_TCP_SESSION_PARTICIPANT_H_
