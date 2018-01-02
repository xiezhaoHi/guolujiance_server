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
//** ������:   HandleMessage
//** ���ܼ���: 
//** �������: 
//   const CMessage & request:
//   CMessage * * ppResponse:
//
//** ����ֵ: 
//   bool:
//
//** �������ڣ�2015/07/15
//** �����ˣ�  ����
//** �޸ļ�¼��
//   ����        �汾      �޸���    �޸�����
//
//** ����˵����
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
//** ������:   RegisterMessageHandler
//** ���ܼ���: 
//** �������: 
//   U32 cmd:
//   IMessageHandler * pHandler:
//   IMessageHandler::HandleMessage handlerFunc:
//
//** ����ֵ: 
//   void:
//
//** �������ڣ�2015/07/15
//** �����ˣ�  ����
//** �޸ļ�¼��
//   ����        �汾      �޸���    �޸�����
//
//** ����˵����
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
//** ������:   UnregisterMessageHandler
//** ���ܼ���: 
//** �������: 
//   U32 cmd:
//   IMessageHandler * pHandler:
//   IMessageHandler::HandleMessage handlerFunc:
//
//** ����ֵ: 
//   void:
//
//** �������ڣ�2015/07/15
//** �����ˣ�  ����
//** �޸ļ�¼��
//   ����        �汾      �޸���    �޸�����
//
//** ����˵����
//******************************************************
void CMessageHanlderHub::UnregisterMessageHandler(U32 cmd,
                                                  IMessageHandler *pHandler, 
                                                  IMessageHandler::HandleMessage handlerFunc)
{
    // TODO
    _ASSERT(false);
    QMutexLocker lock(&m_mutexForHash);
}
