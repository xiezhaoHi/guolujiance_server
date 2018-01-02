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

    // �����豸��
    void SetDeviceName(const QString & deviceName);

    // ��ȡ�豸��
    QString GetDeviceName();

    // ���ò�����
    void SetSampleRate(const QString & sampleRate);

    // ��ȡ������
    QString GetSampleRate();

public:
    T_MSG_DEVICE_PARAM  m_struct;

private:
    QString             m_qstrDeviceName;// �豸��
    QString             m_qstrSampleRate;// ������
};

#endif//_MESSAGE_SET_DEVICE_PARAM_H_