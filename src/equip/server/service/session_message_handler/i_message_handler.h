#ifndef _I_MESSAGE_HANDLER_H_
#define _I_MESSAGE_HANDLER_H_

#include "message/message.h"

class IMessageHandler
{
public:

    IMessageHandler() {}

    virtual ~IMessageHandler() {}

    typedef bool (IMessageHandler:: * HandleMessage)(const message_ptr request,
        message_ptr ppResponse);
};


#endif//!_I_MESSAGE_HANDLER_H_