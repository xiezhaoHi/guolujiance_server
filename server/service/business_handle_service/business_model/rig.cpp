#include "rig.h"
#include "service/socket_service/tcp_session_group.h"
#include "service/socket_service/tcp_session.h"
#include "service/log_service/log_service.h"
#include "user.h"
#include "device.h"
#include "oil_field.h"
#include <QMutexLocker>
#include <QMutex>

CRig::CRig()
{
    type = ENTITY_RIG;
    m_pOilField = NULL;
    m_pClientSessionGroup = std::make_shared<CTcpSessionGroup>();
    m_pDeviceSessionGroup = std::make_shared<CTcpSessionGroup>();
    m_pMutexForLstDevice = new QMutex(QMutex::Recursive);
    m_pMutexForLstUser = new QMutex(QMutex::Recursive);
}

CRig::~CRig()
{
    delete m_pMutexForLstDevice;
    delete m_pMutexForLstUser;
}

void CRig::AddUser(user_ptr user)
{
    Q_ASSERT(user);

    QMutexLocker lock(m_pMutexForLstUser);
    m_lstUser.push_back(user);
}

void CRig::AddDevice(device_ptr device)
{
    Q_ASSERT(device);

    QMutexLocker lock(m_pMutexForLstDevice);
    m_lstDevice.push_back(device);
}

void CRig::BroadcastDeviceRealtimeData(message_ptr msg)
{
    Q_ASSERT(msg);

    m_pClientSessionGroup->deliver(msg);
    LOG_DEBUG() << QStringLiteral("广播消息") << msg->ToString();
}

void CRig::SetOilField(oil_field_ptr oilField)
{
    m_pOilField = oilField;
}

const oil_field_ptr CRig::GetOilField()
{
    return m_pOilField;
}

user_ptr CRig::FindUserByID(const QString & userID)
{
    QMutexLocker lock(m_pMutexForLstUser);
    for (user_ptr pUser : m_lstUser) {
        if (pUser && pUser->id.compare(userID) == 0) {
            return pUser;
        }
    }
    return NULL;
}

user_ptr CRig::GetOrCreateUser(const QString & userID)
{
    QMutexLocker lock(m_pMutexForLstUser);
    for (user_ptr pUser : m_lstUser) {
        if (pUser && pUser->id.compare(userID) == 0) {
            return pUser;
        }
    }
    user_ptr pUser = std::make_shared<CUser>();
    pUser->id = userID;
    pUser->SetRig(this->shared_from_this());
    m_lstUser.push_back(pUser);

    return pUser;
}

device_ptr CRig::FindDeviceByID(const QString & deviceID)
{
    QMutexLocker lock(m_pMutexForLstDevice);
    for (device_ptr pDevice : m_lstDevice) {
        if (pDevice && pDevice->id.compare(deviceID) == 0) {
            return pDevice;
        }
    }
    return NULL;
}

device_ptr CRig::GetOrCreateDevice(const QString & deviceID)
{
    QMutexLocker lock(m_pMutexForLstDevice);
    for (device_ptr pDevice : m_lstDevice) {
        if (pDevice && pDevice->id.compare(deviceID) == 0) {
            return pDevice;
        }
    }

    device_ptr pDevice = std::make_shared<CDevice>();
    pDevice->id = deviceID;
    pDevice->SetRig(this->shared_from_this());
    m_lstDevice.push_back(pDevice);

    return pDevice;
}

void CRig::RemoveClientSession(tcp_session_ptr ptr)
{
    QMutexLocker lock(m_pMutexForLstUser);
    m_pClientSessionGroup->leave(ptr);
    ptr->room_ptr_ = NULL;
}

void CRig::AddClientSession(tcp_session_ptr ptr)
{
    QMutexLocker lock(m_pMutexForLstUser);
    m_pClientSessionGroup->join(ptr);
    ptr->room_ptr_ = m_pClientSessionGroup;
}

