/*****************************************************
** 文件名：  message_handler_hub.h
** 版  权：  XXX 版权所有
** 版  本：  
** 内容简述：
** 创建日期：2015/07/15
** 创建人：  唐桥
** 修改记录：
   日期        版本      修改人    修改内容 
    
*****************************************************/
#ifndef _MESSAGE_HANDLER_HUB_H_
#define _MESSAGE_HANDLER_HUB_H_

#include "i_message_handler.h"
#include "protocol\protocol_data_type.h"
#include "message.h"
#include <QMultiHash>
#include <QMutex>


class CMessageHanlderHub 
{
public:

    static CMessageHanlderHub * GetInstance();

    static void DestroyInstance();

    bool HandleMessage(const message_ptr request, message_ptr ppResponse);

    void RegisterMessageHandler(U32 cmd, IMessageHandler *pHandler,
        IMessageHandler::HandleMessage handlerFunc);

    void UnregisterMessageHandler(U32 cmd, IMessageHandler *pHandler,
        IMessageHandler::HandleMessage handlerFunc);

private:

    CMessageHanlderHub();

    ~CMessageHanlderHub();

private:
    typedef struct T_MESSAGE_HANDLER
    {
        IMessageHandler                * pHandler;
        IMessageHandler::HandleMessage   pHandlerFunc;
    }T_MESSAGE_HANDLER;

private:
    static CMessageHanlderHub         * m_pInstance;
    QMultiHash<U32, T_MESSAGE_HANDLER>  m_hashHandlers;
    QMutex                              m_mutexForHash;
};


#endif//!_MESSAGE_HANDLER_HUB_H_