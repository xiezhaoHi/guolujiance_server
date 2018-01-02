#include "message_body_factory.h"
#include "message_header.h"
#include "protocol/protocol_cmd.h"
#include "message/message_login.h"
#include "message/message_login_rep.h"
#include "message/message_get_device_list_rep.h"
#include "message/message_get_devices_info_rep.h"
#include "message/message_device_realtime_data.h"
#include "message/message_broadcast_device_realtime_data.h"
#include "message/message_get_devices_info_req.h"
#include "message/message_unrecognized_body_type.h"
#include "message/message_get_device_online_status.h"
#include "message/message_get_device_online_status_rep.h"
#include "message/message_broadcast_device_online_status.h"
#include "message/message_set_device_param.h"
#include "message/message_heartbeat.h"
#include "message/message_server_time_beijing.h"
#include "message/message_get_device_data_count_req.h"
#include "message/message_get_device_data_count_rep.h"
#include "message/message_get_device_data_req.h"
#include "message/message_get_device_data_rep.h"
#include "message/message_device_realtime_batched_data.h"
#include <QDebug>

CMessageBodyFactory * CMessageBodyFactory::m_pInstance = NULL;


CMessageBodyFactory::CMessageBodyFactory()
{
    static_assert(sizeof(U8) == 1, "sizeof(U8) != 1");
    static_assert(sizeof(S8) == 1, "sizeof(S8) != 1");
    static_assert(sizeof(U16) == 2, "sizeof(U16) != 2");
    static_assert(sizeof(S16) == 2, "sizeof(S16) != 2");
    static_assert(sizeof(U32) == 4, "sizeof(U32) != 4");
    static_assert(sizeof(S32) == 4, "sizeof(S32) != 4");
    static_assert(sizeof(U64) == 8, "sizeof(U64) != 8");
    static_assert(sizeof(S64) == 8, "sizeof(S64) != 8");
    static_assert(sizeof(UL32) == 4, "sizeof(UL32) != 4");
    static_assert(sizeof(F32) == 4, "sizeof(F32) != 4");
    static_assert(sizeof(D64) == 8, "sizeof(D64) != 8");

    RegisterMessageBodyCreators();
}

CMessageBodyFactory::~CMessageBodyFactory()
{

}

CMessageBodyFactory * CMessageBodyFactory::GetInstance()
{
    if (!m_pInstance) {
        m_pInstance = new CMessageBodyFactory();
    }
    return m_pInstance;
}

void CMessageBodyFactory::DestroyInstance()
{
    if (m_pInstance) {
        delete m_pInstance;
        m_pInstance = NULL;
    }
}

IMessage * CMessageBodyFactory::Product(const CMessageHeader & header)
{
    IMessage * ret = NULL;

    U32 cmd = header.m_struct.cmd;
    U32 cmdRet = header.m_struct.cmdResult;
    if (IS_FROM_PC_SERVER(cmd) && cmdRet != 0 && header.m_struct.bodyLength > 0) {
        return CreateMessageErrorInfo(header);
    }

    QHash<U32,CreateBodyFromHeader>::const_iterator it = m_hash.find(cmd);
    if (it != m_hash.end()) {
        CreateBodyFromHeader creator = it.value();
        if (!creator) {
            creator = &CMessageUnrecognizedBodyType::CreateFromHeader;
        }
        ret = (*creator)(header);
    } else {
        // 
        //DLOG(ERROR) << "cmd:" << cmd << " have no creator.";
        _ASSERT(false);

    }

    return ret;
}

