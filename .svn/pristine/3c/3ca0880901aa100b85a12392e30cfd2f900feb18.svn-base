#include "logging.h"
#include <QDebug>
#include <QDateTime>
#include <QFile>
#include <QTextStream>

CLogging * CLogging::m_pInstance = NULL;

CLogging::CLogging()
{
	m_fileName = "./log/";
	m_fileName += QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss");
	m_fileName += ".txt";
	QFile file( m_fileName );
    // 客户端程序没必要记录日志 tangqiao 2016/01/19
	//if( !file.open(QIODevice::ReadWrite|QIODevice::Truncate) );
	//{
	//	qDebug() << "log file build false";
	//}
	//if( file.isOpen() )
	//	file.close();
}

CLogging::~CLogging()
{

}

CLogging * CLogging::GetInstance()
{
    if (NULL == m_pInstance) {
        m_pInstance = new CLogging();
    }
    return m_pInstance;
}

void CLogging::DestroyInstance()
{
    if (m_pInstance) {
        delete m_pInstance;
        m_pInstance = NULL;
    }
}

void CLogging::Log(E_LOG_LEVEL logLevel, const QString & logSource, 
                   const QString & logInfo)
{
    return;// 客户端没有必要记录日志 tangqiao 2016/01/19

    QString qstrLevel;
    switch (logLevel)
    {
    case LOG_DEBUG:
        qstrLevel = QStringLiteral("调试");
        break;

    case LOG_INFO:
        qstrLevel = QStringLiteral("信息");
        break;

    case LOG_IMPORTANT_INFO:
        qstrLevel = QStringLiteral("重要");
        break;

    case LOG_WARNING:
        qstrLevel = QStringLiteral("警告");
        break;

    case LOG_ERROR:
        qstrLevel = QStringLiteral("错误");
        break;

    default:
        break;
    }

    QString qstrTime = QDateTime::currentDateTime()
        .toString(QStringLiteral("yyyy-MM-dd HH:mm:ss.zzz"));
    QString qstrLogFormat("[%1][%2][%3] %4");
    QString qstrLog = qstrLogFormat.arg(qstrTime).arg(qstrLevel).arg(logSource).arg(logInfo);
    qDebug() << qstrLog;

    // TODO 记录日志
    WriteToLogFile( qstrLog);
   // WriteToLogDB();
}

void CLogging::WriteToLogFile(QString msg)
{
    // TODO 日志写入日志文件
	QFile file(m_fileName);
	if( !file.open( QIODevice::ReadWrite | QIODevice::Truncate ))
	{
		return;
	}
	QTextStream out(&file);  
	out << msg << "\n";  
	file.close();
}


void CLogging::LogDebug(const QString & logSource, const QString & logInfo)
{
    GetInstance()->Log(LOG_DEBUG, logSource, logInfo);
}

void CLogging::LogInfo(const QString & logSource, const QString & logInfo)
{
     GetInstance()->Log(LOG_INFO, logSource, logInfo);
}

void CLogging::LogImportantInfo(const QString & logSource, const QString & logInfo)
{
    GetInstance()->Log(LOG_IMPORTANT_INFO, logSource, logInfo);
}

void CLogging::LogWaring(const QString & logSource, const QString & logInfo)
{
    GetInstance()->Log(LOG_WARNING, logSource, logInfo);
}

void CLogging::LogError(const QString & logSource, const QString & logInfo)
{
    GetInstance()->Log(LOG_ERROR, logSource, logInfo);
}
