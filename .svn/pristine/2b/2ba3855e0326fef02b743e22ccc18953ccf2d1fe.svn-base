#include "message_get_device_data_count_req.h"
#include "message/message_header.h"



CMessageGetDeviceDataCountReq::CMessageGetDeviceDataCountReq()
{
    static_assert(sizeof(T_MSG_GET_DEVICE_DATA_COUNT_REQ) == 24, "sizeof(T_MSG_GET_DEVICE_DATA_COUNT_REQ) != 24");
    memset(&m_struct, 0x00, sizeof(m_struct));
}

CMessageGetDeviceDataCountReq::~CMessageGetDeviceDataCountReq()
{

}

std::size_t CMessageGetDeviceDataCountReq::Length()
{
    m_struct.deviceIDLen = m_qstrDeviceID.toUtf8().length();
    return sizeof(m_struct) + m_struct.deviceIDLen;
}

bool CMessageGetDeviceDataCountReq::WriteToOutputByteArray(OutputByteArray & out)
{
    out.WriteU64(m_struct.begin);
    out.WriteU64(m_struct.end);
    out.WriteU32(m_struct.deviceIDLen);
    out.WriteU32(m_struct.reserved);
    if (m_struct.deviceIDLen > 0)
    {
        out.WriteUtf8String(m_qstrDeviceID);
    }
    return true;
}

bool CMessageGetDeviceDataCountReq::ReadFromInputByteArray(InputByteArray & in)
{
    m_struct.begin = in.ReadU64();
    m_struct.end   = in.ReadU64();
    m_struct.deviceIDLen = in.ReadU32();
    m_struct.reserved = in.ReadU32();
    if (m_struct.deviceIDLen > 0)
    {
        in.ReadUtf8String(m_struct.deviceIDLen, m_qstrDeviceID);
    }
    return true;
}

QString CMessageGetDeviceDataCountReq::ToString()
{
    QString qstr;
    QTextStream stream(&qstr);

    stream << "{ ";
    stream << "[ begin:" << m_struct.begin << "], ";
    stream << "[ end:" << m_struct.end << "], ";
    stream << "[ deviceIDLen:" << m_struct.deviceIDLen << "], ";
    stream << "[ deviceID:" << m_qstrDeviceID << "] ";
    stream << "}";

    return qstr;
}

IMessage * CMessageGetDeviceDataCountReq::CreateFromHeader(const CMessageHeader & header)
{
    if (header.m_struct.bodyLength >= sizeof(T_MSG_GET_DEVICE_DATA_COUNT_REQ)) {
        return new CMessageGetDeviceDataCountReq();
    }
    return NULL;
}

void CMessageGetDeviceDataCountReq::SetDeviceID(const QString & deviceID)
{
    m_qstrDeviceID = deviceID;
    m_struct.deviceIDLen = deviceID.toUtf8().length();
}

QString CMessageGetDeviceDataCountReq::GetDeviceID()
{
    return m_qstrDeviceID;
}
