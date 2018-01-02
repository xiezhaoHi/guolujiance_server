#include "message_devices_list_item.h"



CMessageDevicesListItem::CMessageDevicesListItem()
{
    static_assert(sizeof(T_MSG_DEVICES_LIST_ITEM) == 268, "sizeof(T_MSG_DEVICES_LIST_ITEM) != 263");
    memset(&m_struct, 0x00, sizeof(T_MSG_DEVICES_LIST_ITEM));
}

CMessageDevicesListItem::~CMessageDevicesListItem()
{
}

std::size_t CMessageDevicesListItem::Length()
{
    m_struct.sampleRateLen = m_qstrSampleRate.toLatin1().length();
    std::size_t len = sizeof(T_MSG_DEVICES_LIST_ITEM) + m_struct.sampleRateLen;

    return len;
}

bool CMessageDevicesListItem::WriteToOutputByteArray(OutputByteArray & out)
{
    U8 * pStruct = (U8 *)&m_struct;
    for (int i = 0; i < sizeof(m_struct) - sizeof(U32); i++) {
        out.WriteU8(pStruct[i]);
    }
    out.WriteU32(m_struct.sampleRateLen);
    if (m_struct.sampleRateLen > 0) {
        out.WriteUtf8String(m_qstrSampleRate);
    }
    return true;
}

bool CMessageDevicesListItem::ReadFromInputByteArray(InputByteArray & in)
{
    U8 * pStruct = (U8 *)&m_struct;
    for (int i = 0; i < sizeof(m_struct) - sizeof(U32); i++) {
        pStruct[i] = in.ReadU8();
    }
    m_struct.sampleRateLen = in.ReadU32();
    if (m_struct.sampleRateLen > 0) {
        in.ReadUtf8String(m_struct.sampleRateLen, m_qstrSampleRate);
    }
    return true;
}

QString CMessageDevicesListItem::ToString()
{
    QString qstr;
    QTextStream stream(&qstr);

    stream << "[CMessageDevicesListItem] { ";
    stream << "[ deviceId:" << QString::fromLocal8Bit((char *)&m_struct.deviceId, 36).trimmed() << "], ";
    stream << "[ deviceNo:" << QString::fromLocal8Bit((char *)&m_struct.deviceNo, 36).trimmed() << "], ";
    stream << "[ deviceName:" << QString::fromLocal8Bit((char *)&m_struct.deviceName, 36).trimmed() << "], ";
    stream << "[ warningLine1st:" << QString::fromLocal8Bit((char *)&m_struct.warningLine1st, 36).trimmed() << "], ";
    stream << "[ warningLine2nd:" << QString::fromLocal8Bit((char *)&m_struct.warningLine2nd, 36).trimmed() << "], ";
    stream << "[ maxRange:" << QString::fromLocal8Bit((char *)&m_struct.maxRange, 36).trimmed() << "], ";
    stream << "[ maxRange:" << m_struct.devType << "], ";
    stream << "[ sampleRate:" << m_qstrSampleRate << "], ";
    stream << "}";

    return qstr;
}

void CMessageDevicesListItem::SetSampleRate(QString sampleRate)
{
    m_qstrSampleRate = sampleRate;
    m_struct.sampleRateLen = sampleRate.toLatin1().length();
}

QString CMessageDevicesListItem::GetSampleRate()
{
    return m_qstrSampleRate;
}
