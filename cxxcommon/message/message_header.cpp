#include "message_header.h"
#include <stdexcept>
#include <memory.h>
#include <QTextStream>


CMessageHeader::CMessageHeader()
{
    static_assert(sizeof(T_MESSAGE_HEADER) == 24, "sizeof(T_MESSAGE_HEADER) != 24");
    memset(&m_struct, 0, sizeof(T_MESSAGE_HEADER));
    m_struct.protocolVersion = PROTOCOL_VERSION;
    m_struct.protocolVerifyBits = PROTOCOL_VERIFY_BITS;
}

CMessageHeader::~CMessageHeader()
{

}

std::size_t CMessageHeader::Length()
{
    return sizeof(T_MESSAGE_HEADER);
}

bool CMessageHeader::WriteToOutputByteArray(OutputByteArray & out)
{
    out.WriteU32(m_struct.cmd);
    out.WriteU32(m_struct.protocolVersion);
    out.WriteU32(m_struct.protocolVerifyBits);
    out.WriteU32(m_struct.messageNo);
    out.WriteU32(m_struct.cmdResult);
    out.WriteU32(m_struct.bodyLength);
    return true;
}

bool CMessageHeader::ReadFromInputByteArray(InputByteArray & in)
{
    m_struct.cmd = in.ReadU32();
    m_struct.protocolVersion = in.ReadU32();
    m_struct.protocolVerifyBits = in.ReadU32();
    m_struct.messageNo = in.ReadU32();
    m_struct.cmdResult = in.ReadU32();
    m_struct.bodyLength = in.ReadU32();

    return true;
}

bool CMessageHeader::IsValid() const
{
    // 作协议版本号与协议版本校验位的检查
    if ((m_struct.protocolVersion ^ m_struct.protocolVerifyBits) != 0xFFFFFFFF 
        || m_struct.protocolVersion != PROTOCOL_VERSION){
        return false;
    }
    return true;
}

QString CMessageHeader::ToString()
{
    QString qstr;
    QTextStream stream(&qstr);
    stream << "[CMessageHeader] { ";
    stream << "[ cmd:" << m_struct.cmd << "], ";
    stream << "[ protocolVersion:" << m_struct.protocolVersion << "], ";
    stream << "[ protocolVerifyBits:" << m_struct.protocolVerifyBits << "], ";
    stream << "[ messageNo:" << m_struct.messageNo << "], ";
    stream << "[ cmdResult:" << m_struct.cmdResult << "], ";
    stream << "[ bodyLength:" << m_struct.bodyLength << "] ";
    stream << "}";
 
    return qstr;
}
