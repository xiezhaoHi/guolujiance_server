#ifndef _MESSAGE_LOGIN_REP_H_
#define _MESSAGE_LOGIN_REP_H_

#include "message/i_message.h"
#include "message/message_header.h"

class CMessageLoginRep : public IMessage
{
public:
    
    CMessageLoginRep();

    virtual ~CMessageLoginRep();

    virtual std::size_t Length();

    virtual bool WriteToOutputByteArray(OutputByteArray & out);

    virtual bool ReadFromInputByteArray(InputByteArray & in);

    virtual QString ToString();

    static IMessage * CreateFromHeader(const CMessageHeader & header);

public:

    T_MSG_SERVER_TIME   m_struct;
};

#endif//!_MESSAGE_LOGIN_REP_H_