#ifndef _MESSAGE_SET_DEVICE_PARAM_H_
#define _MESSAGE_SET_DEVICE_PARAM_H_

#include "message/i_message.h"
#include "message/message_device_id.h"

class CMessageSetDeviceParam : public IMessage
{
public:
    CMessageSetDeviceParam();

    virtual ~CMessageSetDeviceParam();

    virtual std::size_t Length();

    virtual bool WriteToOutputByteArray(OutputByteArray & out);

    virtual bool ReadFromInputByteArray(InputByteArray & in);

    virtual QString ToString();

    static IMessage * CreateFromHeader(const CMessageHeader & header);

    // 设置设备名
    void SetDeviceName(const QString & deviceName);

    // 获取设备名
    QString GetDeviceName();

    // 设置采样率
    void SetSampleRate(const QString & sampleRate);

    // 获取采样率
    QString GetSampleRate();

public:
    T_MSG_DEVICE_PARAM  m_struct;

private:
    QString             m_qstrDeviceName;// 设备名
    QString             m_qstrSampleRate;// 采样率
};

#endif//_MESSAGE_SET_DEVICE_PARAM_H_