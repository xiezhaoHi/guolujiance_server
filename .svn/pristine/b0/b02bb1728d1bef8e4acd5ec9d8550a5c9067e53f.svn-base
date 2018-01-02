#ifndef _DBG_LOGGER_H_
#define _DBG_LOGGER_H_

#include <QString>
#include <QFile>
#include <QMutex>
#include "service/log_service/log_service.h"

class CDbgLogger
{
public:

    static CDbgLogger * GetInstance();

    static void DestroyInstance();

    static void LogMessageHandler(QtMsgType type, 
        const QMessageLogContext &context, const QString &msg);

    static void LogMessageHandler(E_LOG_LEVEL type,
        const char * file,
        const int line,
        const QString &msg);

    // 及时检查日志文件是否达到了指定大小，如需切换文件，则创建新文件
    bool CheckOrCreateLogFile();

    // 关闭日志文件
    void CloseLogFile();

private:

    CDbgLogger();

    ~CDbgLogger();

    bool WriteLog(QString & log);

private:
    static CDbgLogger * m_pInstance;
    QString         m_logFileDir;
    QFile           m_logFile;
    QMutex        * m_pMutexForHandler;
    bool            m_bLogFileOpen;
    bool            m_bLogFileLine;
    unsigned int    m_nCheckInterval;
    unsigned int    m_nCheckCount;
    qint64          m_nMaxSizeOfLogFile;
};


#endif