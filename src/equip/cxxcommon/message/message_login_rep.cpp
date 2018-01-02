#include "message_login_rep.h"


CMessageLoginRep::CMessageLoginRep()
{
    static_assert(sizeof(T_MSG_SERVER_TIME) == 8, "sizeof(T_MSG_SERVER_TIME) != 8");
    memset(&m_struct, 0x00, sizeof(T_MSG_SERVER_TIME));
}

CMessageLoginRep::~CMessageLoginRep()
{

}


std::size_t CMessageLoginRep::Length()
{
    return sizeof(T_MSG_SERVER_TIME);
}

bool CMessageLoginRep::WriteToOutputByteArray(OutputByteArray & out)
{
    out.WriteU64(m_struct.serverTime);
    return true;
}

bool CMessageLoginRep::ReadFromInputByteArray(InputByteArray & in)
{
    m_struct.serverTime = in.ReadU64();
    return true;
}

QString CMessageLoginRep::ToString()
{
    QString qstr;
    QTextStream stream(&qstr);

    stream << "{ ";
    stream << "[ serverTime:" << m_struct.serverTime << "] ";
    stream << "}";

    return qstr;
}

IMessage * CMessageLoginRep::CreateFromHeader(const CMessageHeader & header)
{
    if (PC_SERVER_CMD_LOGIN_REP == header.m_struct.cmd) {
        return new CMessageLoginRep();
    }
    return NULL;
}

