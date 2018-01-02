/*****************************************************
** 文件名：  socket_service.h
** 版  权：  XXX 版权所有
** 版  本：  
** 内容简述：
** 创建日期：2015/07/15
** 创建人：  唐桥
** 修改记录：
   日期        版本      修改人    修改内容 
    
*****************************************************/
#ifndef _SOCKET_SERVICE_H_
#define _SOCKET_SERVICE_H_
#include "service/i_service.h"
#include "tcp_server.h"
#include <boost/asio.hpp>
#include <QRunnable>
#include <memory>

using namespace boost::asio;

// 统一对外提供网络通信功能
class CSocketService : public IInternalService
{
public:
    // 
    static CSocketService * GetInstance();

    // 
    static void DestroyInstance();

    // 启动服务
    virtual int StartInternalService();

    // 停止服务
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


// 专门监听TCP连接请求的线程
class CSocketServiceTask : public QRunnable
{
    virtual void run();
};


// 设备TCP连接空闲时间检测线程
class CDevConnKeepLiveTask : public QRunnable
{
    virtual void run();
};


#endif//!_SOCKET_SERVICE_H_