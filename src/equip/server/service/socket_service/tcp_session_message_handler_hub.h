/*****************************************************
** 文件名：  tcp_session_message_handler.h
** 版  权：  XXX 版权所有
** 版  本：  
** 内容简述：TCP会话消息处理
** 创建日期：2015/07/15
** 创建人：  唐桥
** 修改记录：
   日期        版本      修改人    修改内容 
    
*****************************************************/
#include <QQueue>
#include <QHash>
#include <QList>
#include <memory>
#include <QMutex>
#include <QWaitCondition>
#include <QRunnable>
#include "message/message.h"

#include "service/socket_service/i_tcp_session_message_handler.h"

#ifndef _TCP_SESSION_MESSAGE_HANDLER_HUB_H_
#define _TCP_SESSION_MESSAGE_HANDLER_HUB_H_

#include "service/socket_service/tcp_session.h"


class CTcpSession;

typedef struct _T_TCP_SESSION_MSG
{
    tcp_session_ptr pSession;
    message_ptr   pMessage;

    _T_TCP_SESSION_MSG() 
    {
        pSession = NULL;
        pMessage = NULL;
    }

    ~_T_TCP_SESSION_MSG() 
    {
        if (pMessage) {
            pMessage = NULL;
        }
    }
}T_TCP_SESSION_MSG;

typedef std::shared_ptr<T_TCP_SESSION_MSG> session_msg_ptr;


typedef struct _T_TCP_SESSION_MSG_HANDLER_FUNCV2
{
    ITcpSessionMessageHandler             * handler;
    ITcpSessionMessageHandler::HandleSessionMessageV2 handlerFuncV2;

    _T_TCP_SESSION_MSG_HANDLER_FUNCV2()
    {
        handler = NULL;
        handlerFuncV2 = NULL;
    }
}T_TCP_SESSION_MSG_HANDLER_FUNCV2;

typedef struct _T_TCP_SESSION_MSG_HANDLER_FUNC
{
    ITcpSessionMessageHandler             * handler;
    ITcpSessionMessageHandler::HandleSessionMessage handlerFunc;

    _T_TCP_SESSION_MSG_HANDLER_FUNC(ITcpSessionMessageHandler * handler,
        ITcpSessionMessageHandler::HandleSessionMessage handlerFunc)
    {
        this->handler = handler;
        this->handlerFunc = handlerFunc;
    }
}T_TCP_SESSION_MSG_HANDLER_FUNC;

typedef struct _T_TCP_SESSION_MSG_HANDLER_FUNCS
{
    U32                                     cmd;
    T_TCP_SESSION_MSG_HANDLER_FUNCV2        handlerFuncWithRet;
    QList<T_TCP_SESSION_MSG_HANDLER_FUNC>   handleFuncsWithoutRet;
}T_TCP_SESSION_MSG_HANDLER_FUNCS;


class CTcpSessionMessageHandlerHub : public ITcpSessionMessageHandler
{
public:

    static CTcpSessionMessageHandlerHub * GetInstance();

    static void DestroyInstance();

    void CommitSessionMessage(tcp_session_ptr pSession, message_ptr pMessage);

    void RegisterSessionMessageHandler(U32 cmd, 
        ITcpSessionMessageHandler * handler,
        ITcpSessionMessageHandler::HandleSessionMessage handlerFunc);

    void RegisterSessionMessageHandler(U32 cmd,
        ITcpSessionMessageHandler * handler, 
        ITcpSessionMessageHandler::HandleSessionMessageV2 handlerFunc);

    void UnregisterSessionMessageHandler(ITcpSessionMessageHandler * handler);

    bool NotifyExit();

    void NotifyExit(bool flag);

    int HandleSessionMessage(tcp_session_ptr session, message_ptr sessionMessageIn);

    int HandleSessionMessage(tcp_session_ptr session, message_ptr sessionMessageIn,
        message_ptr sessionMessageOut);

private:

    CTcpSessionMessageHandlerHub();

    ~CTcpSessionMessageHandlerHub();

    void RegisterSessionMessageDefaultHandlerFunc();

    bool WaitQueueSessionMessage(int milliseconds);

    bool TakeSessionMessage(session_msg_ptr & sessionMessage);

    bool IsQueueSessionMessageAvaiable();

    bool GetSessionMessgeHandler(U32 cmd, T_TCP_SESSION_MSG_HANDLER_FUNCS & handlerFuncs);

private:
    static CTcpSessionMessageHandlerHub       * m_pInstance;
    
    QQueue<session_msg_ptr>                     m_queueSessionMessages;         // 接收缓冲数据，待处理队列
    QMutex                                    * m_pQueueLock;                    // 待处理队列锁
    QWaitCondition                              m_condtionForQueueAvaiable;     // 待处理队列中有可用数据的通知条件
    QHash<U32, T_TCP_SESSION_MSG_HANDLER_FUNCS> m_hashSessaionMessageHandlers;  // TCP会话消息回调处理哈希表
    QMutex                                      m_hashLock;                     // 哈希表锁
    bool                                        m_bNotifyExit;                  // 是否已通知退出

    friend class CSessionMessageHandleTask;
};

class CSessionMessageHandleTask : public QRunnable
{
    virtual void run();
};

#endif // !_TCP_SESSION_MESSAGE_HANDLER_HUB_H_
