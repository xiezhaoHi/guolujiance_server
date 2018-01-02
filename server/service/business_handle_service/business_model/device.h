#ifndef _DEVICE_H_
#define _DEVICE_H_

#include "structs/structs.h"
#include "message/message.h"
#include "service/socket_service/tcp_session.h"
#include <QDateTime>


class CRig;
typedef std::shared_ptr<CRig> rig_ptr;

class CTcpSession;

typedef std::shared_ptr<QDateTime> date_time_ptr;

class CDevice : public CDeviceInfo, public std::enable_shared_from_this<CDevice>
{
public:

    CDevice();

    ~CDevice();

    // 设置所属井队
    void SetRig(rig_ptr rig);

    // 获取所属井队
    rig_ptr GetRig();

    // 设置设备连接会话
    virtual void SetSession(tcp_session_ptr session);

    // 获取设备连接会话对象
    const tcp_session_ptr GetSession();

    // 广播实时数据到本设备所属井队里的每一个在线客户端
    void BroadcastToPCClients(message_ptr msg);

    // 获取设备的显示文本信息
    virtual QString GetDisplayText();

    // 获取设备当前的在线状态
    bool IsDeviceOnline();

    // 设置登录时间
    void SetLoginTime(date_time_ptr ptr);

    // 获取登录时间
    date_time_ptr GetLoginTime();
private:
                                 // 设备信息
    rig_ptr         m_pRig;      // 所属井队
    tcp_session_ptr m_pSession;  // 设备会话
    date_time_ptr   m_pLoginTime;// 登录时间
};


typedef std::shared_ptr<CDevice> device_ptr;

#endif//!_DEVICE_H_
