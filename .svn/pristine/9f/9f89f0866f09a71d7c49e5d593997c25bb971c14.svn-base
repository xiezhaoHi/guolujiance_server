#ifndef _I_MESSAGE_H_
#define _I_MESSAGE_H_

#include <cstdio>
#include <cstdlib>
#include <QString>
#include <QTextStream>
#include <memory>
#include "protocol/protocol_cmd.h"
#include "protocol/protocol_structs.h"
#include "io_bytearray.h"

class IMessage
{
public:

    IMessage() {};

    virtual ~IMessage() {};

    // 数据的长度
    virtual std::size_t Length() = 0;

    // 将消息内容写到输出字节数组对象
    virtual bool WriteToOutputByteArray(OutputByteArray & out) = 0;

    // 从输入字节数组对象中读出数据，赋值到消息内容中
    virtual bool ReadFromInputByteArray(InputByteArray & in) = 0;

    virtual QString ToString() { return QStringLiteral("not implemented"); };
};

class CMessageHeader;

// 函数指针定义，根据消息头部信息创建出新的消息体对象
typedef IMessage * (* CreateBodyFromHeader)(const CMessageHeader & header);


#endif//!_I_MESSAGE_H_