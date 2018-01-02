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

    // 读取头部
    virtual bool ParseHeader();

    // 读取消息体
    virtual bool ParseBody();

    // 判断消息是否有效
    bool IsValidHeader();

    // 设置读取到的数据
    bool AppendInputByteArray(U8 * u8array, int arrayLen);

    // 设置消息体
    void SetMessageBody(IMessage * pMsgBody);

    virtual QString ToString();

private:
    // 创建消息体对象
    bool CreateMessageBody();


public:
    CMessageHeader      m_header;
    IMessage          * m_pBody;
    InputByteArray      m_InputArray;
    static int          m_nMaxBodySize;
};

#endif//!_COMMON_MESSAGE_H_