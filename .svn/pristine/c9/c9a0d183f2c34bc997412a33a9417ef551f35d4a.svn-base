#ifndef _CONFIGURATION_SERVICE_H_
#define _CONFIGURATION_SERVICE_H_

#include "service/i_service.h"
#include "service/log_service/log_service.h"

class CConfigrationService : public IInternalService
{
public:

    static CConfigrationService * GetInstance();

    static void DestroyInstance();

    virtual int StartInternalService();

    virtual int StopInternalService();

    virtual int NotifyServiceToStop();

    virtual QString GetServiceName();

    //////////////////////////////////////////////////////////////////////////
    // 对外提供的操作接口

    // 获取日志级别
    E_LOG_LEVEL GetLogLevel();

    // 设置日志级别
    void SetLogLevel(E_LOG_LEVEL level);

    // 获取TCP监听端口
    unsigned short GetPort();

    // 获取设备连接的最大空闲时间
    int GetDevConnMaxIdleTime();

    // 获取数据库连接字符串
    QString GetDBConnStr();

    // 获取数据库登录账号
    QString GetDBUser();

    // 获取数据库登录密码
    QString GetDBPassword();

    // 获取数据库主机名
    QString GetDBHostName();

    // 获取数据库服务监听端口
    int GetDBPort();

    // 获取便携式H2S检测仪的类型编码
    QString GetTypeCodeOfPortableH2SDetector();

    // 获取固定式H2S检测仪的类型编码
    QString GetTypeCodeOfStaticH2SDetector();

private:

    CConfigrationService();

    ~CConfigrationService();

    // 从配置文件中加载配置
    void LoadConfiguration();

private:
    static CConfigrationService * m_pInstance;
    E_LOG_LEVEL         m_eLogLevel;    // 日志输出级别
    unsigned short      m_unPort;       // 服务器绑定的端口
    unsigned int        m_unDevConnMaxIdleTime;// 设备连接最大空闲时间
    QString             m_qstrDBConnStr;// 数据库连接字符串
    QString             m_qstrDBUser;   // 数据库登录用户名
    QString             m_qstrDBPassword;//数据库登录密码
    QString             m_qstrDBHostName;// 数据库服务器主机名（IP）
    int                 m_nDBPort;      // 数据库连接端口
    QString             m_qstrTypeCodeOfPortableH2SDetector;// 便携式H2S检测仪设备类型码
    QString             m_qstrTypeCodeOfStaticH2SDetector;// 固定式H2S检查仪
};

#endif//!_CONFIGURATION_SERVICE_H_