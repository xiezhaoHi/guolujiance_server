#ifndef _COMMON_MESSAGE_H_
#define _COMMON_MESSAGE_H_

#include <memory>
#include "i_message.h"
#include "message_header.h"

class CMessage;
typedef std::shared_ptr<CMessage> message_ptr;

class CMessage : public IMessage
{
public:

    CMessage();

    virtual ~CMessage();

    virtual std::size_t Length();

    virtual bool WriteToOutputByteArray(OutputByteArray & out);

    virtual bool ReadFromInputByteArray(InputByteArray &);

    // ��ȡͷ��
    virtual bool ParseHeader();

    // ��ȡ��Ϣ��
    virtual bool ParseBody();

    // �ж���Ϣ�Ƿ���Ч
    bool IsValidHeader();

    // ���ö�ȡ��������
    bool AppendInputByteArray(U8 * u8array, int arrayLen);

    // ������Ϣ��
    void SetMessageBody(IMessage * pMsgBody);

    virtual QString ToString();

	/******************************************************/
	//20180122 ���� ����cmd ��head 

	void SetCMDToHead(unsigned int length 
		,unsigned int cmd = DEVICE_CMD_REALTIME_BATCHED_DATA_NEW);

	//void ParseBody_two();
private:
    // ������Ϣ�����
    bool CreateMessageBody();


public:
    CMessageHeader      m_header;
    IMessage          * m_pBody;
    InputByteArray      m_InputArray;
    static int          m_nMaxBodySize;
};

#endif//!_COMMON_MESSAGE_H_