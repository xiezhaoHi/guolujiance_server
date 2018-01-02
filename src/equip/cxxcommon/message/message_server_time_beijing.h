#ifndef _MESSAGE_SERVER_TIME_BEIJING_H_
#define _MESSAGE_SERVER_TIME_BEIJING_H_


#include "message/i_message.h"

class CMessageServerTimeBeiJing : public IMessage
{
public:
    CMessageServerTimeBeiJing();

    virtual ~CMessageServerTimeBeiJing();

    virtual std::size_t Length();

    virtual bool WriteToOutputByteArray(OutputByteArray & out);

    virtual bool ReadFromInputByteArray(InputByteArray & in);

    static IMessage * CreateFromHeader(const CMessageHeader & header);

    virtual QString ToString();

public:
    T_MSG_SERVER_TIME_BEIJING m_struct;
};



#endif//!_MESSAGE_SERVER_TIME_BEIJING_H_