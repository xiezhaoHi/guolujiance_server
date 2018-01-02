#ifndef _BYTEARRAY_STREAM_H_
#define _BYTEARRAY_STREAM_H_

#include<QByteArray>
#include "protocol/protocol_data_type.h"

#define ENABLE_BYTE_ORDER_CONVERT

#ifdef ENABLE_BYTE_ORDER_CONVERT
#define CONVERT_FROM_NETWORK_ORDER(type, pValue) qFromBigEndian<type>(*((type *)(pValue)))
#define CONVERT_TO_NETWORK_ORDER(type, value) qToBigEndian<type>(value)
#else
#define CONVERT_FROM_NETWORK_ORDER(type, pValue) (*((type *)(pValue)))
#define CONVERT_TO_NETWORK_ORDER(type, value)   (type)(value)
#endif

class InputByteArray
{
public:
    InputByteArray();

    ~InputByteArray();

    U8 ReadU8();

    U16 ReadU16();

    U32 ReadU32();

    UL32 ReadUL32();

    U64 ReadU64();

    S8 ReadS8();

    S16 ReadS16();

    S32 ReadS32();

    L32 ReadL32();

    F32 ReadF32();

    D64 ReadD64();

    S64 ReadS64();

    bool ReadBytes(int len, QByteArray &out);

    bool ReadUtf8String(int len, QString &out);

    void SetOffset(int offset);

    void ResetOffset();

    // 将偏移位置以前的字节全部截断
    void TruncateBytesBeforeOffset();

    void AppendBytes(const QByteArray & bytes);

    int Size();

    U8 * Data() const;

private:
    QByteArray  m_buf;
    int         m_offset;
};

class OutputByteArray
{
public:
    OutputByteArray();

    ~OutputByteArray();

    void WriteU8(U8 val);

    void WriteU16(U16 val);

    void WriteU32(U32 val);

    void WriteUL32(UL32 val);

    void WriteU64(U64 val);

    void WriteS8(S8 val);

    void WriteS16(S16 val);

    void WriteS32(S32 val);

    void WriteL32(L32 val);

    void WriteF32(F32 val);

    void WriteD64(D64 val);

    void WriteS64(S64 val);

    void WriteBytes(const QByteArray &in, int len);

    void WriteUtf8String(const QString &in);

    char *Data();

    int Size();

private:
    QByteArray  m_buf;
};

#endif