#ifndef _RIG_H_
#define _RIG_H_

#include <QList>
#include "structs/structs.h"
#include "service/socket_service/i_tcp_session_participant.h"
#include "service/socket_service/tcp_session_group.h"
#include "service/socket_service/tcp_session.h"
#include "message/message.h"
#include <QMutex>


class COilField;
typedef std::shared_ptr<COilField> oil_field_ptr;

class CUser;
typedef std::shared_ptr<CUser> user_ptr;

class CDevice;
typedef std::shared_ptr<CDevice> device_ptr;

class CTcpSession;
class CMessage;


// 井队
class CRig : public CRigInfo, public std::enable_shared_from_this<CRig>
{
    Q_OBJECT

public:

    CRig();

    ~CRig();

    // 设置所属油田
    void SetOilField(oil_field_ptr oilField);

    // 获取所属油田
    const oil_field_ptr GetOilField();

    // 获取用户对象
    user_ptr FindUserByID(const QString & userID);

    // 
    user_ptr GetOrCreateUser(const QString & userID);

    // 添加人员
    void AddUser(user_ptr user);

    // 获取设备对象
    device_ptr FindDeviceByID(const QString & deviceID);

    // 
    device_ptr GetOrCreateDevice(const QString & deviceID);

    // 添加设备
    void AddDevice(device_ptr device);

    // 广播设备实时采集数据
    void BroadcastDeviceRealtimeData(message_ptr msg);

    // 移除客户端会话
    void RemoveClientSession(tcp_session_ptr ptr);

    // 添加客户端会话
    void AddClientSession(tcp_session_ptr ptr);

    // 移除设备会话
    void RemoveDeviceSession(tcp_session_ptr ptr);

    // 添加设备会话
    void AddDeviceSession(tcp_session_ptr ptr);

    // 移除所有连接会话
    void RemoveEverySession();

    // 获取显示文本
    virtual QString GetDisplayText();

    // 获取所有在线用户的显示文本
    QString GetOnlineUsersDisplayText();

    // 获取所有在线设备的显示文本
    QString GetDevicesOnlineStatusDisplayText();

    // 获取设备数
    int GetDeviceCount();

    // 获取指定索引的设备
    device_ptr GetDevice(int index);

    // 获取在线用户数
    int GetUserCount();

    // 获取指定索引的用户
    user_ptr GetUser(int index);

signals:

    void rigUserUpdated();

    void rigDeviceUpdated();

private:
                                             // 井队信息
    oil_field_ptr       m_pOilField;         // 所属油田
    QList<device_ptr>   m_lstDevice;         // 井队设备
    QMutex            * m_pMutexForLstDevice;
    QList<user_ptr>     m_lstUser;           // 井队人员
    QMutex            * m_pMutexForLstUser;  

    tcp_session_group_ptr   m_pClientSessionGroup;// 客户端会话
    tcp_session_group_ptr   m_pDeviceSessionGroup;// 采集设备会话
};


typedef std::shared_ptr<CRig> rig_ptr;


#endif//!_RIG_H