#include "tcp_session_message_handler_hub.h"
#include "service/log_service/log_service.h"
#include "tcp_session.h"
#include "protocol/protocol_cmd.h"
#include <QMutexLocker>
#include <QDateTime>
#include <QThread>
#include "message/message.h"


CTcpSessionMessageHandlerHub * CTcpSessionMessageHandlerHub::m_pInstance = NULL;


CTcpSessionMessageHandlerHub::CTcpSessionMessageHandlerHub()
{
    m_bNotifyExit = false;
    m_pQueueLock = new QMutex();
}

CTcpSessionMessageHandlerHub::~CTcpSessionMessageHandlerHub()
{
    delete m_pQueueLock;
}

CTcpSessionMessageHandlerHub * CTcpSessionMessageHandlerHub::GetInstance()
{
    if (!m_pInstance) {
        m_pInstance = new CTcpSessionMessageHandlerHub();
        m_pInstance->RegisterSessionMessageDefaultHandlerFunc();
    }
    return m_pInstance;
}

void CTcpSessionMessageHandlerHub::DestroyInstance()
{
    if (m_pInstance) {
        delete m_pInstance;
        m_pInstance = NULL;
    }
}

void CTcpSessionMessageHandlerHub::CommitSessionMessage(tcp_session_ptr pSession,
                                                        message_ptr pMessage)
{
    QMutexLocker lock(m_pQueueLock);
    
    session_msg_ptr sessionMessage = std::make_shared<T_TCP_SESSION_MSG>();
    sessionMessage->pSession = pSession;
    sessionMessage->pMessage = pMessage;
    m_queueSessionMessages.push_back(sessionMessage);

    m_condtionForQueueAvaiable.wakeOne();
}

void CTcpSessionMessageHandlerHub::RegisterSessionMessageHandler(U32 cmd,
                                                                 ITcpSessionMessageHandler * handler,
                                                                 ITcpSessionMessageHandler::HandleSessionMessage handlerFunc)
{
    QMutexLocker lock(&m_hashLock);

    QHash<U32, T_TCP_SESSION_MSG_HANDLER_FUNCS>::iterator it = m_hashSessaionMessageHandlers.find(cmd);
    if (it == m_hashSessaionMessageHandlers.end()) {
        T_TCP_SESSION_MSG_HANDLER_FUNCS funcs;
        funcs.cmd = cmd;
        funcs.handleFuncsWithoutRet.push_back(T_TCP_SESSION_MSG_HANDLER_FUNC(handler, handlerFunc));
        m_hashSessaionMessageHandlers.insert(cmd, funcs);
    } else {
        it->handleFuncsWithoutRet.push_back(T_TCP_SESSION_MSG_HANDLER_FUNC(handler, handlerFunc));
    }
}

void CTcpSessionMessageHandlerHub::RegisterSessionMessageHandler(U32 cmd, 
                                                                 ITcpSessionMessageHandler * handler, 
                                                                 ITcpSessionMessageHandler::HandleSessionMessageV2 handlerFunc)
{
    QMutexLocker lock(&m_hashLock);

    QHash<U32, T_TCP_SESSION_MSG_HANDLER_FUNCS>::iterator it = m_hashSessaionMessageHandlers.find(cmd);
    if (it == m_hashSessaionMessageHandlers.end()) {
        T_TCP_SESSION_MSG_HANDLER_FUNCS funcs;
        funcs.cmd = cmd;
        funcs.handlerFuncWithRet.handler = handler;
        funcs.handlerFuncWithRet.handlerFuncV2 = handlerFunc;
        m_hashSessaionMessageHandlers.insert(cmd, funcs);
    } else {
        it->handlerFuncWithRet.handler = handler;
        it->handlerFuncWithRet.handlerFuncV2 = handlerFunc;
    }
}

