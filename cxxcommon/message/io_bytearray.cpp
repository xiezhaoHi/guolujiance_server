#include "io_bytearray.h"
#include "qendian.h"
#include <QString>

//////////////////////////////////////////////////////////////////////////
// InputByteArray

InputByteArray::InputByteArray()
{
    m_offset = 0;
}

InputByteArray::~InputByteArray()
{
    m_buf.clear();
}

U8 InputByteArray::ReadU8()
{
    U8 ret;
    const uchar *pBuf = (uchar *)m_buf.constData();
    ret = CONVERT_FROM_NETWORK_ORDER(U8, pBuf + m_offset);
    m_offset += sizeof(U8);
    return ret;
}

U16 InputByteArray::ReadU16()
{
    U16 ret;
    const uchar *pBuf = (uchar *)m_buf.constData();
    ret = CONVERT_FROM_NETWORK_ORDER(U16, pBuf + m_offset);
    m_offset += sizeof(U16);
    return ret;
}

U32 InputByteArray::ReadU32()
{
    U32 ret;
    const uchar *pBuf = (uchar *)m_buf.constData();
    ret = CONVERT_FROM_NETWORK_ORDER(U32, pBuf + m_offset);
    m_offset += sizeof(U32);
    return ret;
}

UL32 InputByteArray::ReadUL32()
{
    U32 read = ReadU32();
    return *((UL32 *)&read);
}

U64 InputByteArray::ReadU64()
{
    U64 ret;
    const uchar *pBuf = (uchar *)m_buf.constData();
    ret = CONVERT_FROM_NETWORK_ORDER(U64, pBuf + m_offset);
    m_offset += sizeof(U64);
    return ret;
}

S8 InputByteArray::ReadS8()
{
    U8 read = ReadU8();
    return *((S8 *)&read);
}

S16 InputByteArray::ReadS16()
{
    U16 read = ReadU16();
    return *((S16 *)&read);
}

S32 InputByteArray::ReadS32()
{
    U32 read = ReadU32();
    return *((S32 *)&read);
}

L32 InputByteArray::ReadL32()
{
    U32 read = ReadU32();
    return *((L32 *)&read);
}

F32 InputByteArray::ReadF32()
{
    U32 read = ReadU32();
    return *((F32 *)&read);
}

D64 InputByteArray::ReadD64()
{
    U64 read = ReadU64();
    return *((D64 *)&read);
}

S64 InputByteArray::ReadS64()
{
    U64 read = ReadU64();
    return *((S64 *)&read);
}

void InputByteArray::SetOffset(int offset)
{
    m_offset = offset;
}

void InputByteArray::ResetOffset()
{
    m_offset = 0;
}

void InputByteArray::AppendBytes(const QByteArray & bytes)
{
    m_buf.append(bytes);
}

int InputByteArray::Size()
{
    _ASSERT(m_offset >= 0);
    return m_buf.size() - m_offset;
}

void InputByteArray::TruncateBytesBeforeOffset()
{
    m_buf.remove(0, m_offset);
    m_offset = 0;
}

U8 * InputByteArray::Data() const
{
    return (U8 *)m_buf.constData();
}

bool InputByteArray::ReadBytes(int len, QByteArray &out)
{
    Q_ASSERT(m_buf.length() - len - m_offset >= 0);

    U8 * buf = new U8[len];
    for (int i = 0; i < len; i++) {
        buf[i] = ReadU8();
    }
    out.append(QByteArray::fromRawData((char *)buf, len));
    delete[] buf;

    return true;
}

bool InputByteArray::ReadUtf8String(int len, QString &out)
{
    QByteArray array;
    if (!ReadBytes(len, array)) {
        return false;
    }
    
    out = QString::fromUtf8(array);
    return true;
}

union myfloat
{
	float f;
	uchar buf[4];
};
union  myInt
{
	int a;
	uchar buf[4];
};
//从16进制表示到float
F32 InputByteArray::ReadF32New()
{
	char ret;
	QString str;
	myfloat f;
	U8 ch;
	for (int index = 0; index < 4; ++index)
	{
		f.buf[index] = ReadU8();
	}
	return f.f;
}

//////////////////////////////////////////////////////////////////////////
// OutputByteArray 

OutputByteArray::OutputByteArray()
{

}

OutputByteArray::~OutputByteArray()
{
    m_buf.clear();
}

void OutputByteArray::WriteU8(U8 val)
{
    m_buf.append(val);
}

void OutputByteArray::WriteU16(U16 val)
{
    U16 temp = CONVERT_TO_NETWORK_ORDER(U16, val);
    m_buf.append(QByteArray::fromRawData((char *)&temp, sizeof(U16)));
}

void OutputByteArray::WriteU32(U32 val)
{
    U32 temp = CONVERT_TO_NETWORK_ORDER(U32, val);
    m_buf.append(QByteArray::fromRawData((char *)&temp, sizeof(U32)));
}

void OutputByteArray::WriteUL32(UL32 val)
{
    WriteU32(*(U32 *)&val);
}

void OutputByteArray::WriteU64(U64 val)
{
    U64 temp = CONVERT_TO_NETWORK_ORDER(U64, val);
    m_buf.append(QByteArray::fromRawData((char *)&temp, sizeof(U64)));
}

void OutputByteArray::WriteS8(S8 val)
{
    WriteU8(*(U8 *)&val);
}

void OutputByteArray::WriteS16(S16 val)
{
    WriteU16(*(U16 *)&val);
}

void OutputByteArray::WriteS32(S32 val)
{
    WriteU32(*(U32 *)&val);
}

void OutputByteArray::WriteL32(L32 val)
{
    WriteU32(*(U32 *)&val);
}

void OutputByteArray::WriteF32(F32 val)
{
    WriteU32(*(U32 *)&val);
}

void OutputByteArray::WriteD64(D64 val)
{
    WriteU64(*(U64 *)&val);
}

void OutputByteArray::WriteS64(S64 val)
{
    WriteU64(*(U64 *)&val);
}

char * OutputByteArray::Data()
{
    return m_buf.data();
}

int OutputByteArray::Size()
{
    return m_buf.size();
}

void OutputByteArray::WriteBytes(const QByteArray &in, int len)
{
    Q_ASSERT(in.length() >= len);

    m_buf.append(QByteArray(in.constData(), len));
}

void OutputByteArray::WriteUtf8String(const QString &in)
{
    QByteArray array = in.toUtf8();
    WriteBytes(array, array.length());
}


