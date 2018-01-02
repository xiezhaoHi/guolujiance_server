#ifndef _USER_H_
#define _USER_H_

#include "structs/structs.h"
#include "service/socket_service/tcp_session.h"
#include <QDateTime>

class CRig;
typedef std::shared_ptr<CRig> rig_ptr;

class CTcpSession;

typedef std::shared_ptr<QDateTime> date_time_ptr;

// �û�
class CUser : public CUserInfo, public std::enable_shared_from_this<CUser>
{
public:

    CUser();

    ~CUser();

    // ������������
    void SetRig(rig_ptr rig);

    // ������������
    rig_ptr GetRig();

    // ���ûỰ
    virtual void SetSession(tcp_session_ptr session);

    // ��ȡ�ͻ��˻Ự
    const tcp_session_ptr GetSession();

    // ��ʾ�û���¼��
    virtual QString GetDisplayText();

    // �����û���¼ʱ�� 
    void SetLoginTime(date_time_ptr ptr);

    // ��ȡ�û���¼ʱ��
    date_time_ptr GetLoginTime();

private:
                               // �û���Ϣ
    rig_ptr         m_pRig;    // ��������
    tcp_session_ptr m_pSession;// �ͻ������ӻỰ
    date_time_ptr   m_pLoginTime;
};


typedef std::shared_ptr<CUser> user_ptr;

#endif//!_USER_H_