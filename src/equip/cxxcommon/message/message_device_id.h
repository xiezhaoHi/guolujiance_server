#ifndef _MESSAGE_DEVICE_ID_H_
#define _MESSAGE_DEVICE_ID_H_


#include "message/i_message.h"

class CMessageDeviceID : public IMessage
{
public:

    CMessageDeviceID();

    virtual ~CMessageDeviceID();

    virtual std::size_t Length();

    virtual bool WriteToOutputByteArray(OutputByteArray & out);

    virtual bool ReadFromInputByteArray(InputByteArray & in);

    virtual QString ToString();

public:

    T_MSG_DEVICE_ID m_struct;
};



#endif//!_MESSAGE_DEVICE_ID_H_