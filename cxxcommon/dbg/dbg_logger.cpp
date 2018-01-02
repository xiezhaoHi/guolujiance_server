#include "dbg_logger.h"
#include "state_collector/path_collector.h"
#include "service/configuration_service/configuration_service.h"
#include "service/log_service/log_service.h"
#include <QDateTime>
#include <QDir>
#include <QTextStream>

CDbgLogger * CDbgLogger::m_pInstance = NULL;


CDbgLogger::CDbgLogger()
{
    m_nCheckInterval = 100;// 隔100次检查一次文件
    m_nCheckCount = 0;
    m_nMaxSizeOfLogFile = 4 << 20;// 日志文件最多写入4M
    m_pMutexForHandler = new QMutex(QMutex::NonRecursive);
}

CDbgLogger::~CDbgLogger()
{
    delete m_pMutexForHandler;
}

CDbgLogger * CDbgLogger::GetInstance()
{
    if (!m_pInstance) {
        m_pInstance = new CDbgLogger();
        m_pInstance->CheckOrCreateLogFile();
    }
    return m_pInstance;
}

void CDbgLogger::DestroyInstance()
{
    if (m_pInstance) {
        delete m_pInstance;
        m_pInstance = NULL;
    }
}

void CDbgLogger::LogMessageHandler(QtMsgType type, 
                                   const QMessageLogContext &context,
                                   const QString &msg)
{
    // 获取时间
    QString strTime = QDateTime::currentDateTime().toLocalTime()
        .toString(QStringLiteral("yyyy-MM-dd HH:mm:ss.zzz"));
    QString strLevel;
    E_LOG_LEVEL logLevel;
    switch (type)
    {
    case QtDebugMsg:
        logLevel = LEVEL_DEBUG;
        strLevel = QStringLiteral("  DEBUG");
        break;

    case QtWarningMsg:
        logLevel = LEVEL_INFO;
        strLevel = QStringLiteral("   INFO");
        break;

    case QtCriticalMsg:
        logLevel = LEVEL_WARING;
        strLevel = QStringLiteral("WARNING");
        break;

    case QtFatalMsg:
        logLevel = LEVEL_ERROR;
        strLevel = QStringLiteral("  ERROR");
        break;

    default:
        logLevel = LEVEL_NONE;
        strLevel = QStringLiteral("LEVEL");
    }



    QString strLog;
#ifdef TRACE_FILE_LINE
    // 日期时间 日志级别 输出日志的源文件 源文件行号 日志内容
    QString strFormat(QStringLiteral("[%1][%2][%3:%4] %5"));
    strLog = strFormat.arg(strTime).arg(strLevel)
        .arg(context.file).arg(context.line)
        .arg(msg);
#else
    // 日期时间 日志级别 日志内容
    QString strFormat(QStringLiteral("[%1][%2] %3"));
    strLog = strFormat.arg(strTime).arg(strLevel).arg(msg);
#endif // TRACE_FILE_LINE

    QMutexLocker lock(CDbgLogger::GetInstance()->m_pMutexForHandler);
    CDbgLogger::GetInstance()->m_nCheckCount++;
    if (CDbgLogger::GetInstance()->m_nCheckCount >= CDbgLogger::GetInstance()->m_nCheckInterval) {
        CDbgLogger::GetInstance()->CheckOrCreateLogFile();
        CDbgLogger::GetInstance()->m_nCheckCount = 0;
    }

    // 不输出比配置里定义的级别更低的日志
    if (logLevel >= CConfigrationService::GetInstance()->GetLogLevel()) {
        CDbgLogger::GetInstance()->WriteLog(strLog);
    }
    
    CLogService::GetInstance()->HandleLog(logLevel, strLog);
}

