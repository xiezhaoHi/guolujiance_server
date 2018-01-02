/*****************************************************
** 文件名：  message_get_device_online_status_rep_item.h
** 版  权：  XXX 版权所有
** 版  本：  
** 内容简述：
** 创建日期：2015/08/05
** 创建人：  唐桥
** 修改记录：
   日期        版本      修改人    修改内容 
    
*****************************************************/
#ifndef _MESSAGE_GET_DEVICE_ONLINE_STATUS_REP_ITEM_H_
#define _MESSAGE_GET_DEVICE_ONLINE_STATUS_REP_ITEM_H_

#include "message/i_message.h"


class CMessageDeviceID;

class CMessageGetDeviceOnlineStatusRepItem : public IMessage
{
public:
    CMessageGetDeviceOnlineStatusRepItem();

    virtual ~CMessageGetDeviceOnlineStatusRepItem();

    virtual std::size_t Length();

    virtual bool WriteToOutputByteArray(OutputByteArray & out);

    virtual bool ReadFromInputByteArray(InputByteArray & in);

    virtual QString ToString();

public:
    T_MSG_DEVICE_ONLINE_STATUS_REP_ITEM m_struct;
};


#endif//!_MESSAGE_GET_DEVICE_ONLINE_STATUS_REP_ITEM_H_