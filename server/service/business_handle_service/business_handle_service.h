#ifndef _BUSINESS_HANDLE_SERVICE_H_
#define _BUSINESS_HANDLE_SERVICE_H_

#include "service/i_service.h"
#include "service/socket_service/tcp_session.h"
#include "service/socket_service/i_tcp_session_message_handler.h"
#include <QMap>
class CBusinessHandleService : public IInternalService, public ITcpSessionMessageHandler
{
public:

    static CBusinessHandleService * GetInstance();

    static void DestroyInstance();

    virtual int StartInternalService();

    virtual int StopInternalService();

    virtual int NotifyServiceToStop();

    virtual QString GetServiceName();

    //////////////////////////////////////////////////////////////////////////
    // socket消息回调函数

    // 处理PC客户端登录命令
    int HandlePCClientLogin(tcp_session_ptr session, message_ptr sessionMessageIn, message_ptr responseMessageOut);

    // 处理PC客户端获取设备列表
    int HandlePCClientGetDeviceList(tcp_session_ptr session, message_ptr sessionMessageIn, message_ptr responseMessageOut);

    // 处理PC客户端获取设备详情
    int HandlePCClientGetDeviceInfo(tcp_session_ptr session, message_ptr sessionMessageIn, message_ptr responseMessageOut);

    // 处理PC客户端退出
    int HandlePCClientExit(tcp_session_ptr session, message_ptr sessionMessageIn);

    // 处理PC客户端设置设备参数
    int HandlePCClientSetDeviceParam(tcp_session_ptr session, message_ptr sessionMessageIn, message_ptr responseMessageOut);

    // 处理PC客户端获取设备在线状态
    int HandlePCClientGetDeviceOnlineStatus(tcp_session_ptr session, message_ptr sessionMessageIn, message_ptr responseMessageOut);

    // 处理PC客户端发送心跳
    int HandlePCClientSendHeartbeat(tcp_session_ptr session, message_ptr sessionMessageIn, message_ptr responseMessageOut);

    // 处理PC客户端获取历史设备数据条数
    int HandlePCClientGetHistoryDeviceDataCount(tcp_session_ptr session, message_ptr sessionMessageIn, message_ptr responseMessageOut);

    // 处理PC客户端获取历史设备数据
    int HandlePCClientGetHistoryDeviceData(tcp_session_ptr session, message_ptr sessionMessageIn, message_ptr responseMessageOut);

    // 处理下位机登录命令
    int HandleDeviceLogin(tcp_session_ptr session, message_ptr sessionMessageIn, message_ptr responseMessageOut);

    // 处理下位机上传实时数据
    int HandleDeviceRealTimeData(tcp_session_ptr session, message_ptr sessionMessageIn);

    // 处理下位机退出
    int HandleDeviceExit(tcp_session_ptr session, message_ptr sessionMessageIn);

    // 处理下位机发送心跳，返回服务器时间
    int HandleDeviceSendHeartbeat(tcp_session_ptr session, message_ptr sessionMessageIn, message_ptr responseMessageOut);

    // 处理下位机批量上传实时数据
    int HandleDeviceRealTimeBatchedData(tcp_session_ptr session, message_ptr sessionMessageIn, message_ptr responseMessageOut);

    //////////////////////////////////////////////////////////////////////////
    // socket连接断开 
    void HandleTcpSessionClosed(tcp_session_ptr session);

    // 客户端连接断开
    void HandlePCClientSessionClosed(tcp_session_ptr session);

    // 采集设备连接断开
    void HandleDeviceSessionClosed(tcp_session_ptr session);

	// 新协议处理函数
	int HandleDeviceUploadData(tcp_session_ptr session, message_ptr sessionMessageIn);
private:

    CBusinessHandleService();

    ~CBusinessHandleService();

private:
    static CBusinessHandleService * m_pInstance;

	

};



#endif//!_BUSINESS_HANDLE_SERVICE_H_