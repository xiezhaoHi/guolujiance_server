#include "message_device_item_info.h"

CMessageDeviceItemInfo::CMessageDeviceItemInfo()
{
    static_assert(sizeof(T_MSG_DEVICE_INFO_LIST_ITEM) == 1093, "sizeof(T_MSG_DEVICE_INFO_LIST_ITEM) != 1093");
    memset(&m_struct, 0x00, sizeof(T_MSG_DEVICE_INFO_LIST_ITEM));
}

CMessageDeviceItemInfo::~CMessageDeviceItemInfo()
{
}

std::size_t CMessageDeviceItemInfo::Length()
{
    std::size_t len = sizeof(T_MSG_DEVICE_INFO_LIST_ITEM);
    return len;
}

bool CMessageDeviceItemInfo::WriteToOutputByteArray(OutputByteArray & out)
{
    U8 * pStruct = (U8 *)&m_struct;
    for (int i = 0; i < sizeof(m_struct); i++) {
        out.WriteU8(pStruct[i]);
    }
    return true;
}

bool CMessageDeviceItemInfo::ReadFromInputByteArray(InputByteArray & in)
{
    U8 * pStruct = (U8 *)&m_struct;
    for (int i = 0; i < sizeof(m_struct); i++) {
        pStruct[i] = in.ReadU8();
    }
    return true;
}

QString CMessageDeviceItemInfo::ToString()
{
    QString qstr;
    QTextStream stream(&qstr);

    stream << "[CMessageDeviceItemInfo] { ";
    stream << "[ deviceId:" << QString::fromLocal8Bit((char *)m_struct.deviceId, sizeof(m_struct.deviceId)).trimmed() << "], ";
    stream << "[ deviceNo:" << QString::fromLocal8Bit((char *)m_struct.deviceNo, sizeof(m_struct.deviceNo)).trimmed() << "], ";
    stream << "[ deviceName:" << QString::fromLocal8Bit((char *)m_struct.deviceName, sizeof(m_struct.deviceName)).trimmed() << "], ";
    stream << "[ deviceModel:" << QString::fromLocal8Bit((char *)m_struct.deviceModel, sizeof(m_struct.deviceModel)).trimmed() << "], ";
    stream << "[ deviceState:" << QString::fromLocal8Bit((char *)m_struct.deviceState, sizeof(m_struct.deviceState)).trimmed() << "], ";
    stream << "[ deviceManufacturers:" << QString::fromLocal8Bit((char *)m_struct.deviceManufacturers, sizeof(m_struct.deviceManufacturers)).trimmed() << "], ";
    stream << "[ deviceMainParams:" << QString::fromLocal8Bit((char *)m_struct.deviceMainParams, sizeof(m_struct.deviceMainParams)).trimmed() << "], ";
    stream << "[ deviceRigName:" << QString::fromLocal8Bit((char *)m_struct.deviceRigName, sizeof(m_struct.deviceRigName)).trimmed() << "], ";
    stream << "[ deviceCompany:" << QString::fromLocal8Bit((char *)m_struct.deviceCompany, sizeof(m_struct.deviceCompany)).trimmed() << "], ";
    stream << "[ deviceAddress:" << QString::fromLocal8Bit((char *)m_struct.deviceAddress, sizeof(m_struct.deviceAddress)).trimmed() << "], ";
    stream << "}";

    return qstr;
}


