#ifndef _MESSAGE_ERROR_INFO_H_
#define _MESSAGE_ERROR_INFO_H_

#include "message/i_message.h"

class CMessageErrorInfo : public IMessage
{
public:

    CMessageErrorInfo();

    virtual ~CMessageErrorInfo();

    virtual std::size_t Length();

    virtual bool WriteToOutputByteArray(OutputByteArray & out);

    virtual bool ReadFromInputByteArray(InputByteArray & in);

    virtual QString ToString();

    static IMessage * CreateFromHeader(const CMessageHeader & header);

public:
    T_MSG_ERROR_INFO        m_struct;
    QString                 m_qstrErrorInfo;
};

#endif//!_MESSAGE_ERROR_INFO_H_