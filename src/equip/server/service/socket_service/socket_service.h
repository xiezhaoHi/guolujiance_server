/*****************************************************
** �ļ�����  socket_service.h
** ��  Ȩ��  XXX ��Ȩ����
** ��  ����  
** ���ݼ�����
** �������ڣ�2015/07/15
** �����ˣ�  ����
** �޸ļ�¼��
   ����        �汾      �޸���    �޸����� 
    
*****************************************************/
#ifndef _SOCKET_SERVICE_H_
#define _SOCKET_SERVICE_H_
#include "service/i_service.h"
#include "tcp_server.h"
#include <boost/asio.hpp>
#include <QRunnable>
#include <memory>

using namespace boost::asio;

// ͳһ�����ṩ����ͨ�Ź���
class CSocketService : public IInternalService
{
public:
    // 
    static CSocketService * GetInstance();

    // 
    static void DestroyInstance();

    // ��������
    virtual int StartInternalService();

    // ֹͣ����
    virtual int StopInternalService();

    virtual int NotifyServiceToStop();

    virtual QString GetServiceName();

private:

    CSocketService();

    ~CSocketService();


private:

    static CSocketService     * m_pInstance;
    io_service                  m_io_service;
    std::shared_ptr<CTcpServer> m_pTcpServer;

    friend class CSocketServiceTask;
};

typedef std::shared_ptr<CSocketService> socket_service_ptr;


// ר�ż���TCP����������߳�
class CSocketServiceTask : public QRunnable
{
    virtual void run();
};


// �豸TCP���ӿ���ʱ�����߳�
class CDevConnKeepLiveTask : public QRunnable
{
    virtual void run();
};


#endif//!_SOCKET_SERVICE_H_