void CTcpSessionMessageHandlerHub::UnregisterSessionMessageHandler(ITcpSessionMessageHandler * handler)
{
    QMutexLocker lock(&m_hashLock);

    QHash<U32, T_TCP_SESSION_MSG_HANDLER_FUNCS>::iterator it = m_hashSessaionMessageHandlers.begin();
    while (it != m_hashSessaionMessageHandlers.end()) {
        if (it->handlerFuncWithRet.handler == handler) {
            it->handlerFuncWithRet.handler = NULL;
            it->handlerFuncWithRet.handlerFuncV2 = NULL;
        }

        QList<T_TCP_SESSION_MSG_HANDLER_FUNC>::iterator itLst = it->handleFuncsWithoutRet.begin();
        while (itLst != it->handleFuncsWithoutRet.end()) {
            if (itLst->handler == handler) {
                itLst = it->handleFuncsWithoutRet.erase(itLst);
            } else {
                itLst++;
            }
        }

        it++;
    }
}

bool CTcpSessionMessageHandlerHub::WaitQueueSessionMessage(int milliseconds)
{
    QMutexLocker lock(m_pQueueLock);
    return m_condtionForQueueAvaiable.wait(m_pQueueLock, milliseconds);// 
}

bool CTcpSessionMessageHandlerHub::TakeSessionMessage(session_msg_ptr & sessionMessage)
{
    QMutexLocker lock(m_pQueueLock);
    if (!m_queueSessionMessages.empty()) {
        sessionMessage = m_queueSessionMessages.front();
        m_queueSessionMessages.pop_front();
        return true;
    }
    return false;
}

bool CTcpSessionMessageHandlerHub::IsQueueSessionMessageAvaiable()
{
    QMutexLocker lock(m_pQueueLock);
    return !m_queueSessionMessages.empty();
}


bool CTcpSessionMessageHandlerHub::GetSessionMessgeHandler(U32 cmd, 
                                                           T_TCP_SESSION_MSG_HANDLER_FUNCS & handlerFuncs)
{
    QMutexLocker lock(&m_hashLock);

    QHash<U32, T_TCP_SESSION_MSG_HANDLER_FUNCS>::iterator it = m_hashSessaionMessageHandlers.find(cmd);
    if (it == m_hashSessaionMessageHandlers.end()) {
        return false;
    }

    handlerFuncs.cmd = it->cmd;
    handlerFuncs.handlerFuncWithRet = it->handlerFuncWithRet;
    handlerFuncs.handleFuncsWithoutRet.append(it->handleFuncsWithoutRet);

    return true;
}

bool CTcpSessionMessageHandlerHub::NotifyExit()
{
    return m_bNotifyExit;
}

void CTcpSessionMessageHandlerHub::NotifyExit(bool flag /*= true*/)
{
    this->m_bNotifyExit = flag;
}

void CTcpSessionMessageHandlerHub::RegisterSessionMessageDefaultHandlerFunc()
{
    // PC客户端命令默认处理(输出消息内容到日志文件)
    RegisterSessionMessageHandler(PC_CLIENT_CMD_LOGIN, this, 
        static_cast<ITcpSessionMessageHandler::HandleSessionMessage>(&CTcpSessionMessageHandlerHub::HandleSessionMessage));
    RegisterSessionMessageHandler(PC_CLIENT_CMD_GET_DEVICE_LIST, this, 
        static_cast<ITcpSessionMessageHandler::HandleSessionMessage>(&CTcpSessionMessageHandlerHub::HandleSessionMessage));
    RegisterSessionMessageHandler(PC_CLIENT_CMD_GET_DEVICE_INFO, this, 
        static_cast<ITcpSessionMessageHandler::HandleSessionMessage>(&CTcpSessionMessageHandlerHub::HandleSessionMessage));
    RegisterSessionMessageHandler(PC_CLIENT_CMD_EXIT, this, 
        static_cast<ITcpSessionMessageHandler::HandleSessionMessage>(&CTcpSessionMessageHandlerHub::HandleSessionMessage));

    // 终端采集设备命令默认处理(输出消息内容到日志文件)
    RegisterSessionMessageHandler(DEVICE_CMD_LOGIN, this, 
        static_cast<ITcpSessionMessageHandler::HandleSessionMessage>(&CTcpSessionMessageHandlerHub::HandleSessionMessage));
    RegisterSessionMessageHandler(DEVICE_CMD_REALTIME_DATA, this, 
        static_cast<ITcpSessionMessageHandler::HandleSessionMessage>(&CTcpSessionMessageHandlerHub::HandleSessionMessage));
    RegisterSessionMessageHandler(DEVICE_CMD_EXIT, this, 
        static_cast<ITcpSessionMessageHandler::HandleSessionMessage>(&CTcpSessionMessageHandlerHub::HandleSessionMessage));
}

