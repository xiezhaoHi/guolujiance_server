#include "message.h"
#include "message_body_factory.h"
#include <QDebug>


int CMessage::m_nMaxBodySize = 32 << 10;// ��Ϣ�����ֻ�ܰ���32KB


CMessage::CMessage()
{
    m_pBody = NULL;
}

CMessage::~CMessage()
{
    if (m_pBody) {
        delete m_pBody;
        m_pBody = NULL;
    }
}

std::size_t CMessage::Length()
{
    return (NULL != m_pBody) ? 
        m_pBody->Length() + m_header.Length() :
        m_header.Length();
}

bool CMessage::WriteToOutputByteArray(OutputByteArray & out)
{
    if (m_pBody) {
        m_header.m_struct.bodyLength = m_pBody->Length();
    } else {
        m_header.m_struct.bodyLength = NULL;
    }
    bool ret = m_header.WriteToOutputByteArray(out);
    if (ret && m_pBody) {
        ret = m_pBody->WriteToOutputByteArray(out);
    }

    if (m_header.m_struct.cmd == DEVICE_CMD_LOGIN) {
        char * data = out.Data();

        QString str;
        for (int i = 0; i < out.Size(); i++) {
            if ((i + 1) % 8 == 0){
                str.append(QStringLiteral("\n"));
            }

            //str.append(::sprintf("%02X ", (const char *)(data + i)));
        }

        qDebug() << str;
    }

    return ret;
}

bool CMessage::ReadFromInputByteArray(InputByteArray &)
{
    throw std::logic_error("The method or operation could never been invoked.");
    return false;
}

//******************************************************
//** ������:   SetMessageBody
//** ���ܼ���: 
//** �������: 
//   IMessage * pMsgBody:
//
//** ����ֵ: 
//   void:
//
//** �������ڣ�2015/07/15
//** �����ˣ�  ����
//** �޸ļ�¼��
//   ����        �汾      �޸���    �޸�����
//
//** ����˵����
//******************************************************
void CMessage::SetMessageBody(IMessage * pMsgBody)
{
    _ASSERT(m_pBody == NULL || m_pBody == pMsgBody);

    m_pBody = pMsgBody;
    m_header.m_struct.bodyLength = (NULL == pMsgBody) ? 0 : pMsgBody->Length();
}

//******************************************************
//** ������:   CreateMessageBody
//** ���ܼ���: 
//** �������: 
//
//** ����ֵ: 
//   bool:
//
//** �������ڣ�2015/07/15
//** �����ˣ�  ����
//** �޸ļ�¼��
//   ����        �汾      �޸���    �޸�����
//
//** ����˵����
//******************************************************
bool CMessage::CreateMessageBody()
{
    if (m_pBody) {
        delete m_pBody;
        m_pBody = NULL;
    }

    m_pBody = CMessageBodyFactory::GetInstance()->Product(m_header);

    return m_pBody != NULL;
}

bool CMessage::AppendInputByteArray(U8 * u8array, int arrayLen)
{
    if (!u8array || arrayLen <= 0)
    {
        return false;
    }
	m_InputArray.TruncateBytesBeforeOffset(); //�������
    m_InputArray.AppendBytes(QByteArray((char *)u8array, arrayLen));
    return true;
}

bool CMessage::ParseHeader()
{
    return m_header.ReadFromInputByteArray(m_InputArray);
}

bool CMessage::ParseBody()
{
    if (m_header.m_struct.bodyLength > 0) {
        return CreateMessageBody() && m_pBody->ReadFromInputByteArray(m_InputArray);
    }
    return true;
}

bool CMessage::IsValidHeader()
{
    if (m_header.m_struct.bodyLength > m_nMaxBodySize)
    {
        return false;
    }

    return m_header.IsValid();
}

QString CMessage::ToString()
{
    QString qstr;
    qstr.append(m_header.ToString());
    if (m_pBody) {
        qstr.append(m_pBody->ToString());
    }

    return qstr;
}

void CMessage::SetCMDToHead(unsigned int length ,unsigned int cmd )
{
	m_header.m_struct.cmd = cmd;
	m_header.m_struct.bodyLength = length;
}