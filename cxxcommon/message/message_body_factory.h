/*****************************************************
** 文件名：  message_body_allocator.h
** 版  权：  XXX 版权所有
** 版  本：  
** 内容简述：根据消息头部信息，生成消息体对象
** 创建日期：2015/07/15
** 创建人：  唐桥
** 修改记录：
   日期        版本      修改人    修改内容 
    
*****************************************************/
#ifndef _MESSAGE_BODY_ALLOCATOR_H_
#define _MESSAGE_BODY_ALLOCATOR_H_

#include "i_message.h"
#include "message/message_error_info.h"
#include <QHash>

class CMessageBodyFactory
{
public:
    // 
    static CMessageBodyFactory * GetInstance();

    static void DestroyInstance();

    // 产生与消息头部信息对应的消息体对象
    IMessage * Product(const CMessageHeader & header);

    // 创建携带有错误信息的消息对象
    CMessageErrorInfo * CreateMessageErrorInfo(const CMessageHeader & header);

private:

    CMessageBodyFactory();

    ~CMessageBodyFactory();

    void RegisterMessageBodyCreators();

    void RegisterMessageBodyCreator(U32 cmd, CreateBodyFromHeader creator,
                                    const char *strCmd, const char *strCreator);

private:
    static CMessageBodyFactory *        m_pInstance;
    QHash<U32, CreateBodyFromHeader>    m_hash;
};

#endif//!_MESSAGE_BODY_ALLOCATOR_H_