void CRig::RemoveDeviceSession(tcp_session_ptr ptr)
{
    QMutexLocker lock(m_pMutexForLstDevice);
    if (ptr) {
        m_pDeviceSessionGroup->leave(ptr);
        ptr->room_ptr_ = NULL;
    }
}

void CRig::AddDeviceSession(tcp_session_ptr ptr)
{
    QMutexLocker lock(m_pMutexForLstDevice);
    m_pDeviceSessionGroup->join(ptr);
    ptr->room_ptr_ = m_pDeviceSessionGroup;
}

QString CRig::GetDisplayText()
{
    return name;
}

QString CRig::GetOnlineUsersDisplayText()
{
    QMutexLocker lock(m_pMutexForLstUser);
    QString qstrText;
    for (int i = 0; i < m_lstUser.size(); i++) {
        if (!m_lstUser.at(i)->GetSession()) {
            continue;// 用户不在线
        }
        if (i != 0) {
            qstrText.append(", ");
        }
        qstrText.append(m_lstUser.at(i)->GetDisplayText());
    }
    return qstrText;
}

QString CRig::GetDevicesOnlineStatusDisplayText()
{
    QMutexLocker lock(m_pMutexForLstDevice);
    QString qstrOnlineDevices;
    QString qstrOfflineDevices;
    int nOnlineDevices = 0;
    int nOfflineDevices = 0;

    for (int i = 0; i < m_lstDevice.size(); i++) {
        if (!m_lstDevice.at(i)->GetSession()) {// 设备不在线
            if (i != 0) {
                qstrOfflineDevices.append(", ");
            }
            qstrOfflineDevices.append(m_lstDevice.at(i)->GetDisplayText());
            nOfflineDevices++;
        }
        else
        {
            if (i != 0) {
                qstrOnlineDevices.append(", ");
            }
            qstrOnlineDevices.append(m_lstDevice.at(i)->GetDisplayText());
            nOnlineDevices++;
        }
    }
    return QStringLiteral("井队[%1]，在线设备[%2]台{%3}，离线设备[%4]台{%5}")
        .arg(GetDisplayText())
        .arg(nOnlineDevices).arg(qstrOnlineDevices)
        .arg(nOfflineDevices).arg(qstrOfflineDevices);
}


int CRig::GetDeviceCount()
{
    QMutexLocker lock(m_pMutexForLstDevice);
    return m_lstDevice.size();
}

device_ptr CRig::GetDevice(int index)
{
    QMutexLocker lock(m_pMutexForLstDevice);
    if (index >= 0 && index < m_lstDevice.size()) {
        return m_lstDevice.at(index);
    }
    return NULL;
}

int CRig::GetUserCount()
{
    QMutexLocker lock(m_pMutexForLstUser);
    return m_lstUser.size();
}

user_ptr CRig::GetUser(int index)
{
    QMutexLocker lock(m_pMutexForLstUser);
    if (index >= 0 && index < m_lstUser.size()) {
        return m_lstUser.at(index);
    }
    return NULL;
}


//******************************************************
//** 函数名:   RemoveEverySession
//** 功能简述: 移除所有的连接信息
//** 输入参数: 
//
//** 返回值: 
//   void:
//
//** 创建日期：2015/12/28
//** 创建人：  唐桥
//** 修改记录：
//   日期        版本      修改人    修改内容
//
//** 其它说明：
//******************************************************
void CRig::RemoveEverySession()
{
    // 移除设备会话
    {
        QMutexLocker lock(m_pMutexForLstUser);
        //m_pClientSessionGroup->leave(ptr);
        //ptr->room_ptr_ = NULL;
    }

    // 移除用户会话
    {
        QMutexLocker lock(m_pMutexForLstUser);
        //m_pClientSessionGroup->leave(ptr);
        //ptr->room_ptr_ = NULL;
    }
}

