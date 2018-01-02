#include "message_server_time_beijing.h"
#include "message/message_header.h"



CMessageServerTimeBeiJing::CMessageServerTimeBeiJing()
{
    static_assert(sizeof(T_MSG_SERVER_TIME_BEIJING) == 8, "sizeof(T_MSG_SERVER_TIME_BEIJING) != 8");
    memset(&m_struct, 0x00, sizeof(m_struct));
}

CMessageServerTimeBeiJing::~CMessageServerTimeBeiJing()
{

}

std::size_t CMessageServerTimeBeiJing::Length()
{
    return sizeof(m_struct);
}

bool CMessageServerTimeBeiJing::WriteToOutputByteArray(OutputByteArray & out)
{
    out.WriteU16(m_struct.year);
    out.WriteU8(m_struct.month);
    out.WriteU8(m_struct.day);
    out.WriteU8(m_struct.hour);
    out.WriteU8(m_struct.minute);
    out.WriteU8(m_struct.second);
    out.WriteU8(m_struct.reserved);
    return true;
}

bool CMessageServerTimeBeiJing::ReadFromInputByteArray(InputByteArray & in)
{
    m_struct.year = in.ReadU16();
    m_struct.month = in.ReadU8();
    m_struct.day = in.ReadU8();
    m_struct.hour = in.ReadU8();
    m_struct.minute = in.ReadU8();
    m_struct.second = in.ReadU8();
    m_struct.reserved = in.ReadU8();
    return true;
}

IMessage * CMessageServerTimeBeiJing::CreateFromHeader(const CMessageHeader & header)
{
    CMessageServerTimeBeiJing * pRet = NULL;

    if (header.IsValid() 
        && header.m_struct.bodyLength >= sizeof(T_MSG_SERVER_TIME_BEIJING)) {
            pRet = new CMessageServerTimeBeiJing();
    }

    return pRet;
}

QString CMessageServerTimeBeiJing::ToString()
{
    QString qstr;
    QTextStream stream(&qstr);

    stream << "{ ";
    stream << "[ year:" << m_struct.year << "], ";
    stream << "[ month:" << m_struct.month << "], ";
    stream << "[ day:" << m_struct.day << "], ";
    stream << "[ hour:" << m_struct.hour << "] ";
    stream << "[ minute:" << m_struct.minute << "] ";
    stream << "[ second:" << m_struct.second << "] ";
    stream << "[ amOrPm:" << m_struct.reserved << "] ";
    stream << "}";

    return qstr;
}
