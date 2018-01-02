#ifndef _I_TCP_SESSION_MESSAGE_HANDLER_h_
#define _I_TCP_SESSION_MESSAGE_HANDLER_h_

#include "message/message.h"

#define ERR_NONE                0x00 /*ִ�гɹ�*/
#define ERR_UNMATCHED_CMD       0x01 /*�����벻ƥ��*/
#define ERR_UNMATCHED_ACCOUNT   0x02 /*�û��������벻ƥ��*/
#define ERR_INVALID_ACCOUNT     0x03 /*��������˺�Ȩ�޲���*/
#define ERR_DB_ERROR            0x04 /*���ݿ���Ϣ����*/
#define ERR_INVALID_SESSION     0x05 /*δ�ɹ�������¼��֤�ĻỰ*/
#define ERR_INVALID_ID          0x06 /*��Ч��ID*/
#define ERR_INVALID_LICENSE     0x07 /*��Ч��ע����,������δȡ��������Ȩ*/
#define ERR_INVALID_PERIOD      0x08 /*��Ч��ʱ���*/
#define ERR_INVALID_PARAM       0x09 /*��Ч�Ĳ���*/

class CTcpSession;
typedef std::shared_ptr<CTcpSession> tcp_session_ptr;

class ITcpSessionMessageHandler
{
public:
    ITcpSessionMessageHandler() {m_func = NULL; m_funcV2 = NULL;}

    virtual ~ITcpSessionMessageHandler() {}

    // �ص���������1��������޻ظ���Ϣ
    typedef int (ITcpSessionMessageHandler:: *HandleSessionMessage)(tcp_session_ptr session, message_ptr sessionMessageIn);

    // �ص���������2:������лظ���Ϣ
    typedef int (ITcpSessionMessageHandler:: *HandleSessionMessageV2)(tcp_session_ptr session, message_ptr sessionMessageIn,
        message_ptr responseMessageOut);

    HandleSessionMessage    m_func;

    HandleSessionMessageV2  m_funcV2;
};

#endif//!_I_TCP_SESSION_MESSAGE_HANDLER_h_