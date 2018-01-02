#include "message_device_data_item.h"




CMessageDeviceDataItem::CMessageDeviceDataItem()
{
    static_assert(sizeof(T_MSG_DEVICE_DATA_ITEM) == 12, "sizeof(T_MSG_DEVICE_DATA_ITEM) != 12");
    memset(&m_struct, 0x00, sizeof(m_struct));
}

CMessageDeviceDataItem::~CMessageDeviceDataItem()
{

}

std::size_t CMessageDeviceDataItem::Length()
{
    m_struct.deviceIDLen = m_qstrDeviceID.toUtf8().length();
    return sizeof(m_struct) + m_deviceData.Length() + m_struct.deviceIDLen;
}

bool CMessageDeviceDataItem::WriteToOutputByteArray(OutputByteArray & out)
{
    out.WriteD64(m_struct.data);
    out.WriteU32(m_struct.deviceIDLen);
    m_deviceData.WriteToOutputByteArray(out);
    if (m_struct.deviceIDLen > 0)
    {
        out.WriteUtf8String(m_qstrDeviceID);
    }
    return true;
}

bool CMessageDeviceDataItem::ReadFromInputByteArray(InputByteArray & in)
{
    m_struct.data = in.ReadD64();
    m_struct.deviceIDLen = in.ReadU32();
    m_deviceData.ReadFromInputByteArray(in);
    if (m_struct.deviceIDLen > 0)
    {
        in.ReadUtf8String(m_struct.deviceIDLen, m_qstrDeviceID);
    }
    return true;
}

QString CMessageDeviceDataItem::ToString()
{
    QString qstr;
    QTextStream stream(&qstr);

    stream << "{ ";
    stream << "[ deviceIDLen:" << m_struct.deviceIDLen << "], ";
    stream << "[ data:" << m_struct.data << "] ";
    stream << "[ detailData:" << m_deviceData.ToString() << "] ";
    stream << "}";

    return qstr;
}

void CMessageDeviceDataItem::SetDeviceID(const QString & deviceID)
{
    m_qstrDeviceID = deviceID;
    m_struct.deviceIDLen = deviceID.toUtf8().length();
}

QString CMessageDeviceDataItem::GetDeviceID()
{
    return m_qstrDeviceID;
}
