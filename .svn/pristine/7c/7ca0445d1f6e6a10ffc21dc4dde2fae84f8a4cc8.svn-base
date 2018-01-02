#include "message_handler_hub.h"
#include "QMutex"


CMessageHanlderHub * CMessageHanlderHub::m_pInstance = NULL;


CMessageHanlderHub::CMessageHanlderHub()
{

}

CMessageHanlderHub::~CMessageHanlderHub()
{
    _ASSERT(m_hashHandlers.empty());
}

CMessageHanlderHub * CMessageHanlderHub::GetInstance()
{
    if (!m_pInstance) {
        m_pInstance = new CMessageHanlderHub();
    }

    return m_pInstance;
}

void CMessageHanlderHub::DestroyInstance()
{
    if (m_pInstance) {
        delete m_pInstance;
        m_pInstance = NULL;
    }
}

//******************************************************
//** 函数名:   HandleMessage
//** 功能简述: 
//** 输入参数: 
//   const CMessage & request:
//   CMessage * * ppResponse:
//
//** 返回值: 
//   bool:
//
//** 创建日期：2015/07/15
//** 创建人：  唐桥
//** 修改记录：
//   日期        版本      修改人    修改内容
//
//** 其它说明：
//******************************************************
bool CMessageHanlderHub::HandleMessage(const message_ptr request, 
                                       message_ptr ppResponse)
{
    _ASSERT(ppResponse);

    bool ret = false;
    QList<T_MESSAGE_HANDLER> listHandlers;

    U32 cmd = request->m_header.m_struct.cmd;
    {
        QMutexLocker lock(&m_mutexForHash);
        listHandlers = m_hashHandlers.values(cmd);
        if (listHandlers.empty()) {
            _ASSERT(false);
        }
    }

    for (QList<T_MESSAGE_HANDLER>::iterator it = listHandlers.begin();
        it != listHandlers.end();
        it++)
    {
        _ASSERT(it->pHandler);
        _ASSERT(it->pHandlerFunc);

        ret = ((it->pHandler)->*(it->pHandlerFunc))(request, ppResponse);
    }

    return ret;
}

//******************************************************
//** 函数名:   RegisterMessageHandler
//** 功能简述: 
//** 输入参数: 
//   U32 cmd:
//   IMessageHandler * pHandler:
//   IMessageHandler::HandleMessage handlerFunc:
//
//** 返回值: 
//   void:
//
//** 创建日期：2015/07/15
//** 创建人：  唐桥
//** 修改记录：
//   日期        版本      修改人    修改内容
//
//** 其它说明：
//******************************************************
void CMessageHanlderHub::RegisterMessageHandler(U32 cmd, 
                                                IMessageHandler *pHandler, 
                                                IMessageHandler::HandleMessage handlerFunc)
{
    _ASSERT(pHandler);
    _ASSERT(handlerFunc);
    // TODO
    T_MESSAGE_HANDLER val;
    val.pHandler = pHandler;
    val.pHandlerFunc = handlerFunc;

    QMutexLocker lock(&m_mutexForHash);
    m_hashHandlers.insert(cmd, val);
}

//******************************************************
//** 函数名:   UnregisterMessageHandler
//** 功能简述: 
//** 输入参数: 
//   U32 cmd:
//   IMessageHandler * pHandler:
//   IMessageHandler::HandleMessage handlerFunc:
//
//** 返回值: 
//   void:
//
//** 创建日期：2015/07/15
//** 创建人：  唐桥
//** 修改记录：
//   日期        版本      修改人    修改内容
//
//** 其它说明：
//******************************************************
void CMessageHanlderHub::UnregisterMessageHandler(U32 cmd,
                                                  IMessageHandler *pHandler, 
                                                  IMessageHandler::HandleMessage handlerFunc)
{
    // TODO
    _ASSERT(false);
    QMutexLocker lock(&m_mutexForHash);
}
