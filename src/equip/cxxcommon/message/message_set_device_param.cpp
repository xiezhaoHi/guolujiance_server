#include "message_set_device_param.h"
#include "message/message_header.h"



CMessageSetDeviceParam::CMessageSetDeviceParam()
{
    static_assert(sizeof(T_MSG_DEVICE_PARAM) == 196, "sizeof(T_MSG_DEVICE_PARAM) != 186");
    memset(&m_struct, 0x00, sizeof(T_MSG_DEVICE_PARAM));
}

CMessageSetDeviceParam::~CMessageSetDeviceParam()
{

}

std::size_t CMessageSetDeviceParam::Length()
{
    return sizeof(T_MSG_DEVICE_PARAM) + m_struct.deviceNameLen + m_struct.sampleRateLen;
}

bool CMessageSetDeviceParam::WriteToOutputByteArray(OutputByteArray & out)
{
    U8 * pStruct = (U8 *)&m_struct;
    for (int i = 0; i < sizeof(m_struct) - 2 * sizeof(U32); i++) {
        out.WriteU8(pStruct[i]);
    }

    out.WriteU32(m_struct.deviceNameLen);
    out.WriteU32(m_struct.sampleRateLen);

    if (m_struct.deviceNameLen > 0) {
        out.WriteUtf8String(m_qstrDeviceName);
    }
    if (m_struct.sampleRateLen > 0) {
        out.WriteUtf8String(m_qstrSampleRate);
    }
    return true;
}

bool CMessageSetDeviceParam::ReadFromInputByteArray(InputByteArray & in)
{
    U8 * pStruct = (U8 *)&m_struct;
    for (int i = 0; i < sizeof(m_struct) - 2 * sizeof(U32); i++) {
        pStruct[i] = in.ReadU8();
    }

    m_struct.deviceNameLen = in.ReadU32();
    m_struct.sampleRateLen = in.ReadU32();

    if (m_struct.deviceNameLen > 0) {
        in.ReadUtf8String(m_struct.deviceNameLen, m_qstrDeviceName);
    }
    if (m_struct.sampleRateLen > 0) {
        in.ReadUtf8String(m_struct.sampleRateLen, m_qstrSampleRate);
    }
    return true;
}

QString CMessageSetDeviceParam::ToString()
{
    QString qstr;
    QTextStream stream(&qstr);

    stream << "[CMessageSetDeviceParam]{ ";
    stream << "[ deviceID:" << QString::fromLocal8Bit((char *)(m_struct.deviceID), sizeof(m_struct.deviceID)).trimmed() << "] ";
    stream << "[ deviceName:" << m_qstrDeviceName << "] ";
    stream << "[ warningLine1st:" << QString::fromLocal8Bit((char *)(m_struct.warningLine1st), sizeof(m_struct.warningLine1st)).trimmed() << "] ";
    stream << "[ warningLine2nd:" << QString::fromLocal8Bit((char *)(m_struct.warningLine2nd), sizeof(m_struct.warningLine2nd)).trimmed() << "] ";
    stream << "[ maxRange:" << QString::fromLocal8Bit((char *)(m_struct.maxRange), sizeof(m_struct.maxRange)).trimmed() << "] ";
    stream << "[ sampleRate:" << m_qstrDeviceName << "] ";
    stream << "}";

    return qstr;
}

IMessage * CMessageSetDeviceParam::CreateFromHeader(const CMessageHeader & header)
{
    if (header.m_struct.bodyLength >= sizeof(T_MSG_DEVICE_PARAM)) {
        return new CMessageSetDeviceParam();
    }
    return NULL;
}

void CMessageSetDeviceParam::SetDeviceName(const QString & deviceName)
{
    m_qstrDeviceName = deviceName;
    m_struct.deviceNameLen = deviceName.toUtf8().length();
}

QString CMessageSetDeviceParam::GetDeviceName()
{
    return m_qstrDeviceName;
}

void CMessageSetDeviceParam::SetSampleRate(const QString & sampleRate)
{
    m_qstrSampleRate = sampleRate;
    m_struct.sampleRateLen = sampleRate.toUtf8().length();
}

QString CMessageSetDeviceParam::GetSampleRate()
{
    return m_qstrSampleRate;
}
