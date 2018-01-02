#ifndef _MESSAGE_GET_DEVICE_DATA_COUNT_REP_H_
#define _MESSAGE_GET_DEVICE_DATA_COUNT_REP_H_


//typedef struct _T_MSG_GET_DEVICE_DATA_COUNT_REP
//{
//    U32     deivceDataCount;    // 设备数据的条数
//}T_MSG_GET_DEVICE_DATA_COUNT_REP;// 回复设备数据的条数

#include "message/i_message.h"

class CMessageGetDeviceDataCountRep : public IMessage
{
public:
    CMessageGetDeviceDataCountRep();

    virtual ~CMessageGetDeviceDataCountRep();

    virtual std::size_t Length();

    virtual bool WriteToOutputByteArray(OutputByteArray & out);

    virtual bool ReadFromInputByteArray(InputByteArray & in);

    virtual QString ToString();

    static IMessage * CreateFromHeader(const CMessageHeader & header);

public:
    T_MSG_GET_DEVICE_DATA_COUNT_REP m_struct;
};


#endif//!_MESSAGE_GET_DEVICE_DATA_COUNT_REP_H_