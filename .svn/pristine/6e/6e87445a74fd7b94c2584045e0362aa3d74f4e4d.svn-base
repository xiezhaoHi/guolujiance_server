#ifndef _USER_H_
#define _USER_H_

#include "structs/structs.h"
#include "service/socket_service/tcp_session.h"
#include <QDateTime>

class CRig;
typedef std::shared_ptr<CRig> rig_ptr;

class CTcpSession;

typedef std::shared_ptr<QDateTime> date_time_ptr;

// 用户
class CUser : public CUserInfo, public std::enable_shared_from_this<CUser>
{
public:

    CUser();

    ~CUser();

    // 设置所属井队
    void SetRig(rig_ptr rig);

    // 返回所属井队
    rig_ptr GetRig();

    // 设置会话
    virtual void SetSession(tcp_session_ptr session);

    // 获取客户端会话
    const tcp_session_ptr GetSession();

    // 显示用户登录名
    virtual QString GetDisplayText();

    // 设置用户登录时间 
    void SetLoginTime(date_time_ptr ptr);

    // 获取用户登录时间
    date_time_ptr GetLoginTime();

private:
                               // 用户信息
    rig_ptr         m_pRig;    // 所属油田
    tcp_session_ptr m_pSession;// 客户端连接会话
    date_time_ptr   m_pLoginTime;
};


typedef std::shared_ptr<CUser> user_ptr;

#endif//!_USER_H_