#ifndef _MESSAGE_HEARTBEAT_H_
#define _MESSAGE_HEARTBEAT_H_


#include "message/i_message.h"

class CMessageHeartbeat : public IMessage
{
public:

    CMessageHeartbeat();

    virtual ~CMessageHeartbeat();

    virtual std::size_t Length();

    virtual bool WriteToOutputByteArray(OutputByteArray & out);

    virtual bool ReadFromInputByteArray(InputByteArray & in);

    virtual QString ToString();

    static IMessage * CreateFromHeader(const CMessageHeader & header);

public:
    T_MSG_HEARTBEAT m_struct;
};



#endif//!_MESSAGE_HEARTBEAT_H_