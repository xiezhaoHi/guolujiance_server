#ifndef _LOGGING_H_
#define _LOGGING_H_
#include <QString>


#define LOG_SOURCE_SYSTEM QStringLiteral("System")


typedef enum _E_LOG_LEVEL
{
    LOG_DEBUG = 0,
    LOG_INFO,
    LOG_IMPORTANT_INFO,
    LOG_WARNING,
    LOG_ERROR,
}E_LOG_LEVEL;


// TODO 日志接口
class CLogging
{
public:
    static CLogging * GetInstance();

    static void DestroyInstance();

    static void LogDebug(const QString & logSource, const QString & logInfo);

    static void LogInfo(const QString & logSource, const QString & logInfo);

    static void LogImportantInfo(const QString & logSource, const QString & logInfo);

    static void LogWaring(const QString & logSource, const QString & logInfo);

    static void LogError(const QString & logSource, const QString & logInfo);

    void Log(E_LOG_LEVEL logLevel, const QString & logSource, 
        const QString & logInfo);

private:
    CLogging();

    ~CLogging();

    // 写入日志文件
    void WriteToLogFile(QString);

    // 写入日志数据库
    //void WriteToLogDB();

private:
    static CLogging * m_pInstance;
	QString m_fileName;
};



#endif//!_LOGGING_H_