/*****************************************************
** 文件名：  message_broadcast_device_online_status.h
** 版  权：  XXX 版权所有
** 版  本：  
** 内容简述：设备在线状态广播消息
** 创建日期：2015/08/05
** 创建人：  唐桥
** 修改记录：
   日期        版本      修改人    修改内容 
    
*****************************************************/
#ifndef _MESSAGE_BROADCAST_DEVICE_ONLINE_STATUS_H_
#define _MESSAGE_BROADCAST_DEVICE_ONLINE_STATUS_H_

#include "message/message_get_device_online_status_rep.h"

class CMessageBroadcastDeviceOnlineStatus : public CMessageGetDeviceOnlineStatusRep
{
public:
    CMessageBroadcastDeviceOnlineStatus();

    virtual ~CMessageBroadcastDeviceOnlineStatus();

    virtual std::size_t Length();

    virtual bool WriteToOutputByteArray(OutputByteArray & out);

    virtual bool ReadFromInputByteArray(InputByteArray & in);

    virtual QString ToString();

    static IMessage * CreateFromHeader(const CMessageHeader & header);
};

#endif//!_MESSAGE_BROADCAST_DEVICE_ONLINE_STATUS_H_