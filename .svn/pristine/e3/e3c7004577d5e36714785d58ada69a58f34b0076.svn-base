#include "user.h"
#include "rig.h"
#include "service/socket_service/tcp_session.h"
#include "service/socket_service/tcp_session_group.h"
#include <QString>


CUser::CUser()
{
    type = ENTITY_USER;
    m_pRig = NULL;
    m_pSession = NULL;
}

CUser::~CUser()
{

}

void CUser::SetRig(rig_ptr rig)
{
    m_pRig = rig;
}

rig_ptr CUser::GetRig()
{
    return m_pRig;
}

void CUser::SetSession(tcp_session_ptr session)
{
    if (m_pSession != session){
        Q_ASSERT(m_pRig);

        if (m_pSession) {
            m_pRig->RemoveClientSession(m_pSession);
            m_pSession->set_entity(NULL);
        }

        if (session && session->room_ptr_) {
            session->room_ptr_->leave(session);
            session->room_ptr_ = NULL;
        }

        m_pSession = session;

        if (session) {
            session->set_entity(this->shared_from_this());
            m_pRig->AddClientSession(session);
        }

        if (m_pRig) {
            emit m_pRig->rigUserUpdated();
        }
    }
}

const tcp_session_ptr CUser::GetSession()
{
    return m_pSession;
}

QString CUser::GetDisplayText()
{
    return name;
}

void CUser::SetLoginTime(date_time_ptr ptr)
{
    m_pLoginTime = ptr;

    if (m_pRig) {
        emit m_pRig->rigUserUpdated();
    }
}

date_time_ptr CUser::GetLoginTime()
{
    return m_pLoginTime;
}


