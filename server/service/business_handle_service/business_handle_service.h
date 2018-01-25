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
    // socket��Ϣ�ص�����

    // ����PC�ͻ��˵�¼����
    int HandlePCClientLogin(tcp_session_ptr session, message_ptr sessionMessageIn, message_ptr responseMessageOut);

    // ����PC�ͻ��˻�ȡ�豸�б�
    int HandlePCClientGetDeviceList(tcp_session_ptr session, message_ptr sessionMessageIn, message_ptr responseMessageOut);

    // ����PC�ͻ��˻�ȡ�豸����
    int HandlePCClientGetDeviceInfo(tcp_session_ptr session, message_ptr sessionMessageIn, message_ptr responseMessageOut);

    // ����PC�ͻ����˳�
    int HandlePCClientExit(tcp_session_ptr session, message_ptr sessionMessageIn);

    // ����PC�ͻ��������豸����
    int HandlePCClientSetDeviceParam(tcp_session_ptr session, message_ptr sessionMessageIn, message_ptr responseMessageOut);

    // ����PC�ͻ��˻�ȡ�豸����״̬
    int HandlePCClientGetDeviceOnlineStatus(tcp_session_ptr session, message_ptr sessionMessageIn, message_ptr responseMessageOut);

    // ����PC�ͻ��˷�������
    int HandlePCClientSendHeartbeat(tcp_session_ptr session, message_ptr sessionMessageIn, message_ptr responseMessageOut);

    // ����PC�ͻ��˻�ȡ��ʷ�豸��������
    int HandlePCClientGetHistoryDeviceDataCount(tcp_session_ptr session, message_ptr sessionMessageIn, message_ptr responseMessageOut);

    // ����PC�ͻ��˻�ȡ��ʷ�豸����
    int HandlePCClientGetHistoryDeviceData(tcp_session_ptr session, message_ptr sessionMessageIn, message_ptr responseMessageOut);

    // ������λ����¼����
    int HandleDeviceLogin(tcp_session_ptr session, message_ptr sessionMessageIn, message_ptr responseMessageOut);

    // ������λ���ϴ�ʵʱ����
    int HandleDeviceRealTimeData(tcp_session_ptr session, message_ptr sessionMessageIn);

    // ������λ���˳�
    int HandleDeviceExit(tcp_session_ptr session, message_ptr sessionMessageIn);

    // ������λ���������������ط�����ʱ��
    int HandleDeviceSendHeartbeat(tcp_session_ptr session, message_ptr sessionMessageIn, message_ptr responseMessageOut);

    // ������λ�������ϴ�ʵʱ����
    int HandleDeviceRealTimeBatchedData(tcp_session_ptr session, message_ptr sessionMessageIn, message_ptr responseMessageOut);

    //////////////////////////////////////////////////////////////////////////
    // socket���ӶϿ� 
    void HandleTcpSessionClosed(tcp_session_ptr session);

    // �ͻ������ӶϿ�
    void HandlePCClientSessionClosed(tcp_session_ptr session);

    // �ɼ��豸���ӶϿ�
    void HandleDeviceSessionClosed(tcp_session_ptr session);

	// ��Э�鴦����
	int HandleDeviceUploadData(tcp_session_ptr session, message_ptr sessionMessageIn);
private:

    CBusinessHandleService();

    ~CBusinessHandleService();

private:
    static CBusinessHandleService * m_pInstance;

	

};



#endif//!_BUSINESS_HANDLE_SERVICE_H_