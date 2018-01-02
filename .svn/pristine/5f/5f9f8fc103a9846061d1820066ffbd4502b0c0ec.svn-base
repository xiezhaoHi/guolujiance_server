#ifndef _I_TCP_SESSION_MESSAGE_HANDLER_h_
#define _I_TCP_SESSION_MESSAGE_HANDLER_h_

#include "message/message.h"

#define ERR_NONE                0x00 /*执行成功*/
#define ERR_UNMATCHED_CMD       0x01 /*命令码不匹配*/
#define ERR_UNMATCHED_ACCOUNT   0x02 /*用户名、密码不匹配*/
#define ERR_INVALID_ACCOUNT     0x03 /*密码错误、账号权限不够*/
#define ERR_DB_ERROR            0x04 /*数据库信息错误*/
#define ERR_INVALID_SESSION     0x05 /*未成功经过登录验证的会话*/
#define ERR_INVALID_ID          0x06 /*无效的ID*/
#define ERR_INVALID_LICENSE     0x07 /*无效的注册码,服务器未取得正版授权*/
#define ERR_INVALID_PERIOD      0x08 /*无效的时间段*/
#define ERR_INVALID_PARAM       0x09 /*无效的参数*/

class CTcpSession;
typedef std::shared_ptr<CTcpSession> tcp_session_ptr;

class ITcpSessionMessageHandler
{
public:
    ITcpSessionMessageHandler() {m_func = NULL; m_funcV2 = NULL;}

    virtual ~ITcpSessionMessageHandler() {}

    // 回调函数定义1：处理后无回复消息
    typedef int (ITcpSessionMessageHandler:: *HandleSessionMessage)(tcp_session_ptr session, message_ptr sessionMessageIn);

    // 回调函数定义2:处理后有回复消息
    typedef int (ITcpSessionMessageHandler:: *HandleSessionMessageV2)(tcp_session_ptr session, message_ptr sessionMessageIn,
        message_ptr responseMessageOut);

    HandleSessionMessage    m_func;

    HandleSessionMessageV2  m_funcV2;
};

#endif//!_I_TCP_SESSION_MESSAGE_HANDLER_h_