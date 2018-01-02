#include "message_login.h"
#include "message/message_header.h"


CMessageLogin::CMessageLogin()
{
    static_assert(sizeof(T_MSG_LOGIN) == 4, "sizeof(T_MSG_LOGIN) != 4");
    memset(&m_struct, 0x00, sizeof(T_MSG_LOGIN));
}


CMessageLogin::~CMessageLogin()
{

}

std::size_t CMessageLogin::Length()
{
    m_struct.accountLen = m_qstrAccount.toUtf8().length();
    m_struct.passwordLen = m_qstrPassword.toUtf8().length();
    return sizeof(T_MSG_LOGIN) + m_struct.accountLen + m_struct.passwordLen;
}

bool CMessageLogin::WriteToOutputByteArray(OutputByteArray & out)
{
    out.WriteU16(m_struct.accountLen);
    out.WriteU16(m_struct.passwordLen);

    if (m_struct.accountLen > 0) {
        out.WriteUtf8String(m_qstrAccount);
    }

    if (m_struct.passwordLen > 0) {
        out.WriteUtf8String(m_qstrPassword);
    }

    return true;
}

bool CMessageLogin::ReadFromInputByteArray(InputByteArray & in)
{
    m_struct.accountLen = in.ReadU16();
    m_struct.passwordLen = in.ReadU16();

    if ( m_struct.accountLen > 0) {
        in.ReadUtf8String(m_struct.accountLen, m_qstrAccount);
    }

    if (m_struct.passwordLen > 0) {
        in.ReadUtf8String(m_struct.passwordLen, m_qstrPassword);
    }

    return true;
}

IMessage * CMessageLogin::CreateFromHeader(const CMessageHeader & header)
{
    CMessageLogin * pRet = NULL;

    if (header.IsValid() 
        && header.m_struct.bodyLength >= sizeof(T_MSG_LOGIN)) {
        pRet = new CMessageLogin();
    }

    return pRet;
}

QString CMessageLogin::ToString()
{
    QString qstr;
    QTextStream stream(&qstr);

    stream << "{ ";
    stream << "[ accountLen:" << m_struct.accountLen << "], ";
    stream << "[ passwordLen:" << m_struct.passwordLen << "], ";
    stream << "[ m_qstrAccount:" << m_qstrAccount << "], ";
    stream << "[ m_qstrPassword:" << m_qstrPassword << "] ";
    stream << "}";

    return qstr;
}
