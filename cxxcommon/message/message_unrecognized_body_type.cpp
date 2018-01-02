#include "message_unrecognized_body_type.h"


CMessageUnrecognizedBodyType::CMessageUnrecognizedBodyType()
{
    m_bodyDataLen = 0;
}

CMessageUnrecognizedBodyType::~CMessageUnrecognizedBodyType()
{
}

std::size_t CMessageUnrecognizedBodyType::Length()
{
    m_bodyDataLen = m_bodyData.length();
    return m_bodyDataLen;
}

bool CMessageUnrecognizedBodyType::WriteToOutputByteArray(OutputByteArray & out)
{
    out.WriteBytes(m_bodyData, m_bodyDataLen);
    return true;
}

bool CMessageUnrecognizedBodyType::ReadFromInputByteArray(InputByteArray & in)
{
    m_bodyData.clear();
    in.ReadBytes(m_bodyDataLen, m_bodyData);
    return true;
}

QString CMessageUnrecognizedBodyType::ToString()
{
    QString qstr;
    QTextStream stream(&qstr);

    stream << "[CMessageUnrecognizedBodyType] { ";
    stream << "[ bodyLen:" << m_bodyDataLen << "], ";
    stream << "[ bodyString:" << QString(m_bodyData) << "], ";
    stream << "[ bodyRawData:";
    for (int i = 0; i < m_bodyDataLen; i++) {
        stream << m_bodyData[i] << " ";
        if ((i + 1) % 8 == 0) {
            stream << " ";
        }
    }
    stream << "] ";
    stream << "}";

    return qstr;
}

IMessage * CMessageUnrecognizedBodyType::CreateFromHeader(const CMessageHeader & header)
{
    return new CMessageUnrecognizedBodyType();
}
