#ifndef _MESSAGE_UNRECOGNIZED_BODY_TYPE_H_
#define _MESSAGE_UNRECOGNIZED_BODY_TYPE_H_


#include "message/i_message.h"


class CMessageUnrecognizedBodyType : public IMessage
{
public:

    CMessageUnrecognizedBodyType();

    virtual ~CMessageUnrecognizedBodyType();

    virtual std::size_t Length();

    virtual bool WriteToOutputByteArray(OutputByteArray & out);

    virtual bool ReadFromInputByteArray(InputByteArray & in);

    virtual QString ToString();

    static IMessage * CreateFromHeader(const CMessageHeader & header);

public:

    U8          m_bodyDataLen;
    QByteArray  m_bodyData;
};

#endif//!_MESSAGE_UNRECOGNIZED_BODY_TYPE_H_