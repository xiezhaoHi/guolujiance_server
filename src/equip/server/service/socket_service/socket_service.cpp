#include "socket_service.h"
#include "tcp_server.h"
#include "service/configuration_service/configuration_service.h"
#include "service/socket_service/tcp_session_message_handler_hub.h"
#include "service/task_service/task_service.h"
#include "message/message_body_factory.h"
#include "registration/registration.h"
#include "service/business_handle_service/business_model/business_model.h"
#include "service/business_handle_service/business_model/device.h"
#include <boost/exception/errinfo_errno.hpp>
#include <boost/exception/all.hpp>
#include <QDateTime>
#include <QThread>



CSocketService * CSocketService::m_pInstance = NULL;

CSocketService::CSocketService()
{
    m_pTcpServer = NULL;
}

CSocketService::~CSocketService()
{
    if (m_pTcpServer) {
        m_pTcpServer = NULL;
    }
}

CSocketService * CSocketService::GetInstance()
{
    if (!m_pInstance) {
        m_pInstance = new CSocketService();
    }
    return m_pInstance;
}

void CSocketService::DestroyInstance()
{
    if (m_pInstance) {
        delete m_pInstance;
        m_pInstance = NULL;
    }
}

int CSocketService::StartInternalService()
{
    // 软件授权验证
    IS_REGISTED(-1);

    CMessageBodyFactory::GetInstance();
    CTcpSessionMessageHandlerHub::GetInstance()->NotifyExit(false);
    CTaskService::GetInstance()->CommitTask(new CSocketServiceTask());
    CTaskService::GetInstance()->CommitTask(new CDevConnKeepLiveTask());
    for (int i = 0; i < 5; i++) {
        CTaskService::GetInstance()->CommitTask(new CSessionMessageHandleTask());
    }

    return 0;
}

int CSocketService::StopInternalService()
{
    NotifyServiceToStop();
    m_io_service.stop();
    CTcpSessionMessageHandlerHub::GetInstance()->NotifyExit(true);
    CMessageBodyFactory::DestroyInstance();
    m_pTcpServer = NULL;
    return 0;
}

int CSocketService::NotifyServiceToStop()
{
    IInternalService::NotifyServiceToStop();
    return 0;
}

QString CSocketService::GetServiceName()
{
    return QStringLiteral("socket service");
}

typedef boost::error_info<struct tag_err_no, int> err_no;
typedef boost::error_info<struct tag_err_str, std::string> err_str;


// 启动线程，监听端口
void CSocketServiceTask::run()
{
    // 软件授权验证
    IS_REGISTED(;);

    QString qstrFormat(QStringLiteral("%1##%2"));
    QString taskID = qstrFormat.arg(QDateTime::currentDateTime().toString("dd#HH_mm_ss__zzz")).arg(qrand());
    LOG_DEBUG() << "SOCKET SERVICE THREAD " << taskID << "START";
    unsigned short port = CConfigrationService::GetInstance()->GetPort();
    ip::tcp::endpoint endpoint_(ip::tcp::v4(), port);

    while (!CSocketService::GetInstance()->IsStopSingalTriggered())// 加上while循环，不让任一连接的错误影响到其它连接的数据收发 tangqiao 2015/12/29
    {
        try
        {
            if (!CSocketService::GetInstance()->m_pTcpServer) {
                CSocketService::GetInstance()->m_pTcpServer = std::make_shared<CTcpServer>(CSocketService::GetInstance()->m_io_service, endpoint_);
            }
            CSocketService::GetInstance()->m_io_service.run();
        }
        catch (boost::system::system_error & e)
        {
            LOG_WARING() << QStringLiteral("SYSTEM ERROR, val[%1], info[%2]")
                .arg(e.code().value()).arg(QString::fromLocal8Bit(e.what()));
        }
    }

    LOG_DEBUG() << "SOCKET SERVICE THREAD " << taskID << "EXIT";
}


// 启动线程，检查设备连接空闲时间是否达到最大值
void CDevConnKeepLiveTask::run()
{
    LOG_DEBUG() << QStringLiteral("CDevConnKeepLiveTask start");

    // 间隔检查设备连接超时
    while (!CSocketService::GetInstance()->IsStopSingalTriggered())
    {
        QThread::currentThread()->msleep(1000);// 间隔1s
        if (CSocketService::GetInstance()->IsStopSingalTriggered()) {
            break;
        }

        quint64 current = QDateTime::currentMSecsSinceEpoch();

        QList<rig_ptr> lst;
        CBusinessModel::GetInstance()->GetRigList(lst);

        for (rig_ptr pRig : lst) {
            int count = pRig->GetDeviceCount();
            for (int i = 0; i < count; i++) {
                device_ptr pDevice = pRig->GetDevice(i);
                tcp_session_ptr pSession = pDevice->GetSession();
                if (!pSession) {
                    continue;
                }

                quint64 elapsed = current - pSession->GetLatestRecvTime();
                quint64 sec = elapsed / 1000;
                int maxIdleTime = CConfigrationService::GetInstance()->GetDevConnMaxIdleTime();
                if (sec >= maxIdleTime) {
                    pRig->RemoveDeviceSession(pSession);
                    pDevice->SetSession(NULL);
                    pSession->set_entity(NULL);
                    LOG_WARING() << QStringLiteral("井队[%1]的采集设备[%2]空闲时间已超过服务器允许的最大空闲连接时间[%3],主动关闭连接[%4]")
                        .arg(pRig->GetDisplayText()).arg(pDevice->GetDisplayText())
                        .arg(maxIdleTime).arg(pSession->get_display_text());
                    pSession->stop();
                    LOG_WARING() << pRig->GetDevicesOnlineStatusDisplayText();
                }
            }
        }
    }

    LOG_DEBUG() << QStringLiteral("CDevConnKeepLiveTask exit");
}
