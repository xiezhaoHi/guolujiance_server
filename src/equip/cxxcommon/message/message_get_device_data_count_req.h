#ifndef _MESSAGE_GET_DEVICE_DATA_COUNT_REQ_H_
#define _MESSAGE_GET_DEVICE_DATA_COUNT_REQ_H_


//typedef struct _T_MSG_GET_DEVICE_DATA_COUNT_REQ
//{
//    U64     begin;              // 开始时间
//    U64     end;                // 结束时间
//    U32     deviceIDLen;        // 设备ID长度
//    // U8 * deviceID;           // 设备ID，为空时表示本用户所在井队的所有设备
//}T_MSG_GET_DEVICE_DATA_COUNT_REQ;// 获取设备数据的总条数

#include "message/i_message.h"

class CMessageGetDeviceDataCountReq : public IMessage
{
public:
    CMessageGetDeviceDataCountReq();

    virtual ~CMessageGetDeviceDataCountReq();

    virtual std::size_t Length();

    virtual bool WriteToOutputByteArray(OutputByteArray & out);

    virtual bool ReadFromInputByteArray(InputByteArray & in);

    virtual QString ToString();

    static IMessage * CreateFromHeader(const CMessageHeader & header);

    // 设置设备ID
    void SetDeviceID(const QString & deviceID);

    // 获取设备ID
    QString GetDeviceID();

public:
    T_MSG_GET_DEVICE_DATA_COUNT_REQ m_struct;

private:
    QString                         m_qstrDeviceID;// 设备ID
};



#endif//!_MESSAGE_GET_DEVICE_DATA_COUNT_REQ_H_