void CDbgLogger::LogMessageHandler(E_LOG_LEVEL type, const char * file, 
                                   const int line, const QString &msg)
{
    // 获取时间
    QString strTime = QDateTime::currentDateTime().toLocalTime()
        .toString("yyyy-MM-dd HH:mm:ss.zzz");
    QString strLevel;
    E_LOG_LEVEL logLevel = type;

    switch(type)
    {
    case LEVEL_DEBUG:
        strLevel = QString("  DEBUG");
        break;

    case LEVEL_INFO:
        strLevel = QString("   INFO");
        break;

    case LEVEL_WARING:
        strLevel = QString("WARNING");
        break;

    case LEVEL_ERROR:
        strLevel = QString("  ERROR");
        break;

    default:
        logLevel = LEVEL_NONE;
        strLevel = QString("LEVEL");
    }

    QString strLog;
#ifdef TRACE_FILE_LINE
    // 日期时间 日志级别 输出日志的源文件 源文件行号 日志内容
    QString strFormat(QString("[%1][%2][%3:%4] %5"));
    strLog = strFormat.arg(strTime).arg(strLevel)
        .arg(file).arg(line)
        .arg(msg);
#else
    // 日期时间 日志级别 日志内容
    QString strFormat(QString("[%1][%2] %3"));
    strLog = strFormat.arg(strTime).arg(strLevel).arg(msg);
#endif // TRACE_FILE_LINE

    QMutexLocker lock(CDbgLogger::GetInstance()->m_pMutexForHandler);
    CDbgLogger::GetInstance()->m_nCheckCount++;

    //if(CDbgLogger::GetInstance()->m_nCheckCount >= CDbgLogger::GetInstance()->m_nCheckInterval)
    {
        CDbgLogger::GetInstance()->CheckOrCreateLogFile();
        CDbgLogger::GetInstance()->m_nCheckCount = 0;
    }

    // 不输出比配置里定义的级别更低的日志
    //if(logLevel >= CConfigrationService::GetInstance()->GetLogLevel())
    {
        CDbgLogger::GetInstance()->WriteLog(strLog);
    }

    switch (type)
    {
    case LEVEL_DEBUG:
    case LEVEL_INFO:
    case LEVEL_WARING:
        qDebug() << strLog;
        break;

    case LEVEL_ERROR:
        qDebug() << strLog;
        break;

    default:
        break;
    }
}

bool CDbgLogger::CheckOrCreateLogFile()
{
    bool ret = false;
    // 检查文件大小是否达到切换值
    if (m_logFile.isOpen()) {
        m_logFile.flush();
        qint64 sizeOfFile = m_logFile.size();
        if (sizeOfFile >= m_nMaxSizeOfLogFile) {
            m_logFile.close();// 日志文件大小超过最大值，需要创建新文件
        } else {
            return true;
        }
    } 

    { // 创建并打开新文件
        QString strLogDirName(CPathCollector::m_strAppDirPath + QStringLiteral("/H2SMonitorSvcLog"));
        QString strLogDirNameForToday = strLogDirName + QStringLiteral("/")
            + QDateTime::currentDateTime().toLocalTime()
            .toString(QStringLiteral("yyyy-MM-dd"));
        QString strLogFileName = QDateTime::currentDateTime().toLocalTime()
            .toString(QStringLiteral("HH_mm_ss_zzz")).append(QStringLiteral(".txt"));

        // 判断按天存放的日志目录是否已创建
        QDir logDirForToday(strLogDirNameForToday);
        if (!logDirForToday.exists()) {
            ret = logDirForToday.mkpath(strLogDirNameForToday);
            if (!ret) {
                return false;
            }
        }

        m_logFile.setFileName(strLogDirNameForToday + QStringLiteral("/") + strLogFileName);

        // 已打开新日志文件，在切换文件时关闭当前打开的文件，或者进程退出时关闭
        ret = m_logFile.open(QIODevice::WriteOnly | QIODevice::Append);
        if (!ret) {
            return false;
        }
    }

    return true;
}

void CDbgLogger::CloseLogFile()
{
    if (m_logFile.isOpen()) {
        m_logFile.close();
    }
}

bool CDbgLogger::WriteLog(QString & log)
{

    while (log.contains(QStringLiteral("\" "))) {
        log = log.remove(QStringLiteral("\" "));
    }
    while (log.contains(QStringLiteral("\""))) {
        log = log.remove(QStringLiteral("\""));
    }
    if (m_logFile.isOpen()) {
        QTextStream textstream(&m_logFile);
        textstream << log;
        if (!log.endsWith(QStringLiteral("\n"))) {
            textstream << QStringLiteral("\r\n");
        }
        return true;
    }
    return false;
}
