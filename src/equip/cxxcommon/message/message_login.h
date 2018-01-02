#ifndef _MESSAGE_LOGIN_H_
#define _MESSAGE_LOGIN_H_

#include "i_message.h"
#include <QString>

class CMessageLogin : public IMessage
{
public:

    CMessageLogin();

    virtual ~CMessageLogin();

    virtual std::size_t Length();

    virtual bool WriteToOutputByteArray(OutputByteArray & out);

    virtual bool ReadFromInputByteArray(InputByteArray & in);

    static IMessage * CreateFromHeader(const CMessageHeader & header);

    virtual QString ToString();

public:
    T_MSG_LOGIN     m_struct;
    QString         m_qstrAccount;
    QString         m_qstrPassword;
};




#endif//!_MESSAGE_LOGIN_H_