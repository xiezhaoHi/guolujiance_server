#ifndef _DEVICE_H_
#define _DEVICE_H_

#include "structs/structs.h"
#include "message/message.h"
#include "service/socket_service/tcp_session.h"
#include <QDateTime>


class CRig;
typedef std::shared_ptr<CRig> rig_ptr;

class CTcpSession;

typedef std::shared_ptr<QDateTime> date_time_ptr;

class CDevice : public CDeviceInfo, public std::enable_shared_from_this<CDevice>
{
public:

    CDevice();

    ~CDevice();

    // ������������
    void SetRig(rig_ptr rig);

    // ��ȡ��������
    rig_ptr GetRig();

    // �����豸���ӻỰ
    virtual void SetSession(tcp_session_ptr session);

    // ��ȡ�豸���ӻỰ����
    const tcp_session_ptr GetSession();

    // �㲥ʵʱ���ݵ����豸�����������ÿһ�����߿ͻ���
    void BroadcastToPCClients(message_ptr msg);

    // ��ȡ�豸����ʾ�ı���Ϣ
    virtual QString GetDisplayText();

    // ��ȡ�豸��ǰ������״̬
    bool IsDeviceOnline();

    // ���õ�¼ʱ��
    void SetLoginTime(date_time_ptr ptr);

    // ��ȡ��¼ʱ��
    date_time_ptr GetLoginTime();
private:
                                 // �豸��Ϣ
    rig_ptr         m_pRig;      // ��������
    tcp_session_ptr m_pSession;  // �豸�Ự
    date_time_ptr   m_pLoginTime;// ��¼ʱ��
};


typedef std::shared_ptr<CDevice> device_ptr;

#endif//!_DEVICE_H_
