#ifndef _TCP_SESSION_GROUP_H_
#define _TCP_SESSION_GROUP_H_

#include <deque>
#include <set>
#include <memory>
#include <QMutex>
#include <QMutexLocker>
#include "i_tcp_session_participant.h"

class CTcpSessionGroup;
typedef std::shared_ptr<CTcpSessionGroup> tcp_session_group_ptr;
typedef std::deque<message_ptr> session_message_queue;

class CTcpSessionGroup
{
public:
    CTcpSessionGroup(){ m_pLock = new QMutex(QMutex::Recursive); }

    ~CTcpSessionGroup(){ delete m_pLock; }

    void join(i_tcp_session_participant_ptr participant)
    {
        QMutexLocker lock(m_pLock);
        participants_.insert(participant);
        for (auto msg: recent_msgs_)
            participant->deliver(msg);
    }

    void leave(i_tcp_session_participant_ptr participant)
    {
        QMutexLocker lock(m_pLock);
        participants_.erase(participant);
    }

    void deliver(message_ptr msg)
    {
        QMutexLocker lock(m_pLock);
        recent_msgs_.push_back(msg);
        while (recent_msgs_.size() > max_recent_msgs)
            recent_msgs_.pop_front();

        for (auto participant: participants_)
            participant->deliver(msg);
    }

    void GetParticipants(std::set<i_tcp_session_participant_ptr> & participants)
    {
        QMutexLocker lock(m_pLock);
        for (auto participant: participants_)
        {
            participants.insert(participant);
        }
    }

    void RemoveEveryParticipant()
    {
        std::set<i_tcp_session_participant_ptr> participants;
        GetParticipants(participants);
        for (i_tcp_session_participant_ptr participant: participants_)
        {
            
        }

        participants.clear();
    }

private:
    std::set<i_tcp_session_participant_ptr> participants_;// 客户端会话
    session_message_queue                   recent_msgs_;
    QMutex                                * m_pLock;
    enum { max_recent_msgs = 0 };// 不保存最近的消息
};

#endif//!_TCP_SESSION_GROUP_H_