void CMessageBodyFactory::RegisterMessageBodyCreators()
{
    // 注册PC客户端请求命令对应的消息体生成函数
    RegisterMessageBodyCreator(PC_CLIENT_CMD_LOGIN, CMessageLogin::CreateFromHeader, "PC_CLIENT_CMD_LOGIN", "");
    RegisterMessageBodyCreator(PC_CLIENT_CMD_GET_DEVICE_LIST, NULL, "PC_CLIENT_CMD_GET_DEVICE_LIST", "");
    RegisterMessageBodyCreator(PC_CLIENT_CMD_GET_DEVICE_INFO, CMessageGetDevicesInfoReq::CreateFromHeader, "PC_CLIENT_CMD_GET_DEVICE_INFO", "");
    RegisterMessageBodyCreator(PC_CLIENT_CMD_EXIT, NULL, "PC_CLIENT_CMD_EXIT", "");
    RegisterMessageBodyCreator(PC_CLIENT_CMD_GET_DEVICE_STATUS, CMessageGetDeviceOnlineStatus::CreateFromHeader, "PC_CLIENT_CMD_GET_DEVICE_STATUS", "");
    RegisterMessageBodyCreator(PC_CLIENT_CMD_SET_DEVICE_PARAM, CMessageSetDeviceParam::CreateFromHeader, "PC_CLIENT_CMD_SET_DEVICE_PARAM", "");
    RegisterMessageBodyCreator(PC_CLIENT_CMD_SEND_HEARTBEAT, NULL, "PC_CLIENT_CMD_SEND_HEARTBEAT", "");
    RegisterMessageBodyCreator(PC_CLIENT_CMD_GET_HISTORY_DEVICE_DATA_COUNT, CMessageGetDeviceDataCountReq::CreateFromHeader, "PC_CLIENT_CMD_SEND_HEARTBEAT", "");
    RegisterMessageBodyCreator(PC_CLIENT_CMD_GET_HISTORY_DEVICE_DATA, CMessageGetDeviceDataReq::CreateFromHeader, "PC_CLIENT_CMD_GET_HISTORY_DEVICE_DATA", "");

    // 注册采集设备请求命令对应的消息体生成函数
    RegisterMessageBodyCreator(DEVICE_CMD_LOGIN, CMessageLogin::CreateFromHeader, "DEVICE_CMD_LOGIN", "");
    RegisterMessageBodyCreator(DEVICE_CMD_REALTIME_DATA, CMessageDeviceRealtimeData::CreateFromHeader, "DEVICE_CMD_REALTIME_DATA", "");
    RegisterMessageBodyCreator(DEVICE_CMD_EXIT, NULL, "DEVICE_CMD_EXIT", "");
    RegisterMessageBodyCreator(DEVICE_CMD_SEND_HEARTBEAT, NULL, "DEVICE_CMD_SEND_HEARTBEAT", "");
    RegisterMessageBodyCreator(DEVICE_CMD_REALTIME_BATCHED_DATA, CMessageDeviceRealtimeBatchedData::CreateFromHeader, "DEVICE_CMD_REALTIME_BATCHED_DATA", "");

    // 注册PC服务端命令对应的消息体生成函数
    RegisterMessageBodyCreator(PC_SERVER_CMD_LOGIN_REP, CMessageLoginRep::CreateFromHeader, "PC_SERVER_CMD_LOGIN_REP", "");
    RegisterMessageBodyCreator(PC_SERVER_CMD_GET_DEVICE_LIST_REP, CMessageGetDeviceListRep::CreateFromHeader, "PC_SERVER_CMD_GET_DEVICE_LIST_REP", "");
    RegisterMessageBodyCreator(PC_SERVER_CMD_GET_DEVICES_INFO_REP, CMessageGetDevicesInfoRep::CreateFromHeader, "PC_SERVER_CMD_GET_DEVICES_INFO_REP", "");
    RegisterMessageBodyCreator(PC_SERVER_CMD_GET_DEVICE_STATUS_REP, CMessageGetDeviceOnlineStatusRep::CreateFromHeader, "PC_SERVER_CMD_BROADCAST_REALTIME_DATA", "");
    RegisterMessageBodyCreator(PC_SERVER_CMD_SET_DEVICE_PARAM_REP, NULL, "PC_SERVER_CMD_SET_DEVICE_PARAM_REP", "");
    RegisterMessageBodyCreator(PC_SERVER_CMD_SEND_HEARTBEAT_REP, CMessageHeartbeat::CreateFromHeader, "PC_SERVER_CMD_SEND_HEARTBEAT_REP", "");
    RegisterMessageBodyCreator(PC_SERVER_CMD_GET_HISTORY_DEVICE_DATA_COUNT_REP, CMessageGetDeviceDataCountRep::CreateFromHeader, "PC_SERVER_CMD_SEND_HEARTBEAT_REP", "");
    RegisterMessageBodyCreator(PC_SERVER_CMD_GET_HISTORY_DEVICE_DATA_REP, CMessageGetDeviceDataRep::CreateFromHeader, "PC_SERVER_CMD_SEND_HEARTBEAT_REP", "");
    // 服务器心跳返回的结构不同，为CMessageServerTimeBeiJing tangqiao 2015/12/30
    //RegisterMessageBodyCreator(PC_SERVER_CMD_SEND_HEARTBEAT_REP, CMessageServerTimeBeiJing::CreateFromHeader, "PC_SERVER_CMD_SEND_HEARTBEAT_REP", "");
    RegisterMessageBodyCreator(PC_SERVER_CMD_BROADCAST_REALTIME_DATA, CMessageBroadcastDeviceRealtimeData::CreateFromHeader, "PC_SERVER_CMD_BROADCAST_REALTIME_DATA", "");
    RegisterMessageBodyCreator(PC_SERVER_CMD_BROADCAST_DEVICE_STATUS, CMessageBroadcastDeviceOnlineStatus::CreateFromHeader, "PC_SERVER_CMD_BROADCAST_DEVICE_STATUS", "");
}

void CMessageBodyFactory::RegisterMessageBodyCreator(U32 cmd,
                                                     CreateBodyFromHeader creator,
                                                     const char *strCmd, 
                                                     const char *strCreator)
{
    qDebug() << "RegisterMessageBodyCreator ##cmdName:" << strCmd << " ##value:" << cmd 
        << " ##creator:" << strCreator;

    m_hash.insert(cmd, creator);
}

CMessageErrorInfo * CMessageBodyFactory::CreateMessageErrorInfo(const CMessageHeader & header)
{
    if (header.m_struct.cmdResult != 0) {
        return new CMessageErrorInfo();
    }
    return NULL;
}

