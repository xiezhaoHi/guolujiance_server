#include "log_service.h"
#include "dbg/dbg_logger.h"
#include "state_collector/path_collector.h"
#include <QMutexLocker>
#include <QDir>


CLogService * CLogService::m_pInstance = NULL;

CLogService::CLogService()
{
    m_pInnerObserver = NULL;
}

CLogService::~CLogService()
{

}

int CLogService::StartInternalService()
{
    CDbgLogger::GetInstance();
    qInstallMessageHandler(CDbgLogger::LogMessageHandler);

    return 0;
}

int CLogService::StopInternalService()
{
    CDbgLogger::GetInstance()->CloseLogFile();
    return 0;
}

int CLogService::NotifyServiceToStop()
{
    return 0;
}

QString CLogService::GetServiceName()
{
    return QStringLiteral("log service");
}

CLogService * CLogService::GetInstance()
{
    if (!m_pInstance) {
        m_pInstance = new CLogService();
    }
    return m_pInstance;
}

void CLogService::DestroyInstance()
{
    if (m_pInstance) {
        delete m_pInstance;
        m_pInstance = NULL;
    }

    CDbgLogger::DestroyInstance();
}

void CLogService::HandleLog(E_LOG_LEVEL level, const QString & log)
{
    QMutexLocker lock(&m_observerLock);
    if (m_pInnerObserver) {
        m_pInnerObserver->HandleLog(level, log);
    }
}

void CLogService::RegisterLogObserver(ILogObserver * observer)
{
    QMutexLocker lock(&m_observerLock);
    m_pInnerObserver = observer;
}

void CLogService::UnregisterLogObserver(ILogObserver * observer)
{
    QMutexLocker lock(&m_observerLock);
    if (m_pInnerObserver == observer)
    {
        m_pInnerObserver = NULL;
    }
}
