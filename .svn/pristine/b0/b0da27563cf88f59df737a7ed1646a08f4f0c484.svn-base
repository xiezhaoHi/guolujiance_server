#ifndef _MESSAGE_GET_DEVICE_DATA_REP_H_
#define _MESSAGE_GET_DEVICE_DATA_REP_H_


//typedef struct _T_MSG_GET_DEVICE_DATA_REP
//{
//    U32     pageIndex;              // 当前数据的分页号
//    U32     dataCountOfCurrentPage; // 当前页的数据条数
//    //T_MSG_DEVICE_DATA_ITEM * deviceDataArray;// 设备采集到的数据
//}T_MSG_GET_DEVICE_DATA_REP;// 回复

#include "message/i_message.h"
#include "message/message_device_data_item.h"
#include <QList>

class CMessageGetDeviceDataRep : public IMessage
{
public:
    CMessageGetDeviceDataRep();

    virtual ~CMessageGetDeviceDataRep();

    virtual std::size_t Length();

    virtual bool WriteToOutputByteArray(OutputByteArray & out);

    virtual bool ReadFromInputByteArray(InputByteArray & in);

    virtual QString ToString();

    static IMessage * CreateFromHeader(const CMessageHeader & header);

public:
    T_MSG_GET_DEVICE_DATA_REP           m_struct;
    QList<CMessageDeviceDataItem *>     m_lstDeviceData; 
};


#endif//!_MESSAGE_GET_DEVICE_DATA_REP_H_