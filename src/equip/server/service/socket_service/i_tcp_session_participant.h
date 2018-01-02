/*****************************************************
** 文件名：  i_tcp_session_participant.h
** 版  权：  XXX 版权所有
** 版  本：  
** 内容简述：TCP会话参与者类
** 创建日期：2015/07/15
** 创建人：  唐桥
** 修改记录：
   日期        版本      修改人    修改内容 
    
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
