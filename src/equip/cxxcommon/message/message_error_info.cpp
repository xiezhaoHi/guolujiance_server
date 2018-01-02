#include "message_error_info.h"
#include "message/message_header.h"


CMessageErrorInfo::CMessageErrorInfo()
{
    static_assert(sizeof(T_MSG_ERROR_INFO) == 4, "sizeof(T_MSG_ERROR_INFO) != 4");
    memset(&m_struct, 0x00, sizeof(T_MSG_ERROR_INFO));
}

CMessageErrorInfo::~CMessageErrorInfo()
{

}


std::size_t CMessageErrorInfo::Length()
{
    m_struct.infoBufLen = m_qstrErrorInfo.toUtf8().length();
    return sizeof(T_MSG_ERROR_INFO) + m_struct.infoBufLen;
}

bool CMessageErrorInfo::WriteToOutputByteArray(OutputByteArray & out)
{
    out.WriteU32(m_struct.infoBufLen);
    if (m_struct.infoBufLen > 0) {
        out.WriteUtf8String(m_qstrErrorInfo);
    }
    return true;
}

bool CMessageErrorInfo::ReadFromInputByteArray(InputByteArray & in)
{
    m_struct.infoBufLen = in.ReadU32();
    QByteArray buf;
    if (m_struct.infoBufLen > 0) {
        in.ReadUtf8String(m_struct.infoBufLen, m_qstrErrorInfo);
    }
    return true;
}

QString CMessageErrorInfo::ToString()
{
    QString qstr;
    QTextStream stream(&qstr);

    stream << "{ ";
    stream << "[ infoBufLen:" << m_struct.infoBufLen << "], ";
    stream << "[ ErrorInfo:" << m_qstrErrorInfo << "] ";
    stream << "}";

    return qstr;
}


IMessage * CMessageErrorInfo::CreateFromHeader(const CMessageHeader & header)
{
    if (header.m_struct.bodyLength >= sizeof(T_MSG_ERROR_INFO)) {
        return new CMessageErrorInfo();
    }
    return NULL;
}
