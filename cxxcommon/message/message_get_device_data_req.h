#ifndef _MESSAGE_GET_DEVICE_DATA_REQ_H_
#define _MESSAGE_GET_DEVICE_DATA_REQ_H_


//typedef struct _T_MSG_GET_DEVICE_DATA_REQ
//{
//    U64     begin;              // ��ʼʱ��
//    U64     end;                // ����ʱ��
//    U32     pageIndex;          // ��ҳ��,��1��ʼ����
//    U32     dataCountOfPerPage; // ÿһҳ����������������20��50��100
//    U32     deviceIDLen;        // �豸ID����
//    // U8 * deviceID;           // �豸ID��Ϊ��ʱ��ʾ���û����ھ��ӵ������豸
//}T_MSG_GET_DEVICE_DATA_REQ;// ��ȡ�豸���ݵ�����

#include "message/i_message.h"

class CMessageGetDeviceDataReq : public IMessage
{
public:
    CMessageGetDeviceDataReq();

    virtual ~CMessageGetDeviceDataReq();

    virtual std::size_t Length();

    virtual bool WriteToOutputByteArray(OutputByteArray & out);

    virtual bool ReadFromInputByteArray(InputByteArray & in);

    virtual QString ToString();

    static IMessage * CreateFromHeader(const CMessageHeader & header);

    // �����豸ID
    void SetDeviceID(const QString & deviceID);

    // ��ȡ�豸ID
    QString GetDeviceID();

public:
    T_MSG_GET_DEVICE_DATA_REQ   m_struct;

private:
    QString                     m_qstrDeviceID;// �豸ID
};


#endif//!_MESSAGE_GET_DEVICE_DATA_REQ_H_