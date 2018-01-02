/*****************************************************
** 文件名：  message_get_device_online_status_rep_item.cpp
** 版  权：  XXX 版权所有
** 版  本：  
** 内容简述：
** 创建日期：2015/08/05
** 创建人：  唐桥
** 修改记录：
   日期        版本      修改人    修改内容 
    
*****************************************************/
#include "message_get_device_online_status_rep_item.h"




CMessageGetDeviceOnlineStatusRepItem::CMessageGetDeviceOnlineStatusRepItem()
{
    static_assert(sizeof(T_MSG_DEVICE_ONLINE_STATUS_REP_ITEM) == 37, "sizeof(T_MSG_DEVICE_ONLINE_STATUS_REP_ITEM) != 37");
    memset(&m_struct, 0x00, sizeof(T_MSG_DEVICE_ONLINE_STATUS_REP_ITEM));
}

CMessageGetDeviceOnlineStatusRepItem::~CMessageGetDeviceOnlineStatusRepItem()
{

}

std::size_t CMessageGetDeviceOnlineStatusRepItem::Length()
{
    return sizeof(T_MSG_DEVICE_ONLINE_STATUS_REP_ITEM);
}

bool CMessageGetDeviceOnlineStatusRepItem::WriteToOutputByteArray(OutputByteArray & out)
{
    U8 * pStruct = (U8 *)&m_struct;
    for (int i = 0; i < sizeof(m_struct); i++) {
        out.WriteU8(pStruct[i]);
    }
    return true;
}

bool CMessageGetDeviceOnlineStatusRepItem::ReadFromInputByteArray(InputByteArray & in)
{
    U8 * pStruct = (U8 *)&m_struct;
    for (int i = 0; i < sizeof(m_struct); i++) {
        pStruct[i] = in.ReadU8();
    }
    return true;
}

QString CMessageGetDeviceOnlineStatusRepItem::ToString()
{
    QString qstr;
    QTextStream stream(&qstr);

    stream << "[CMessageGetDeviceOnlineStatusRepItem] { ";
    stream << "[ deviceID:" << QString::fromLocal8Bit((char *)(m_struct.deviceID), sizeof(m_struct.deviceID)).trimmed() << "], ";
    stream << "[ deviceOnlineStatus:" << m_struct.deviceOnlineStatus << "] ";
    stream << "}";

    return qstr;
}
