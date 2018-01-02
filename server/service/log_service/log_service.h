#ifndef _LOG_SERVICE_H_
#define _LOG_SERVICE_H_

#include "service/i_service.h"
#include <QDebug>
#include <QTextStream>
#include <QMutex>





// 日志文件输出实用宏定义
#define LOG_DEBUG()     qDebug()    /*日志级别为DEBUG*/
#define LOG_INFO()      qWarning()  /*日志级别为INFO*/
#define LOG_WARING()    qCritical() /*日志级别为WARING*/
#define LOG_ERROR_BEGIN() {\
                            QTextStream textstream;\
                            textstream
#define LOG_ERROR_END()     "";\
                            QString str;\
                            textstream >> str;\
                            qFatal(str.toLatin1().data());\
                          }
            //qFatal("FATAL")    /*日志级别为ERROR*/


// 命中断言时输出ERROR日志
#define LOG_ASSERT(cond) ((!(cond)) ? qt_assert(#cond,__FILE__,__LINE__) : qt_noop())


// 日志输出级别枚举值定义
typedef enum _E_LOG_LEVEL
{
    LEVEL_NONE = 0,
    LEVEL_DEBUG = 1,
    LEVEL_INFO,
    LEVEL_WARING,
    LEVEL_ERROR,
}E_LOG_LEVEL;


class ILogObserver
{
public:
    ILogObserver() {}

    virtual ~ILogObserver() {}

    virtual void HandleLog(E_LOG_LEVEL level, const QString & log) = 0;
};

class CLogService : public IInternalService, public ILogObserver
{
public:

    static CLogService * GetInstance();

    static void DestroyInstance();

    virtual int StartInternalService();

    virtual int StopInternalService();

    virtual int NotifyServiceToStop();

    virtual QString GetServiceName();

    // 注册日志观察者
    void RegisterLogObserver(ILogObserver * observer);

    // 注册日志观察者
    void UnregisterLogObserver(ILogObserver * observer);

    // 处理日志消息
    virtual void HandleLog(E_LOG_LEVEL level, const QString & log);

private:

    CLogService();

    ~CLogService();


private:

    static CLogService * m_pInstance;
    ILogObserver       * m_pInnerObserver;
    QMutex               m_observerLock;
};



#endif//!_LOG_SERVICE_H_