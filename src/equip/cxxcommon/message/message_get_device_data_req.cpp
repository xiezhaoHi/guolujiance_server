#include "message_get_device_data_req.h"
#include "message/message_header.h"



CMessageGetDeviceDataReq::CMessageGetDeviceDataReq()
{
    static_assert(sizeof(T_MSG_GET_DEVICE_DATA_REQ) == 32, "sizeof(T_MSG_GET_DEVICE_DATA_REQ) != 32");
    memset(&m_struct, 0x00, sizeof(m_struct));
}

CMessageGetDeviceDataReq::~CMessageGetDeviceDataReq()
{

}

std::size_t CMessageGetDeviceDataReq::Length()
{
    m_struct.deviceIDLen = m_qstrDeviceID.toUtf8().length();
    return sizeof(m_struct) + m_struct.deviceIDLen;
}

bool CMessageGetDeviceDataReq::WriteToOutputByteArray(OutputByteArray & out)
{
    out.WriteU64(m_struct.begin);
    out.WriteU64(m_struct.end);
    out.WriteU32(m_struct.pageIndex);
    out.WriteU32(m_struct.rowsOfPerPage);
    out.WriteU32(m_struct.deviceIDLen);
    out.WriteU32(m_struct.reserved);
    if (m_struct.deviceIDLen > 0)
    {
        out.WriteUtf8String(m_qstrDeviceID);
    }
    return true;
}

bool CMessageGetDeviceDataReq::ReadFromInputByteArray(InputByteArray & in)
{
    m_struct.begin = in.ReadU64();
    m_struct.end = in.ReadU64();
    m_struct.pageIndex = in.ReadU32();
    m_struct.rowsOfPerPage = in.ReadU32();
    m_struct.deviceIDLen = in.ReadU32();
    m_struct.reserved = in.ReadU32();
    if (m_struct.deviceIDLen > 0)
    {
        in.ReadUtf8String(m_struct.deviceIDLen, m_qstrDeviceID);
    }
    return true;
}

QString CMessageGetDeviceDataReq::ToString()
{
    QString qstr;
    QTextStream stream(&qstr);

    stream << "{ ";
    stream << "[ begin:" << m_struct.begin << "], ";
    stream << "[ end:" << m_struct.end << "], ";
    stream << "[ pageIndex:" << m_struct.pageIndex << "], ";
    stream << "[ dataCountOfPerPage:" << m_struct.rowsOfPerPage << "], ";
    stream << "[ deviceIDLen:" << m_struct.deviceIDLen << "], ";
    stream << "[ deviceID:" << m_qstrDeviceID << "] ";
    stream << "}";

    return qstr;
}

IMessage * CMessageGetDeviceDataReq::CreateFromHeader(const CMessageHeader & header)
{
    if (header.m_struct.bodyLength >= sizeof(T_MSG_GET_DEVICE_DATA_REQ)) {
        return new CMessageGetDeviceDataReq();
    }
    return NULL;
}

void CMessageGetDeviceDataReq::SetDeviceID(const QString & deviceID)
{
    m_qstrDeviceID = deviceID;
    m_struct.deviceIDLen = deviceID.toUtf8().length();
}

QString CMessageGetDeviceDataReq::GetDeviceID()
{
    return m_qstrDeviceID;
}