int CTcpSessionMessageHandlerHub::HandleSessionMessage(tcp_session_ptr session, message_ptr sessionMessageIn)
{
    if (sessionMessageIn) {
        LOG_DEBUG() << QStringLiteral("接收 ") << sessionMessageIn->ToString();
        return 0;
    }
    return 1;
}

int CTcpSessionMessageHandlerHub::HandleSessionMessage(tcp_session_ptr session, 
                                                       message_ptr sessionMessageIn, 
                                                       message_ptr sessionMessageOut)
{
    // TODO
    return 0;
}


void CSessionMessageHandleTask::run()
{
    QString qstrFormat(QStringLiteral("%1##%2"));
    QString taskID = qstrFormat.arg(QDateTime::currentDateTime().toString("dd#HH_mm_ss__zzz"))
        .arg((int)QThread::currentThreadId());
    LOG_DEBUG() << "CTcpSessionMessageHandlerHub task " << taskID << "start";

    while (!CTcpSessionMessageHandlerHub::GetInstance()->NotifyExit()) {
        if (!CTcpSessionMessageHandlerHub::GetInstance()->IsQueueSessionMessageAvaiable()) {
            CTcpSessionMessageHandlerHub::GetInstance()->WaitQueueSessionMessage(500);
            continue;
        }

        session_msg_ptr sessionMessage;// 变量里的message在变量离开作用域以后自动删除
        if (CTcpSessionMessageHandlerHub::GetInstance()->TakeSessionMessage(sessionMessage)) {// 取走队首的消息

            T_TCP_SESSION_MSG_HANDLER_FUNCS funcs;
            if (CTcpSessionMessageHandlerHub::GetInstance()->GetSessionMessgeHandler(sessionMessage->pMessage->m_header.m_struct.cmd, funcs)) {// 获取
                
                // 调用没有回复消息的处理函数
                for (QList<T_TCP_SESSION_MSG_HANDLER_FUNC>::iterator it = funcs.handleFuncsWithoutRet.begin();
                    it != funcs.handleFuncsWithoutRet.end();
                    it++) {
                        (it)->handler->m_func = (it)->handlerFunc;
                        ((it)->handler->*((it)->handler->m_func))(sessionMessage->pSession, sessionMessage->pMessage);
                }

                // 调用有回复消息的处理函数
                if (funcs.handlerFuncWithRet.handler && funcs.handlerFuncWithRet.handlerFuncV2) {
                    funcs.handlerFuncWithRet.handler->m_funcV2 = funcs.handlerFuncWithRet.handlerFuncV2;

                    message_ptr response = std::make_shared<CMessage>();
                    int ret = (funcs.handlerFuncWithRet.handler->*(funcs.handlerFuncWithRet.handler->m_funcV2))(sessionMessage->pSession, sessionMessage->pMessage, response);
                    response->m_header.m_struct = sessionMessage->pMessage->m_header.m_struct;
                    response->m_header.m_struct.cmd = SET_PC_SERVER_CMD_BIT(SET_REP_BIT(sessionMessage->pMessage->m_header.m_struct.cmd));
                    response->m_header.m_struct.cmdResult = ret;
                    if (0 != ret && sessionMessage->pMessage->m_header.m_struct.cmdResult == 0) {
                        // 执行错误，且发送端不要求获得错误信息
                        delete response->m_pBody;
                        response->m_pBody = NULL;
                        response->m_header.m_struct.bodyLength = 0;
                    }
                    sessionMessage->pSession->deliver(response);// 发送回复消息
                }
            }
        }
    }

    LOG_DEBUG() << "CTcpSessionMessageHandlerHub task" << taskID << " exit";
}
