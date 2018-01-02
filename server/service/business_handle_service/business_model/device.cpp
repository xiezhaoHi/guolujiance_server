#include "device.h"
#include "service/business_handle_service/business_model/rig.h"
#include "service/socket_service/tcp_session.h"
#include "service/socket_service/tcp_session_group.h"
#include <QString>


CDevice::CDevice()
{
    type = ENTITY_DEVICE;
    deviceType = DEVICE_TYPE_UNKNOWN;
    m_pRig = NULL;
    m_pSession = NULL;
}

CDevice::~CDevice()
{

}

void CDevice::SetRig(rig_ptr rig)
{
    m_pRig = rig;
}

rig_ptr CDevice::GetRig()
{
    return m_pRig;
}

void CDevice::SetSession(tcp_session_ptr session)
{
    Q_ASSERT(m_pRig);

    if (m_pSession != session){
        if (m_pSession) {
            m_pRig->RemoveDeviceSession(session);
            m_pSession->set_entity(NULL);
        }

        if (session && session->room_ptr_) {
            session->room_ptr_->leave(session);
            session->room_ptr_ = NULL;
        }

        m_pSession = session;

        if (session) {
            m_pRig->AddDeviceSession(session);
            session->set_entity(this->shared_from_this());
        }

        if (m_pRig) {
            emit m_pRig->rigDeviceUpdated();
        }
    }
}

const tcp_session_ptr CDevice::GetSession()
{
    return m_pSession;
}

void CDevice::BroadcastToPCClients(message_ptr msg)
{
    Q_ASSERT(msg);
    
    if (m_pRig) {
        m_pRig->BroadcastDeviceRealtimeData(msg);
    } else {
        Q_ASSERT(false);
    }
}

QString CDevice::GetDisplayText()
{
    return QStringLiteral("%1 %2").arg(name).arg(code);
}

bool CDevice::IsDeviceOnline()
{
    return m_pSession != NULL;
}

void CDevice::SetLoginTime(date_time_ptr ptr)
{
    this->m_pLoginTime = ptr;
    if (m_pRig) {
        emit m_pRig->rigDeviceUpdated();
    }
}

date_time_ptr CDevice::GetLoginTime()
{
    return m_pLoginTime;
}
