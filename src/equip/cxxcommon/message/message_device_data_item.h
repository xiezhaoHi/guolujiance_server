#ifndef _MESSAGE_DEVICE_DATA_ITEM_H_
#define _MESSAGE_DEVICE_DATA_ITEM_H_

//typedef struct _T_MSG_DEVICE_DATA_ITEM
//{
//    U32     deviceIDLen;            // 设备ID号长度
//    D64     data;                   // 设备采集到的数据
//    T_MSG_DEVICE_REALTIME_DATA detailData;// 详细数据
//    // U8 * deviceID;               // 设备ID
//}T_MSG_DEVICE_DATA_ITEM;// 设备数据详情

#include "message/i_message.h"
#include "message/message_device_realtime_data.h"

class CMessageDeviceDataItem : public IMessage
{
public:
    CMessageDeviceDataItem();

    virtual ~CMessageDeviceDataItem();

    virtual std::size_t Length();

    virtual bool WriteToOutputByteArray(OutputByteArray & out);

    virtual bool ReadFromInputByteArray(InputByteArray & in);

    virtual QString ToString();

    // 设置设备ID
    void SetDeviceID(const QString & deviceID);

    // 获取设备ID
    QString GetDeviceID();

public:
    T_MSG_DEVICE_DATA_ITEM      m_struct;
    CMessageDeviceRealtimeData  m_deviceData;
private:
    QString                m_qstrDeviceID;
};




#endif//!_MESSAGE_DEVICE_DATA_ITEM_H_