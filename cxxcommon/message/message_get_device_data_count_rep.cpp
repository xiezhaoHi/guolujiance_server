#include "message_get_device_data_count_rep.h"
#include "message/message_header.h"



CMessageGetDeviceDataCountRep::CMessageGetDeviceDataCountRep()
{
    static_assert(sizeof(T_MSG_GET_DEVICE_DATA_COUNT_REP) == 4, "sizeof(T_MSG_GET_DEVICE_DATA_COUNT_REP) != 4");
    memset(&m_struct, 0x00, sizeof(m_struct));
}

CMessageGetDeviceDataCountRep::~CMessageGetDeviceDataCountRep()
{

}

std::size_t CMessageGetDeviceDataCountRep::Length()
{
    return sizeof(m_struct);
}

bool CMessageGetDeviceDataCountRep::WriteToOutputByteArray(OutputByteArray & out)
{
    out.WriteU32(m_struct.deivceDataCount);
    return true;
}

bool CMessageGetDeviceDataCountRep::ReadFromInputByteArray(InputByteArray & in)
{
    m_struct.deivceDataCount = in.ReadU32();
    return true;
}

QString CMessageGetDeviceDataCountRep::ToString()
{
    QString qstr;
    QTextStream stream(&qstr);

    stream << "{ ";
    stream << "[ deivceDataCount:" << m_struct.deivceDataCount << "], ";
    stream << "}";

    return qstr;
}

IMessage * CMessageGetDeviceDataCountRep::CreateFromHeader(const CMessageHeader & header)
{
    if (header.m_struct.bodyLength >= sizeof(T_MSG_GET_DEVICE_DATA_COUNT_REP)) {
        return new CMessageGetDeviceDataCountRep();
    }
    return NULL;
}
