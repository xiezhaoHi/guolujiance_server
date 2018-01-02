#ifndef _LOG_SERVICE_H_
#define _LOG_SERVICE_H_

#include "service/i_service.h"
#include <QDebug>
#include <QTextStream>
#include <QMutex>





// ��־�ļ����ʵ�ú궨��
#define LOG_DEBUG()     qDebug()    /*��־����ΪDEBUG*/
#define LOG_INFO()      qWarning()  /*��־����ΪINFO*/
#define LOG_WARING()    qCritical() /*��־����ΪWARING*/
#define LOG_ERROR_BEGIN() {\
                            QTextStream textstream;\
                            textstream
#define LOG_ERROR_END()     "";\
                            QString str;\
                            textstream >> str;\
                            qFatal(str.toLatin1().data());\
                          }
            //qFatal("FATAL")    /*��־����ΪERROR*/


// ���ж���ʱ���ERROR��־
#define LOG_ASSERT(cond) ((!(cond)) ? qt_assert(#cond,__FILE__,__LINE__) : qt_noop())


// ��־�������ö��ֵ����
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

    // ע����־�۲���
    void RegisterLogObserver(ILogObserver * observer);

    // ע����־�۲���
    void UnregisterLogObserver(ILogObserver * observer);

    // ������־��Ϣ
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