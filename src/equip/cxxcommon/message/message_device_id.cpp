#include "message_device_id.h"



CMessageDeviceID::CMessageDeviceID()
{
    static_assert(sizeof(T_MSG_DEVICE_ID) == 36, "sizeof(T_MSG_DEVICE_ID) != 36");
    memset(&m_struct, 0x00, sizeof(T_MSG_DEVICE_ID));
}

CMessageDeviceID::~CMessageDeviceID()
{

}


std::size_t CMessageDeviceID::Length()
{
    return sizeof(T_MSG_DEVICE_ID);
}

bool CMessageDeviceID::WriteToOutputByteArray(OutputByteArray & out)
{
    U8 * pStruct = (U8 *)&m_struct;
    for (int i = 0; i < sizeof(T_MSG_DEVICE_ID); i++) {
        out.WriteU8(pStruct[i]);
    }
    return true;
}

bool CMessageDeviceID::ReadFromInputByteArray(InputByteArray & in)
{
    U8 * pStruct = (U8 *)&m_struct;
    for (int i = 0; i < sizeof(T_MSG_DEVICE_ID); i++) {
        pStruct[i] = in.ReadU8();
    }
    return true;
}

QString CMessageDeviceID::ToString()
{
    QString qstr;
    QTextStream stream(&qstr);

    stream << "[CMessageDeviceID] { ";
    stream << "[ deviceID:" << QString::fromLocal8Bit((char *)(m_struct.deviceID), sizeof(m_struct.deviceID)) << "] ";
    stream << "}";

    return qstr;
}