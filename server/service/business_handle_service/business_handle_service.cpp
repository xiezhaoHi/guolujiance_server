#include "business_handle_service.h"
#include "service/business_handle_service/business_model/business_model.h"
#include "service/business_handle_service/business_model/oil_field.h"
#include "service/business_handle_service/business_model/rig.h"
#include "service/business_handle_service/business_model/user.h"
#include "service/business_handle_service/business_model/device.h"
#include "service/db_service/db_service.h"
#include "service/socket_service/tcp_session.h"
#include "service/socket_service/tcp_session_message_handler_hub.h"
#include "service/log_service/log_service.h"
#include "message/message_login.h"
#include "message/message_login_rep.h"
#include "message/message_error_info.h"
#include "message/message_device_realtime_data.h"
#include "message/message_get_device_list_rep.h"
#include "message/message_devices_list_item.h"
#include "message/message_get_devices_info_rep.h"
#include "message/message_broadcast_device_realtime_data.h"
#include "message/message_get_devices_info_req.h"
#include "message/message_device_id.h"
#include "message/message_set_device_param.h"
#include "message/message_get_device_online_status.h"
#include "message/message_get_device_online_status_rep.h"
#include "message/message_broadcast_device_online_status.h"
#include "message/message_get_device_online_status_rep_item.h"
#include "message/message_heartbeat.h"
#include "message/message_server_time_beijing.h"
#include "message/message_get_device_data_count_req.h"
#include "message/message_get_device_data_count_rep.h"
#include "message/message_get_device_data_req.h"
#include "message/message_get_device_data_rep.h"
#include "message/message_device_realtime_batched_data.h"
#include "message/message_device_realtime_data_item.h"
#include "util/global_util.h"
#include "registration/registration.h"
#include <QDateTime>
#include <QRegularExpression>



CBusinessHandleService * CBusinessHandleService::m_pInstance = NULL;


CBusinessHandleService::CBusinessHandleService()
{

}

CBusinessHandleService::~CBusinessHandleService()
{

}

CBusinessHandleService * CBusinessHandleService::GetInstance()
{
    if (!m_pInstance) {
        m_pInstance = new CBusinessHandleService();
    }
    return m_pInstance;
}

void CBusinessHandleService::DestroyInstance()
{
    if (m_pInstance) {
        delete m_pInstance;
        m_pInstance = NULL;
    }
}


int CBusinessHandleService::StartInternalService()
{
    //////////////////////////////////////////////////////////////////////////
    // ����PC�ͻ�������

    // �����¼��Ϣ�Ļظ�
    CTcpSessionMessageHandlerHub::GetInstance()->RegisterSessionMessageHandler(PC_CLIENT_CMD_LOGIN, this, 
        static_cast<ITcpSessionMessageHandler::HandleSessionMessageV2>(&CBusinessHandleService::HandlePCClientLogin));
    // ��ȡ�豸�б�
    CTcpSessionMessageHandlerHub::GetInstance()->RegisterSessionMessageHandler(PC_CLIENT_CMD_GET_DEVICE_LIST, this, 
        static_cast<ITcpSessionMessageHandler::HandleSessionMessageV2>(&CBusinessHandleService::HandlePCClientGetDeviceList));
    // ��ȡ�豸����
    CTcpSessionMessageHandlerHub::GetInstance()->RegisterSessionMessageHandler(PC_CLIENT_CMD_GET_DEVICE_INFO, this, 
        static_cast<ITcpSessionMessageHandler::HandleSessionMessageV2>(&CBusinessHandleService::HandlePCClientGetDeviceInfo));
    // �ͻ����˳�
    CTcpSessionMessageHandlerHub::GetInstance()->RegisterSessionMessageHandler(PC_CLIENT_CMD_EXIT, this, 
        static_cast<ITcpSessionMessageHandler::HandleSessionMessage>(&CBusinessHandleService::HandlePCClientExit));
    // �����豸����
    CTcpSessionMessageHandlerHub::GetInstance()->RegisterSessionMessageHandler(PC_CLIENT_CMD_SET_DEVICE_PARAM, this, 
        static_cast<ITcpSessionMessageHandler::HandleSessionMessageV2>(&CBusinessHandleService::HandlePCClientSetDeviceParam));
    // ��ȡ�豸����״̬
    CTcpSessionMessageHandlerHub::GetInstance()->RegisterSessionMessageHandler(PC_CLIENT_CMD_GET_DEVICE_STATUS, this, 
        static_cast<ITcpSessionMessageHandler::HandleSessionMessageV2>(&CBusinessHandleService::HandlePCClientGetDeviceOnlineStatus));
    // �ͻ�������
    CTcpSessionMessageHandlerHub::GetInstance()->RegisterSessionMessageHandler(PC_CLIENT_CMD_SEND_HEARTBEAT, this, 
        static_cast<ITcpSessionMessageHandler::HandleSessionMessageV2>(&CBusinessHandleService::HandlePCClientSendHeartbeat));
    // �ͻ��������ȡ��ʷ���ݵ�����
    CTcpSessionMessageHandlerHub::GetInstance()->RegisterSessionMessageHandler(PC_CLIENT_CMD_GET_HISTORY_DEVICE_DATA_COUNT, this, 
        static_cast<ITcpSessionMessageHandler::HandleSessionMessageV2>(&CBusinessHandleService::HandlePCClientGetHistoryDeviceDataCount));
    // �ͻ��������ȡ��ʷ����
    CTcpSessionMessageHandlerHub::GetInstance()->RegisterSessionMessageHandler(PC_CLIENT_CMD_GET_HISTORY_DEVICE_DATA, this, 
        static_cast<ITcpSessionMessageHandler::HandleSessionMessageV2>(&CBusinessHandleService::HandlePCClientGetHistoryDeviceData));

    //////////////////////////////////////////////////////////////////////////
    // �����豸����

    // �豸��¼
    CTcpSessionMessageHandlerHub::GetInstance()->RegisterSessionMessageHandler(DEVICE_CMD_LOGIN, this, 
        static_cast<ITcpSessionMessageHandler::HandleSessionMessageV2>(&CBusinessHandleService::HandleDeviceLogin));
    // �豸�ϴ�ʵʱ�ɼ�����
    CTcpSessionMessageHandlerHub::GetInstance()->RegisterSessionMessageHandler(DEVICE_CMD_REALTIME_DATA, this, 
        static_cast<ITcpSessionMessageHandler::HandleSessionMessage>(&CBusinessHandleService::HandleDeviceRealTimeData));
    // �豸�����ϴ�ʵʱ�ɼ�����
    CTcpSessionMessageHandlerHub::GetInstance()->RegisterSessionMessageHandler(DEVICE_CMD_REALTIME_BATCHED_DATA, this, 
        static_cast<ITcpSessionMessageHandler::HandleSessionMessageV2>(&CBusinessHandleService::HandleDeviceRealTimeBatchedData));
    // �豸�˳�
    CTcpSessionMessageHandlerHub::GetInstance()->RegisterSessionMessageHandler(DEVICE_CMD_EXIT, this, 
        static_cast<ITcpSessionMessageHandler::HandleSessionMessage>(&CBusinessHandleService::HandleDeviceExit));
    // �豸��������ȡ������ʱ��
    CTcpSessionMessageHandlerHub::GetInstance()->RegisterSessionMessageHandler(DEVICE_CMD_SEND_HEARTBEAT, this, 
        static_cast<ITcpSessionMessageHandler::HandleSessionMessageV2>(&CBusinessHandleService::HandleDeviceSendHeartbeat));

    CBusinessModel::GetInstance();// ҵ��ģ����

    return 0;
}

int CBusinessHandleService::StopInternalService()
{
    CBusinessModel::DestroyInstance();

    CTcpSessionMessageHandlerHub::GetInstance()->UnregisterSessionMessageHandler(this);

    return 0;
}

int CBusinessHandleService::NotifyServiceToStop()
{
    return 0;
}

QString CBusinessHandleService::GetServiceName()
{
    return QStringLiteral("business handle service");
}

//******************************************************
//** ������:   HandleMessageLogin
//** ���ܼ���: ����PC�ͻ��˵�¼��Ϣ
//** �������: 
//   CMessage * sessionMessageIn:������Ϣ
//   CMessage & responseMessageOut:�ظ���Ϣ
//
//** ����ֵ: 
//   int:�ɹ�ʱ����ERR_NONE�����򷵻���Ӧ�Ĵ�����
//
//** �������ڣ�2015/07/26
//** �����ˣ�  ����
//** �޸ļ�¼��
//   ����        �汾      �޸���    �޸�����
//
//** ����˵����
//******************************************************
int CBusinessHandleService::HandlePCClientLogin(tcp_session_ptr session, 
                                                message_ptr sessionMessageIn,
                                                message_ptr responseMessageOut)
{
    // �����Ȩ��֤
    IS_REGISTED(ERR_INVALID_LICENSE);

    Q_ASSERT(sessionMessageIn);

    if (sessionMessageIn->m_header.m_struct.cmd != PC_CLIENT_CMD_LOGIN) {
        return ERR_UNMATCHED_CMD;
    }

    int retVal = ERR_NONE;
    CMessageErrorInfo * pRepErrorInfo = NULL;

    CMessageLogin * pMessageLogin = static_cast<CMessageLogin *>(sessionMessageIn->m_pBody);
	
    QString md5Password=CGlobalUtil::getInstance()->MD5(pMessageLogin->m_qstrPassword);
    bool ret = CDBService::GetInstance()->IsMatchedAccount(pMessageLogin->m_qstrAccount, md5Password);
    if (!ret) {
        ret = CDBService::GetInstance()->IsValidUser(pMessageLogin->m_qstrAccount);
        if (!ret) { 
            retVal = ERR_INVALID_ACCOUNT;// �û���������
            pRepErrorInfo = new CMessageErrorInfo();
            pRepErrorInfo->m_qstrErrorInfo = QStringLiteral("�û���������");
            responseMessageOut->SetMessageBody(pRepErrorInfo);
        } else {
            retVal = ERR_UNMATCHED_ACCOUNT;// �û��������벻ƥ��
            pRepErrorInfo = new CMessageErrorInfo();
            pRepErrorInfo->m_qstrErrorInfo = QStringLiteral("�û��������벻ƥ��");
            responseMessageOut->SetMessageBody(pRepErrorInfo);
        }
    } else {
        QString accountID;// �û�ID
        ret = CDBService::GetInstance()->GetAccountID(pMessageLogin->m_qstrAccount, accountID);
        if (!ret || accountID.isEmpty()) {
            retVal = ERR_DB_ERROR;// ���ݿ��¼����
            pRepErrorInfo = new CMessageErrorInfo();
            pRepErrorInfo->m_qstrErrorInfo = QStringLiteral("���ݿ��¼����");
            responseMessageOut->SetMessageBody(pRepErrorInfo);
        } else {
            // �û���¼�ɹ�������tcp����Ϊ����֤״̬�����û����������Ӱ�

            // ��ȡ�û��������ӡ���������
            oil_field_ptr pOilField = NULL;
            rig_ptr pRig = NULL;
            user_ptr pUser = NULL;

            QString qstrRigID;
            ret = CDBService::GetInstance()->GetRigIDByAccountID(accountID, qstrRigID);
            if (ret) {
                QString qstrOilFieldID;
                ret = CDBService::GetInstance()->GetOilFieldIDByRigID(qstrRigID, qstrOilFieldID);
                if (ret) {
                    // ��ȡ�������
                    pOilField = CBusinessModel::GetInstance()->FindOilFieldByID(qstrOilFieldID);
                    if (!pOilField) {
                        pOilField = CBusinessModel::GetInstance()->GetOrCreateOilField(qstrOilFieldID);
                    }

                    Q_ASSERT(pOilField);

                    // ��ȡ���Ӷ���
                    pRig = pOilField->FindRigByID(qstrRigID);
                    if (!pRig) {
                        pRig = pOilField->GetOrCreateRig(qstrRigID);
                    }

                    Q_ASSERT(pRig);

                    // ��ȡ�û�����
                    pUser = pRig->FindUserByID(accountID);
                    if (!pUser) {
                        pUser = pRig->GetOrCreateUser(accountID);
                        pUser->name = pMessageLogin->m_qstrAccount;
                    }

                    Q_ASSERT(pUser);

                    pUser->SetSession(session);// ���ÿͻ��˻Ự���ڲ��Զ�������뾮�ӵ����߿ͻ����б�
                    // ��¼�û���¼ʱ��
                    pUser->SetLoginTime(std::make_shared<QDateTime>(QDateTime::currentDateTime()));

                    // ���ûظ���Ϣ
                    CMessageLoginRep * pResponse = new CMessageLoginRep();
                    // ��ȡ��ǰ������ʱ�䣬������
                    pResponse->m_struct.serverTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
                    responseMessageOut->SetMessageBody(pResponse);

                    LOG_WARING() << pRig->GetDevicesOnlineStatusDisplayText();
                    LOG_WARING() << QStringLiteral("����[%1]�������û�").arg(pRig->GetDisplayText())
                        << pRig->GetOnlineUsersDisplayText();
                }
            }
            else
            {
                retVal = ERR_INVALID_ACCOUNT;// �û���������
                pRepErrorInfo = new CMessageErrorInfo();
                pRepErrorInfo->m_qstrErrorInfo = QStringLiteral("�û����κξ�����Ա");
                responseMessageOut->SetMessageBody(pRepErrorInfo);
            }
        }
    }

    if (retVal != ERR_NONE) {
        Q_ASSERT(pRepErrorInfo);

        LOG_WARING() << QStringLiteral("PC�ͻ��˵�¼ʧ�� ����") << pRepErrorInfo->m_qstrErrorInfo
            << QStringLiteral("{��¼��:") << pMessageLogin->m_qstrAccount
            << QStringLiteral(", ����:") << pMessageLogin->m_qstrPassword 
            << QStringLiteral("}");
    } else  {
        LOG_INFO() << QStringLiteral("PC�ͻ��˵�¼�ɹ� ")
            << QStringLiteral("{��¼��:") << pMessageLogin->m_qstrAccount
            << QStringLiteral(", ����:") << pMessageLogin->m_qstrPassword 
            << QStringLiteral("}");
    }

    return retVal;
}



//******************************************************
//** ������:   HandlePCClientGetDeviceList
//** ���ܼ���: ���������豸ID�б�
//** �������: 
//   CMessage * sessionMessageIn:
//   CMessage & responseMessageOut:
//
//** ����ֵ: 
//   int:
//
//** �������ڣ�2015/07/27
//** �����ˣ�  ����
//** �޸ļ�¼��
//   ����        �汾      �޸���    �޸�����
//
//** ����˵����
//******************************************************
int CBusinessHandleService::HandlePCClientGetDeviceList(tcp_session_ptr session, 
                                                        message_ptr sessionMessageIn, 
                                                        message_ptr responseMessageOut)
{
    Q_ASSERT(session);
    Q_ASSERT(sessionMessageIn);

    if (sessionMessageIn->m_header.m_struct.cmd != PC_CLIENT_CMD_GET_DEVICE_LIST) {
        return ERR_UNMATCHED_CMD;
    }

    int retVal = ERR_NONE;
    CMessageErrorInfo * pRepErrorInfo = NULL;
    rig_ptr pRig;
    user_ptr pUser = std::static_pointer_cast<CUser>(session->m_entity);
    QList<CMessageDevicesListItem *> lstDeviceItems;

    if (!pUser) {
        retVal = ERR_INVALID_SESSION;
        pRepErrorInfo = new CMessageErrorInfo();
        pRepErrorInfo->m_qstrErrorInfo = QStringLiteral("�ܾ��Ǿ�����¼��֤�Ŀͻ��˻�ȡ�豸�б�");
        responseMessageOut->SetMessageBody(pRepErrorInfo);
    } else {// �����û�ID��ȡ���û��ܷ��ʵ������豸ID
        Q_ASSERT(pUser);
        QString qstrUserID = pUser->id;// ��session��ȡ���û�ID

        // ��ȡ���˺����ھ��ӵ�H2S�豸ID�б�
        bool ret = CDBService::GetInstance()->GetH2SDeviceListByPersonID(qstrUserID, lstDeviceItems);
        if (!ret) {
            retVal = ERR_INVALID_ACCOUNT;
            pRepErrorInfo = new CMessageErrorInfo();
            pRepErrorInfo->m_qstrErrorInfo = QStringLiteral("�����û����Ҳ�������ʹ�õ��豸�б�");
            responseMessageOut->SetMessageBody(pRepErrorInfo);
        } else {
            pRig = pUser->GetRig();
            Q_ASSERT(pRig);

            // ����ҵ��ʵ��ģ������豸��Ϣ
            for (CMessageDevicesListItem * pItem : lstDeviceItems) {
                QString qstrDeviceID = QString::fromLocal8Bit(((char *)pItem->m_struct.deviceId), sizeof(pItem->m_struct.deviceId)).trimmed();
                device_ptr pDevice = pRig->FindDeviceByID(qstrDeviceID);
                if (!pDevice) {
                    pDevice = pRig->GetOrCreateDevice(qstrDeviceID);
                    pDevice->code = QString::fromLocal8Bit((char *)(pItem->m_struct.deviceNo), sizeof(pItem->m_struct.deviceNo)).trimmed();
                    pDevice->name = QString::fromLocal8Bit((char *)(pItem->m_struct.deviceName), sizeof(pItem->m_struct.deviceName)).trimmed();
                }
                pDevice->deviceType = (pItem->m_struct.devType == 1) ? DEVICE_TYPE_PORTABLE_H2S_DETECTOR : DEVICE_TYPE_STATIC_H2S_DETECTOR;
                pDevice->warningLine1st = QString::fromLocal8Bit((char *)(pItem->m_struct.warningLine1st), strlen((char *)pItem->m_struct.warningLine1st));
                pDevice->warningLine2nd = QString::fromLocal8Bit((char *)(pItem->m_struct.warningLine2nd), strlen((char *)pItem->m_struct.warningLine2nd));
                pDevice->maxRange = QString::fromLocal8Bit((char *)(pItem->m_struct.maxRange), strlen((char *)pItem->m_struct.maxRange));
                pDevice->sampleRate = pItem->GetSampleRate();
            }

            if (pRig) {
                emit pRig->rigDeviceUpdated();
            }

            // ��CMessageGetDeviceListRep���ؽ��
            CMessageGetDeviceListRep * pResponse = new CMessageGetDeviceListRep();
            pResponse->m_lstDevices = lstDeviceItems;
            pResponse->m_struct.deviceCount = lstDeviceItems.count();
            // ���ûظ���Ϣ����
            responseMessageOut->SetMessageBody(pResponse);
        }
    }

    if (retVal != ERR_NONE) {
        Q_ASSERT(pRepErrorInfo);

        if (pUser) {
            LOG_WARING() << QStringLiteral("PC�ͻ���[%1]��ѯ�豸�б�ʧ��,������Ϣ��%2")
                .arg(pUser->GetDisplayText()).arg(pRepErrorInfo->m_qstrErrorInfo);
        } else {
            LOG_WARING() << QStringLiteral("PC�ͻ���[%1]��ѯ�豸�б�ʧ��,������Ϣ��%2")
                .arg(session->get_display_text())<< pRepErrorInfo->m_qstrErrorInfo;
        }
    } else  {
        Q_ASSERT(pUser);
        LOG_INFO() << QStringLiteral("PC�ͻ���[%1]��ѯ�豸�б�ɹ�,������[%2]���豸��Ϣ")
            .arg(pUser->GetDisplayText()).arg(lstDeviceItems.size());
    }

    return retVal;
}

//******************************************************
//** ������:   HandlePCClientGetDeviceInfo
//** ���ܼ���: ���ظ����豸ID���϶�Ӧ���豸����
//** �������: 
//   CMessage * sessionMessageIn:
//   CMessage & responseMessageOut:
//
//** ����ֵ: 
//   int:
//
//** �������ڣ�2015/07/27
//** �����ˣ�  ����
//** �޸ļ�¼��
//   ����        �汾      �޸���    �޸�����
//
//** ����˵����
//******************************************************
int CBusinessHandleService::HandlePCClientGetDeviceInfo(tcp_session_ptr session, 
                                                        message_ptr sessionMessageIn,
                                                        message_ptr responseMessageOut)
{
    Q_ASSERT(session);
    Q_ASSERT(sessionMessageIn);

    if (sessionMessageIn->m_header.m_struct.cmd != PC_CLIENT_CMD_GET_DEVICE_INFO) {
        LOG_WARING() << QStringLiteral("ERR_UNMATCHED_CMD cmd:%1 func:%2")
            .arg(sessionMessageIn->m_header.m_struct.cmd).arg(__FUNCTION__);
        return ERR_UNMATCHED_CMD;
    }

    int retVal = ERR_NONE;
    CMessageErrorInfo * pRepErrorInfo = NULL;
    QString qstrDeviceID;
    CMessageGetDevicesInfoReq * pRequest = static_cast<CMessageGetDevicesInfoReq *>(sessionMessageIn->m_pBody);

    user_ptr pUser = std::static_pointer_cast<CUser>(session->m_entity);

    if (!pUser) {
        retVal = ERR_INVALID_SESSION;
        pRepErrorInfo = new CMessageErrorInfo();
        pRepErrorInfo->m_qstrErrorInfo = QStringLiteral("�ܾ�δ������¼��֤�Ŀͻ��˻�ȡ�豸��ϸ��Ϣ");
        responseMessageOut->SetMessageBody(pRepErrorInfo);
    } else {
        Q_ASSERT(pUser);
        // ��CMessageGetDeviceListRep���ؽ��
        CMessageGetDevicesInfoRep * pResponse = new CMessageGetDevicesInfoRep();
        QString qstrUserID = pUser->id;// ��session��ȡ���豸ID
        
        bool ret = true;
        for (int i = 0; i < pRequest->m_struct.deviceCount; i++) {
            CMessageDeviceID * pDeviceID = pRequest->m_lstDeviceID.at(i);
            qstrDeviceID = QByteArray((char *)&(pDeviceID->m_struct), sizeof(T_MSG_DEVICE_ID));

            CMessageDeviceItemInfo * pDeviceInfo = new CMessageDeviceItemInfo();
            ret = CDBService::GetInstance()->GetDeviceInfoByDeviceID(qstrDeviceID, pDeviceInfo);
            if (!ret) {
                retVal = ERR_INVALID_ID;// ��Ч���豸ID
                pRepErrorInfo = new CMessageErrorInfo();
                pRepErrorInfo->m_qstrErrorInfo = QStringLiteral("�����豸���[%1]�Ҳ����豸��ϸ��Ϣ").arg(qstrDeviceID);
                responseMessageOut->SetMessageBody(pRepErrorInfo);
                break;
            } else {
                pResponse->m_lstDeviceItemInfo.push_back(pDeviceInfo);
            }
        }

        if (ret) {
            // ���ûظ���Ϣ����
            responseMessageOut->SetMessageBody(pResponse);
        } else {
            delete pResponse;
        }
    }

    if (retVal != ERR_NONE) {
        Q_ASSERT(pRepErrorInfo);

        if (pUser) {
            LOG_WARING() << QStringLiteral("PC�ͻ���[%1]��ѯ�豸[%2]��Ϣʧ��,������Ϣ��%3")
                .arg(pUser->GetDisplayText()).arg(qstrDeviceID)
                .arg(pRepErrorInfo->m_qstrErrorInfo);
        } else {
            LOG_WARING() << QStringLiteral("PC�ͻ���[%1]��ѯ�豸��Ϣʧ��,������Ϣ��%3")
                .arg(session->get_display_text()).arg(pRepErrorInfo->m_qstrErrorInfo);
        }
    } else {
        LOG_INFO() << QStringLiteral("PC�ͻ���[%1]��ѯ�豸[%2]��Ϣ�ɹ� ")
            .arg(pUser->GetDisplayText()).arg(qstrDeviceID);
    }

    return retVal;
}

//******************************************************
//** ������:   HandlePCClientExit
//** ���ܼ���: PC�ͻ����˳�
//** �������: 
//   CMessage * sessionMessageIn:
//
//** ����ֵ: 
//   int:
//
//** �������ڣ�2015/07/27
//** �����ˣ�  ����
//** �޸ļ�¼��
//   ����        �汾      �޸���    �޸�����
//
//** ����˵����
//******************************************************
int CBusinessHandleService::HandlePCClientExit(tcp_session_ptr session, 
                                               message_ptr sessionMessageIn)
{
    Q_ASSERT(session);
    Q_ASSERT(sessionMessageIn);

    if (sessionMessageIn->m_header.m_struct.cmd != PC_CLIENT_CMD_EXIT) {
        Q_ASSERT(false);
        return ERR_UNMATCHED_CMD;
    }

    user_ptr pUser = std::static_pointer_cast<CUser>(session->m_entity);
    Q_ASSERT(pUser);

    rig_ptr pRig = std::static_pointer_cast<CRig>(pUser->GetRig());
    Q_ASSERT(pRig);

    LOG_INFO() << QStringLiteral("����[%1]���û�[%2]�˳�,�ر�����[%3]")
        .arg(pRig->GetDisplayText()).arg(pUser->GetDisplayText())
        .arg(session->get_display_text());

    // ����ǰ�Ự�Ƴ������ӵĿͻ��˻Ự��,���ر�����
    pRig->RemoveClientSession(session);
    session->stop();
    session->set_entity(NULL);
    pUser->SetSession(NULL);

    return 0;
}


//******************************************************
//** ������:   HandlePCClientGetDeviceOnlineStatus
//** ���ܼ���: 
//** �������: 
//   tcp_session_ptr session:
//   message_ptr sessionMessageIn:
//   message_ptr responseMessageOut:
//
//** ����ֵ: 
//   int:
//
//** �������ڣ�2015/08/05
//** �����ˣ�  ����
//** �޸ļ�¼��
//   ����        �汾      �޸���    �޸�����
//
//** ����˵����
//******************************************************
int CBusinessHandleService::HandlePCClientGetDeviceOnlineStatus(tcp_session_ptr session,
                                                                message_ptr sessionMessageIn, 
                                                                message_ptr responseMessageOut)
{
    Q_ASSERT(session);
    Q_ASSERT(sessionMessageIn);

    int retVal = ERR_NONE;
    CMessageErrorInfo * pRepErrorInfo = NULL;
    user_ptr pUser = std::static_pointer_cast<CUser>(session->m_entity);
    rig_ptr pRig;

    if (!pUser) {// �ͻ���δ��¼
        retVal = ERR_INVALID_SESSION;
        pRepErrorInfo = new CMessageErrorInfo();
        pRepErrorInfo->m_qstrErrorInfo = QStringLiteral("�ܾ�δ������¼��֤�Ŀͻ��˻�ȡ�豸����״̬");
        responseMessageOut->SetMessageBody(pRepErrorInfo);
    } else {
        CMessageGetDeviceOnlineStatus * pReq = static_cast<CMessageGetDeviceOnlineStatus *>(sessionMessageIn->m_pBody);
        Q_ASSERT(pReq);

        if (pReq->m_lstDeviceIDs.empty()) {// �����е��豸ID�б�Ϊ��
            retVal = ERR_INVALID_ID;
            pRepErrorInfo = new CMessageErrorInfo();
            pRepErrorInfo->m_qstrErrorInfo = QStringLiteral("�ͻ��������ȡ����״̬���豸ID�б�Ϊ��");
            responseMessageOut->SetMessageBody(pRepErrorInfo);
        } else {
            CMessageGetDeviceOnlineStatusRep * pRep = new CMessageGetDeviceOnlineStatusRep();
            for (CMessageDeviceID * pDeviceID : pReq->m_lstDeviceIDs)
            {
                CMessageGetDeviceOnlineStatusRepItem * pStatusItem = new CMessageGetDeviceOnlineStatusRepItem();
                memcpy_s(pStatusItem->m_struct.deviceID, sizeof(pStatusItem->m_struct.deviceID), pDeviceID->m_struct.deviceID, sizeof(pDeviceID->m_struct.deviceID));

                QString qstrDeviceID = QString::fromLocal8Bit((char *)(pDeviceID->m_struct.deviceID), sizeof(pDeviceID->m_struct.deviceID)).trimmed();
                device_ptr pDevice = pRig->FindDeviceByID(qstrDeviceID);
                if (!pDevice) {// δ���ҵ���ӦID���豸
                    pStatusItem->m_struct.deviceOnlineStatus = DEVICE_ONLINE_STATUS_INVALID_DEVICE;
                } else {
                    pStatusItem->m_struct.deviceOnlineStatus = (pDevice->IsDeviceOnline()) ?
                        DEVICE_ONLINE_STATUS_ONLINE : DEVICE_ONLINE_STATUS_OFFLINE;
                }

                pRep->m_lstDeviceIDs.push_back(pStatusItem);
            }
            responseMessageOut->SetMessageBody(pRep);
            retVal = ERR_NONE;
        }
    }

    if (ERR_NONE != retVal) {
        if (!pUser) {
            LOG_WARING() << QStringLiteral("�ܾ�δ������¼��֤�Ŀͻ���[%1]��ȡ�豸����״̬")
                .arg(session->get_display_text());
        } else {
            LOG_WARING() << QStringLiteral("����[%1]���û�[%2]��ȡ�豸����״̬ʧ��,������Ϣ[%3]")
                .arg(pRig->GetDisplayText()).arg(pUser->GetDisplayText())
                .arg(pRepErrorInfo->m_qstrErrorInfo);
        }
    } else {
        LOG_INFO() << QStringLiteral("����[%1]���û�[%2]��ȡ�豸����״̬�ɹ�,������[%3]������״̬��Ϣ")
            .arg(pRig->GetDisplayText()).arg(pUser->GetDisplayText()).arg(3);// TODO 
    }

    return retVal;
}


//******************************************************
//** ������:   HandleDeviceLogin
//** ���ܼ���: ������λ����¼��Ϣ
//** �������: 
//   CMessage * sessionMessageIn:������Ϣ
//   CMessage & responseMessageOut:�ظ���Ϣ
//
//** ����ֵ: 
//   int:�ɹ�ʱ����ERR_NONE�����򷵻���Ӧ�Ĵ�����
//
//** �������ڣ�2015/07/26
//** �����ˣ�  ����
//** �޸ļ�¼��
//   ����        �汾      �޸���    �޸�����
//
//** ����˵����
//******************************************************
int CBusinessHandleService::HandleDeviceLogin(tcp_session_ptr session, 
                                              message_ptr sessionMessageIn,
                                              message_ptr responseMessageOut)
{
	//return ERR_NONE;
    // �����Ȩ��֤
    IS_REGISTED(ERR_INVALID_LICENSE);

    Q_ASSERT(sessionMessageIn);

    if (sessionMessageIn->m_header.m_struct.cmd != DEVICE_CMD_LOGIN) {
        return ERR_UNMATCHED_CMD;
    }

    int retVal = ERR_NONE;
    oil_field_ptr pOilField;
    rig_ptr pRig;
    device_ptr pDevice;
    CMessageErrorInfo * pResponse = NULL;
    CMessageLogin * pMessageLogin = static_cast<CMessageLogin *>(sessionMessageIn->m_pBody);
	LOG_WARING() << QStringLiteral("�豸��¼_number#") << pMessageLogin->m_qstrAccount;
    bool ret = CDBService::GetInstance()->IsExistedDevice(pMessageLogin->m_qstrAccount, pMessageLogin->m_qstrPassword);
    if (!ret) {
        retVal = ERR_INVALID_ACCOUNT;// �豸δע��
        CMessageErrorInfo * pResponse = new CMessageErrorInfo();
        pResponse->m_qstrErrorInfo = QStringLiteral("δע����豸ID");
        responseMessageOut->SetMessageBody(pResponse);
    } else {
        // �豸��¼�ɹ�������tcp����Ϊ����֤״̬���������ӵ��û���Ϣ
        // ��ȡ�豸ID
        QString qstrDeviceID;
        ret = CDBService::GetInstance()->GetDeviceIDByDeviceCode(pMessageLogin->m_qstrAccount, qstrDeviceID);
		LOG_WARING() << "business_handle_service.cpp 664 __id#" << session->get_display_text()  << "#" <<qstrDeviceID;
        if (!ret) {
            pResponse = new CMessageErrorInfo();
            pResponse->m_qstrErrorInfo = QStringLiteral("���ݿ��¼����");
            responseMessageOut->SetMessageBody(pResponse);
            retVal = ERR_DB_ERROR;
		}
		else {
#ifndef RIGID //���� ���� û�а�,��ע�͵� 2017/6/9
			// �����ڴ����ʵ��ҵ��ģ�ͣ���ȡ�û��������ӡ���������
			QString qstrRigID;
			ret = CDBService::GetInstance()->GetRigIDByDeviceID(qstrDeviceID, qstrRigID);
			if (ret) {
				QString qstrOilFieldID;
				ret = CDBService::GetInstance()->GetOilFieldIDByRigID(qstrRigID, qstrOilFieldID);
				if (ret) {
					// ��ȡ�������
					pOilField = CBusinessModel::GetInstance()->FindOilFieldByID(qstrOilFieldID);
					if (!pOilField) {
						pOilField = CBusinessModel::GetInstance()->GetOrCreateOilField(qstrOilFieldID);
					}

					Q_ASSERT(pOilField);

					// ��ȡ���Ӷ���
					pRig = pOilField->FindRigByID(qstrRigID);
					if (!pRig) {
						pRig = pOilField->GetOrCreateRig(qstrRigID);
					}

					Q_ASSERT(pRig);
#endif
					// ��ȡ�豸����
					pDevice = pRig->FindDeviceByID(qstrDeviceID);
					if (!pDevice) {
						pDevice = pRig->GetOrCreateDevice(qstrDeviceID);
					}

					Q_ASSERT(pDevice);

					if (pDevice->GetSession()
						&& session->get_display_text() != pDevice->GetSession()->get_display_text()) {
						// �����ѵ�¼����ͬID���豸 tangqiao 2016/05/17
						//retVal = ERR_INVALID_SESSION;// ��ͬID���豸�ѵ�¼
						//pResponse = new CMessageErrorInfo();
						//pResponse->m_qstrErrorInfo = QStringLiteral("��ͬ��ŵ��豸�ѵ�¼������");
						//responseMessageOut->SetMessageBody(pResponse);
						LOG_WARING() << QStringLiteral("���[%1]���豸�ѵ�¼��������������[%2]����������[%3]")
							.arg(qstrDeviceID).arg(session->get_display_text())
							.arg(pDevice->GetSession()->get_display_text());

						pDevice->SetSession(session);// �����豸�Ự���ڲ��Զ�������뾮�ӵ������豸�б�

						// ���ûظ���Ϣ
						CMessageLoginRep * pResponse = new CMessageLoginRep();
						// ��ȡ��ǰ������ʱ��
						pResponse->m_struct.serverTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
						responseMessageOut->SetMessageBody(pResponse);

						LOG_WARING() << pRig->GetDevicesOnlineStatusDisplayText();
					}
					else {
						pDevice->SetSession(session);// �����豸�Ự���ڲ��Զ�������뾮�ӵ������豸�б�

						// ���ûظ���Ϣ
						CMessageLoginRep * pResponse = new CMessageLoginRep();
						// ��ȡ��ǰ������ʱ��
						pResponse->m_struct.serverTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
						responseMessageOut->SetMessageBody(pResponse);

						LOG_WARING() << pRig->GetDevicesOnlineStatusDisplayText();
					}

				}
			}
		}
    }
	

	return retVal;

    QString format = QStringLiteral("�豸[%1]��¼");
    if (retVal == ERR_NONE) {
        LOG_INFO() << format.arg(pMessageLogin->m_qstrAccount) << QStringLiteral("business_handle_service.cpp_744_�ɹ�");

        pDevice->SetLoginTime(std::make_shared<QDateTime>(QDateTime::currentDateTime()));

        // �㲥�豸���ߵ�֪ͨ�����ӵ����е�¼�ͻ���
        CMessageBroadcastDeviceOnlineStatus * pBroadcastMsg = new CMessageBroadcastDeviceOnlineStatus();
        CMessageGetDeviceOnlineStatusRepItem * pStatusItem = new CMessageGetDeviceOnlineStatusRepItem();
        memcpy_s((char *)(pStatusItem->m_struct.deviceID), sizeof(pStatusItem->m_struct.deviceID), pDevice->id.toLocal8Bit(), pDevice->id.toLocal8Bit().length());
        pStatusItem->m_struct.deviceOnlineStatus = DEVICE_ONLINE_STATUS_ONLINE;
        pBroadcastMsg->m_struct.itemCount = 0;
        pBroadcastMsg->m_lstDeviceIDs.push_back(pStatusItem);

        message_ptr broadcast_msg = std::make_shared<CMessage>();
        broadcast_msg->m_header.m_struct.cmd = PC_SERVER_CMD_BROADCAST_DEVICE_STATUS;
        broadcast_msg->SetMessageBody(pBroadcastMsg);
        pDevice->BroadcastToPCClients(broadcast_msg);// ���͹㲥
		
    } else {
        LOG_WARING() << format.arg(pMessageLogin->m_qstrAccount) << QStringLiteral("ʧ�ܣ�����")
            << pResponse->m_qstrErrorInfo;
    }

    
}

//******************************************************
//** ������:   HandleDeviceRealTimeData
//** ���ܼ���: �����豸�ϴ���ʵʱ�ɼ�����
//** �������: 
//   CMessage * sessionMessageIn:
//
//** ����ֵ: 
//   int:
//
//** �������ڣ�2015/07/27
//** �����ˣ�  ����
//** �޸ļ�¼��
//   ����        �汾      �޸���    �޸�����
//
//** ����˵����
//******************************************************
int CBusinessHandleService::HandleDeviceRealTimeData(tcp_session_ptr session, 
                                                     message_ptr sessionMessageIn)
{
	
    Q_ASSERT(sessionMessageIn);
	
	LOG_WARING() << QStringLiteral("�豸ʵʱ�ϴ�(��������)DEVICE_CMD_REALTIME_DATA_%1 ### cmd_%2##").arg(DEVICE_CMD_REALTIME_DATA)
		.arg(sessionMessageIn->m_header.m_struct.cmd) << GetCurrentThreadId();
    if (sessionMessageIn->m_header.m_struct.cmd != DEVICE_CMD_REALTIME_DATA) {
        return ERR_UNMATCHED_CMD;
    }
    int retVal = ERR_NONE;

    // �豸�ϴ���ʵʱ����
    CMessageDeviceRealtimeData * pMessageData = static_cast<CMessageDeviceRealtimeData *>(sessionMessageIn->m_pBody);

    // ��ȡ��Ӧ���豸����
    device_ptr pDevice = std::static_pointer_cast<CDevice>(session->m_entity);
    if (!pDevice)
    {
        CMessageErrorInfo * pResponse = new CMessageErrorInfo();
        pResponse->m_qstrErrorInfo = QStringLiteral("�ܾ�δ������¼��֤���豸�ϴ�ʵʱ����");
        LOG_WARING() << pResponse->m_qstrErrorInfo;

        message_ptr pMessageOut = std::make_shared<CMessage>();
        pMessageOut->m_header.m_struct.cmd = PC_SERVER_CMD_BROADCAST_REALTIME_DATA;
        pMessageOut->m_header.m_struct.messageNo = sessionMessageIn->m_header.m_struct.messageNo;
        pMessageOut->m_header.m_struct.cmdResult = ERR_INVALID_SESSION;
        if (sessionMessageIn->m_header.m_struct.cmdResult != 0) {
            pMessageOut->SetMessageBody(pResponse);
        } else {
            delete pResponse;
            pResponse = NULL;
        }
        session->deliver(pMessageOut);

        return ERR_INVALID_SESSION;
    }
    Q_ASSERT(pDevice);

    // ���������ϴ�ʱ��
    pMessageData->m_struct.timestamp = QDateTime::currentMSecsSinceEpoch();

    D64 data = 0;
    {
        U8 buf[8];
        U8 * pData = (U8 *)&(pMessageData->m_struct.data_);
        memcpy(buf, pData, 7);

        LOG_DEBUG() << QStringLiteral("�ɼ�����{����״̬[%1];ʵʱŨ��[%2 %3 %4];С����λ[%5];��������[%6];������λ[%7]}")
            .arg(buf[0])
            .arg(buf[1]).arg(buf[2]).arg(buf[3])
            .arg(buf[4])
            .arg(buf[5])
            .arg(buf[6]);
        data = Convert7ByteDataToD64(buf);
    }

    // ��ȡ�豸��������
    rig_ptr pRig = pDevice->GetRig();
    Q_ASSERT(pRig);

    QString qstrOnlineUsers = pRig->GetOnlineUsersDisplayText();
    if (qstrOnlineUsers.isEmpty()) {
        LOG_INFO() << QStringLiteral("�յ�����[%1]��H2S����豸[%2]��ʵʱ�ɼ�����")
            .arg(pRig->GetDisplayText()).arg(pDevice->GetDisplayText());
    } else {
        // ����㲥��Ϣ
        CMessageBroadcastDeviceRealtimeData * pBroadcastData = new CMessageBroadcastDeviceRealtimeData();
        int size = sizeof(pBroadcastData->m_struct.rigId);
        memcpy_s((char *)(pBroadcastData->m_struct.rigId), sizeof(pBroadcastData->m_struct.rigId), (char *)(pRig->id.toLocal8Bit().data()), pRig->id.toLocal8Bit().length());
        memcpy(&(pBroadcastData->m_struct.realtimeData), &(pMessageData->m_struct), sizeof(T_MSG_DEVICE_REALTIME_DATA));
        memcpy_s((char *)(pBroadcastData->m_struct.deviceId), sizeof(pBroadcastData->m_struct.deviceId), pDevice->id.toLocal8Bit().data(), pDevice->id.toLocal8Bit().length());
        pBroadcastData->m_struct.realtimeData.datatype = pMessageData->m_struct.data_[5];

        pBroadcastData->m_struct.data = data;

        message_ptr pMessageBroadcast = std::make_shared<CMessage>();
        pMessageBroadcast->m_header.m_struct.cmd = PC_SERVER_CMD_BROADCAST_REALTIME_DATA;
        pMessageBroadcast->m_header.m_struct.messageNo = 0;
        pMessageBroadcast->m_header.m_struct.cmdResult = 0;
        pMessageBroadcast->SetMessageBody(pBroadcastData);
        // �򾮶ӵ����߿ͻ��˹㲥��Ϣ
        pDevice->BroadcastToPCClients(pMessageBroadcast);

        LOG_INFO() << QStringLiteral("�յ�����[%1]��H2S����豸[%2]��ʵʱ�ɼ�����,�򾮶ӵ������û�[%3]�㲥ʵʱ�ɼ�����")
            .arg(pRig->GetDisplayText()).arg(pDevice->GetDisplayText())
            .arg(qstrOnlineUsers);
		
    }
	LOG_INFO() << "business_handle_service.cpp 872##" << pDevice->GetDisplayText() << "#" <<  pDevice->id;
    // ʵʱ�������
	bool ret;// = CDBService::GetInstance()->DataQueuePushBack(pDevice->id, *pMessageData);
    if (!ret) {
        retVal = ERR_DB_ERROR;// �豸δע��
        // ���ݽ��봦�����ʧ��
    } else {
        // ���ݽ��봦����гɹ�
    }
    return retVal;
}

//******************************************************
//** ������:   HandleDeviceExit
//** ���ܼ���: �����豸�˳�
//** �������: 
//   CMessage * sessionMessageIn:
//
//** ����ֵ: 
//   int:
//
//** �������ڣ�2015/07/27
//** �����ˣ�  ����
//** �޸ļ�¼��
//   ����        �汾      �޸���    �޸�����
//
//** ����˵����
//******************************************************
int CBusinessHandleService::HandleDeviceExit(tcp_session_ptr session, 
                                             message_ptr sessionMessageIn)
{
    Q_ASSERT(session);
    Q_ASSERT(sessionMessageIn);

    if (sessionMessageIn->m_header.m_struct.cmd != DEVICE_CMD_EXIT) {
        return ERR_UNMATCHED_CMD;
    }

    device_ptr pDevice = std::static_pointer_cast<CDevice>(session->m_entity);
    if (!pDevice) {
        LOG_WARING() << QStringLiteral("�ܾ�δ��¼���豸[%1]�����˳�����")
            .arg(session->get_display_text());
        return ERR_NONE;
    }

    Q_ASSERT(pDevice);

    rig_ptr pRig =  pDevice->GetRig();
    Q_ASSERT(pRig);

    // �ر���λ����ͨ�ŻỰ
    LOG_INFO() << QStringLiteral("����[%1]��H2S����豸[%2]�˳�,�رջỰ[%3]")
        .arg(pRig->GetDisplayText()).arg(pDevice->GetDisplayText())
        .arg(session->get_display_text());
    pRig->RemoveDeviceSession(session);
    session->stop();
    session->set_entity(NULL);
    pDevice->SetSession(NULL);

    // ֪ͨ���ӵ����пͻ��ˣ���ǰ��λ����״̬
    QString qstrOnlineUsers = pRig->GetOnlineUsersDisplayText();
    if (!qstrOnlineUsers.isEmpty()) {
        LOG_INFO() << QStringLiteral("�򾮶�[%1]�������û�[%2]���͹㲥֪ͨ,�ɼ��豸[%3]���˳�")
            .arg(pRig->GetDisplayText()).arg(qstrOnlineUsers).arg(pDevice->GetDisplayText());
        
        CMessageBroadcastDeviceOnlineStatus * pBroadcastMsg = new CMessageBroadcastDeviceOnlineStatus();
        CMessageGetDeviceOnlineStatusRepItem * pStatusItem = new CMessageGetDeviceOnlineStatusRepItem();
        memcpy_s((char *)(pStatusItem->m_struct.deviceID), sizeof(pStatusItem->m_struct.deviceID), pDevice->id.toLocal8Bit(), pDevice->id.toLocal8Bit().length());
        pStatusItem->m_struct.deviceOnlineStatus = DEVICE_ONLINE_STATUS_OFFLINE;// �豸����
        pBroadcastMsg->m_struct.itemCount = 0;
        pBroadcastMsg->m_lstDeviceIDs.push_back(pStatusItem);

        message_ptr broadcast_msg = std::make_shared<CMessage>();
        broadcast_msg->m_header.m_struct.cmd = PC_SERVER_CMD_BROADCAST_DEVICE_STATUS;
        broadcast_msg->SetMessageBody(pBroadcastMsg);
        pDevice->BroadcastToPCClients(broadcast_msg);
    }

    return ERR_NONE;
}

//******************************************************
//** ������:   HandleDeviceSendHeartbeat
//** ���ܼ���: ������λ���������������������ص�ǰʱ��
//** �������: 
//   tcp_session_ptr session:
//   message_ptr sessionMessageIn:
//   message_ptr responseMessageOut:
//
//** ����ֵ: 
//   int:
//
//** �������ڣ�2015/12/29
//** �����ˣ�  ����
//** �޸ļ�¼��
//   ����        �汾      �޸���    �޸�����
//
//** ����˵����
//******************************************************
int CBusinessHandleService::HandleDeviceSendHeartbeat(tcp_session_ptr session, 
                                                      message_ptr sessionMessageIn, 
                                                      message_ptr responseMessageOut)
{
    Q_ASSERT(session);

    // �յ��豸����ʱ���ط�����ʱ��ı���ʱ���ʾ��������ʱ����
    QDateTime current = QDateTime::currentDateTime();// ȡ�õ�ʱ��Ϊ��ǰʱ����ʱ��
    CMessageServerTimeBeiJing * serverTime = new CMessageServerTimeBeiJing();
    serverTime->m_struct.year = U16toBCD((U16)current.date().year());
    serverTime->m_struct.month = U8toBCD((U8)current.date().month());
    serverTime->m_struct.day = U8toBCD((U8)current.date().day());
    serverTime->m_struct.hour = U8toBCD((U8)current.time().hour());
    serverTime->m_struct.minute = U8toBCD((U8)current.time().minute());
    serverTime->m_struct.second = U8toBCD((U8)current.time().second());
    responseMessageOut->SetMessageBody(serverTime);

    device_ptr pDevice = std::static_pointer_cast<CDevice>(session->m_entity);
    if (pDevice)
    {
        rig_ptr pRig =  pDevice->GetRig();
        Q_ASSERT(pRig);
        LOG_INFO() << QStringLiteral("���յ����Ծ���[%1]�豸[%2]�����������䷵�ط�������ǰʱ��")
            .arg(pRig->GetDisplayText())
            .arg(pDevice->GetDisplayText());

        LOG_WARING() << pRig->GetDevicesOnlineStatusDisplayText();
        LOG_WARING() << QStringLiteral("����[%1]�������û�").arg(pRig->GetDisplayText())
            << pRig->GetOnlineUsersDisplayText();
    }
    else
    {
        LOG_WARING() << QStringLiteral("���յ�δ��¼�豸[%1]�����������䷵�ط�������ǰʱ��").arg(session->get_display_text());
    }

    return ERR_NONE;
}


//******************************************************
//** ������:   HandleDeviceRealTimeBatchedData
//** ���ܼ���: �����豸�����ϴ��ɼ�����
//** �������: 
//   tcp_session_ptr session:
//   message_ptr sessionMessageIn:
//
//** ����ֵ: 
//   int:
//
//** �������ڣ�2016/1/11
//** �����ˣ�  ����
//** �޸ļ�¼��
//   ����        �汾      �޸���    �޸�����
//
//** ����˵����
//******************************************************
int CBusinessHandleService::HandleDeviceRealTimeBatchedData(tcp_session_ptr session, 
                                                            message_ptr sessionMessageIn, 
                                                            message_ptr responseMessageOut)
{
    Q_ASSERT(sessionMessageIn);
	//LOG_WARING() << QStringLiteral("�豸�����ϴ�_DEVICE_CMD_REALTIME_BATCHED_DATA_%1 ### cmd_%2###").arg(DEVICE_CMD_REALTIME_BATCHED_DATA)
	//	.arg(sessionMessageIn->m_header.m_struct.cmd) << GetCurrentThreadId();
    if (sessionMessageIn->m_header.m_struct.cmd != DEVICE_CMD_REALTIME_BATCHED_DATA) {
        return ERR_UNMATCHED_CMD;
    }
    int retVal = ERR_NONE;

    // �豸�ϴ���ʵʱ����
    CMessageDeviceRealtimeBatchedData * pMessageData = static_cast<CMessageDeviceRealtimeBatchedData *>(sessionMessageIn->m_pBody);

    // ��ȡ��Ӧ���豸����
    device_ptr pDevice = std::static_pointer_cast<CDevice>(session->m_entity);
    if (!pDevice)
    {
        CMessageErrorInfo * pResponse = new CMessageErrorInfo();
        pResponse->m_qstrErrorInfo = QStringLiteral("�ܾ�δ������¼��֤���豸�����ϴ�ʵʱ����");
        LOG_WARING() << pResponse->m_qstrErrorInfo;

        message_ptr pMessageOut = std::make_shared<CMessage>();
        pMessageOut->m_header.m_struct.cmd = PC_SERVER_CMD_BROADCAST_REALTIME_DATA;
        pMessageOut->m_header.m_struct.messageNo = sessionMessageIn->m_header.m_struct.messageNo;
        pMessageOut->m_header.m_struct.cmdResult = ERR_INVALID_SESSION;
        if (sessionMessageIn->m_header.m_struct.cmdResult != 0) {
            responseMessageOut->SetMessageBody(pResponse);
        } else {
            delete pResponse;
            pResponse = NULL;
        }
        session->deliver(pMessageOut);

        return ERR_INVALID_SESSION;
    }
	LOG_WARING() << session->get_display_text() << "business_handle_service.cpp__1064##" << pDevice->id;
    Q_ASSERT(pDevice);

    // ���������ϴ�ʱ��
    pMessageData->m_struct.timestamp = QDateTime::currentMSecsSinceEpoch();

    QVector<message_device_realtime_data_ptr> vec;
    pMessageData->SplitBatchedMessage(vec);

	LOG_DEBUG() << QStringLiteral("%1�豸�����ϴ�����!").arg(pDevice->id);

	// ʵʱ�������
	bool ret = CDBService::GetInstance()->DataQueuePushBack(pDevice->id, vec);


#ifdef RIG_TEXT
   //  ��ȡ�豸��������
    rig_ptr pRig = pDevice->GetRig();
    Q_ASSERT(pRig);

    LOG_DEBUG() << QStringLiteral("�յ�����[%1]��H2S����豸[%2]�����ϴ���ʵʱ�ɼ�����[%3]")
        .arg(pRig->GetDisplayText()).arg(pDevice->GetDisplayText())
        .arg(pMessageData->ToString());

    QString qstrOnlineUsers = pRig->GetOnlineUsersDisplayText();
    if (!qstrOnlineUsers.isEmpty()) {
        // �������ϴ������ݲ�ֳɶ��֪ͨ
        for (message_device_realtime_data_ptr pRealtimeData : vec)
        {
            D64 data = Convert7ByteDataToD64(pRealtimeData->m_struct.data_);

            // ����㲥��Ϣ
            CMessageBroadcastDeviceRealtimeData * pBroadcastData = new CMessageBroadcastDeviceRealtimeData();
            int size = sizeof(pBroadcastData->m_struct.rigId);
            memcpy_s((char *)(pBroadcastData->m_struct.rigId), sizeof(pBroadcastData->m_struct.rigId), (char *)(pRig->id.toLocal8Bit().data()), pRig->id.toLocal8Bit().length());
            memcpy(&(pBroadcastData->m_struct.realtimeData), &(pRealtimeData->m_struct), sizeof(T_MSG_DEVICE_REALTIME_DATA));
            memcpy_s((char *)(pBroadcastData->m_struct.deviceId), sizeof(pBroadcastData->m_struct.deviceId), pDevice->id.toLocal8Bit().data(), pDevice->id.toLocal8Bit().length());
            pBroadcastData->m_struct.realtimeData.datatype = pRealtimeData->m_struct.data_[5];

            pBroadcastData->m_struct.data = data;

            message_ptr pMessageBroadcast = std::make_shared<CMessage>();
            pMessageBroadcast->m_header.m_struct.cmd = PC_SERVER_CMD_BROADCAST_REALTIME_DATA;
            pMessageBroadcast->m_header.m_struct.messageNo = 0;
            pMessageBroadcast->m_header.m_struct.cmdResult = 0;
            pMessageBroadcast->SetMessageBody(pBroadcastData);
            // �򾮶ӵ����߿ͻ��˹㲥��Ϣ
            pDevice->BroadcastToPCClients(pMessageBroadcast);

            LOG_INFO() << QStringLiteral("������[%1]��H2S����豸[%2]ʵʱ�ɼ�������,�򾮶ӵ������û�[%3]�㲥")
                .arg(pRig->GetDisplayText()).arg(pDevice->GetDisplayText())
                .arg(qstrOnlineUsers);
        }
    }

    for (message_device_realtime_data_ptr pRealtimeData : vec)
    {

	D64 data = 0;
    {
        U8 buf[8];
        U8 * pData = (U8 *)&(pRealtimeData->m_struct.data_);
        memcpy(buf, pData, 7);

        LOG_DEBUG() << QStringLiteral("�ɼ�����{����״̬[%1];ʵʱŨ��[%2 %3 %4];С����λ[%5];��������[%6];������λ[%7]}")
            .arg(buf[0])
            .arg(buf[1]).arg(buf[2]).arg(buf[3])
            .arg(buf[4])
            .arg(buf[5])
            .arg(buf[6]);
        data = Convert7ByteDataToD64(buf);
    }

        // ʵʱ�������
        bool ret = CDBService::GetInstance()->DataQueuePushBack(pDevice->id, *pRealtimeData);
        if (!ret) {
            retVal = ERR_DB_ERROR;// �豸δע��
			//LOG_DEBUG() << QStringLiteral("���ݽ��봦�����ʧ��");
            // ���ݽ��봦�����ʧ��
        } else {
            // ���ݽ��봦����гɹ�
			//LOG_DEBUG() << QStringLiteral("���ݽ��봦����гɹ�");
        }
    }
#endif 
    return retVal;
}

//******************************************************
//** ������:   HandlePCClientSetDeviceParam
//** ���ܼ���: ����PC�ͻ��������豸���� 
//** �������: 
//   tcp_session_ptr session:
//   message_ptr sessionMessageIn:
//   message_ptr responseMessageOut:
//
//** ����ֵ: 
//   int:
//
//** �������ڣ�2015/08/04
//** �����ˣ�  ����
//** �޸ļ�¼��
//   ����        �汾      �޸���    �޸�����
//
//** ����˵����
//******************************************************
int CBusinessHandleService::HandlePCClientSetDeviceParam(tcp_session_ptr session, 
                                                         message_ptr sessionMessageIn, 
                                                         message_ptr responseMessageOut)
{
    Q_ASSERT(session);
    Q_ASSERT(sessionMessageIn);

    int retVal = ERR_NONE;
    CMessageErrorInfo * pRepErrorInfo = NULL;
    CMessageSetDeviceParam * pParam = NULL;
    rig_ptr pRig;
    device_ptr pDevice;
    user_ptr pUser = static_pointer_cast<CUser>(session->m_entity);
    
    if (!pUser) {
        retVal = ERR_INVALID_SESSION;
        pRepErrorInfo = new CMessageErrorInfo();
        pRepErrorInfo->m_qstrErrorInfo = QStringLiteral("�ܾ�δ������¼��֤�Ŀͻ���[�����豸����");
        responseMessageOut->SetMessageBody(pRepErrorInfo);
    } else {
        pRig = static_pointer_cast<CRig>(pUser->GetRig());
        Q_ASSERT(pRig);

        pParam = static_cast<CMessageSetDeviceParam *>(sessionMessageIn->m_pBody);
        Q_ASSERT(pParam);

        QString qstrDeviceID(QByteArray((char *)(pParam->m_struct.deviceID), sizeof(pParam->m_struct.deviceID)));
        pDevice = pRig->FindDeviceByID(qstrDeviceID);

        if (!pDevice) {
            retVal = ERR_INVALID_ID;
            pRepErrorInfo = new CMessageErrorInfo();
            pRepErrorInfo->m_qstrErrorInfo = QStringLiteral("�豸ID��Ч");
            responseMessageOut->SetMessageBody(pRepErrorInfo);
        } else {
            // �����豸��
            bool updateDeviceNameSucceed = CDBService::GetInstance()->SetDeviceName(qstrDeviceID, pParam->GetDeviceName());
            if (!updateDeviceNameSucceed) {
                retVal = ERR_DB_ERROR;
                pRepErrorInfo = new CMessageErrorInfo();
                pRepErrorInfo->m_qstrErrorInfo = QStringLiteral("���ݿ�����豸���Ƴ���");
                responseMessageOut->SetMessageBody(pRepErrorInfo);
            } else {
                // �������ݿ���ָ���豸���豸����
                bool ret = CDBService::GetInstance()->SetDeviceParam(pDevice->deviceType, qstrDeviceID, *pParam);
                if (!ret) {
                    retVal = ERR_DB_ERROR;
                    pRepErrorInfo = new CMessageErrorInfo();
                    pRepErrorInfo->m_qstrErrorInfo = QStringLiteral("���ݿ�����豸��������");
                    responseMessageOut->SetMessageBody(pRepErrorInfo);
                } else {
                    pDevice->warningLine1st = QString::fromLocal8Bit((char *)(pParam->m_struct.warningLine1st), strlen((char *)pParam->m_struct.warningLine1st));
                    pDevice->warningLine2nd = QString::fromLocal8Bit((char *)(pParam->m_struct.warningLine2nd), strlen((char *)pParam->m_struct.warningLine2nd));
                    pDevice->maxRange = QString::fromLocal8Bit((char *)(pParam->m_struct.maxRange), strlen((char *)pParam->m_struct.maxRange));
                    pDevice->name = pParam->GetDeviceName();
                    pDevice->sampleRate = pParam->GetSampleRate();
                    if (pRig) {
                        emit pRig->rigDeviceUpdated();
                    }
                }
            }
        }
    }

    if (ERR_NONE != retVal && pRig && pUser) {
        LOG_WARING() << QStringLiteral("����[%1]���û�[%2]�����豸����ʧ��,������Ϣ[%3]")
            .arg(pRig->GetDisplayText()).arg(pUser->GetDisplayText())
            .arg(pRepErrorInfo->m_qstrErrorInfo);
    } else {
        LOG_INFO() << QStringLiteral("����[%1]���û�[%2]����H2S����豸[%3]�Ĳ����ɹ�����������[%4]")
            .arg(pRig->GetDisplayText()).arg(pUser->GetDisplayText())
            .arg(pDevice->GetDisplayText()).arg(pParam->ToString());
    }

    return retVal;
}


//******************************************************
//** ������:   HandlePCClientSendHeartbeat
//** ���ܼ���: ����PC�ͻ��˷�������
//** �������: 
//   tcp_session_ptr session:
//   message_ptr sessionMessageIn:
//   message_ptr responseMessageOut:
//
//** ����ֵ: 
//   int:
//
//** �������ڣ�2015/12/28
//** �����ˣ�  ����
//** �޸ļ�¼��
//   ����        �汾      �޸���    �޸�����
//
//** ����˵����
//******************************************************
int CBusinessHandleService::HandlePCClientSendHeartbeat(tcp_session_ptr session, 
                                                        message_ptr sessionMessageIn, 
                                                        message_ptr responseMessageOut)
{
    Q_ASSERT(session);
    Q_ASSERT(sessionMessageIn);

    int retVal = ERR_NONE;
    CMessageErrorInfo * pRepErrorInfo = NULL;
    user_ptr pUser = static_pointer_cast<CUser>(session->m_entity);
    if (!pUser) {
        retVal = ERR_INVALID_SESSION;
        pRepErrorInfo = new CMessageErrorInfo();
        pRepErrorInfo->m_qstrErrorInfo = QStringLiteral("�ܾ�δ������¼��֤�Ŀͻ���[��������]");
        responseMessageOut->SetMessageBody(pRepErrorInfo);
        LOG_WARING() << QStringLiteral("�ܾ�δ������¼��֤�Ŀͻ���[%1][��������]").arg(session->get_display_text());
    } else {
        CMessageHeartbeat * heartbeat = new CMessageHeartbeat();
        heartbeat->m_struct.heartbeatTime = QDateTime::currentMSecsSinceEpoch();
        responseMessageOut->SetMessageBody(heartbeat);
        LOG_INFO() << QStringLiteral("�û�[%2]��������").arg(pUser->GetDisplayText());
    }

    return retVal;
}


//******************************************************
//** ������:   HandlePCClientGetHistoryDeviceDataCount
//** ���ܼ���: ����PC��ͻ��˻�ȡ��ʷ�豸��������
//** �������: 
//   tcp_session_ptr session:
//   message_ptr sessionMessageIn:
//   message_ptr responseMessageOut:
//
//** ����ֵ: 
//   int:
//
//** �������ڣ�2016/1/4
//** �����ˣ�  ����
//** �޸ļ�¼��
//   ����        �汾      �޸���    �޸�����
//
//** ����˵����
//******************************************************
int CBusinessHandleService::HandlePCClientGetHistoryDeviceDataCount(tcp_session_ptr session, 
                                                                    message_ptr sessionMessageIn, 
                                                                    message_ptr responseMessageOut)
{
    Q_ASSERT(session);
    Q_ASSERT(sessionMessageIn);

    int retVal = ERR_NONE;
    CMessageErrorInfo * pRepErrorInfo = NULL;
    CMessageGetDeviceDataCountReq * pReq = NULL;
    rig_ptr pRig;
    user_ptr pUser = static_pointer_cast<CUser>(session->m_entity);
    if (!pUser) {
        retVal = ERR_INVALID_SESSION;
        pRepErrorInfo = new CMessageErrorInfo();
        pRepErrorInfo->m_qstrErrorInfo = QStringLiteral("�ܾ�δ������¼��֤�Ŀͻ��˻�ȡ��ʷ��������");
        responseMessageOut->SetMessageBody(pRepErrorInfo);
        LOG_WARING() << QStringLiteral("�ܾ�δ������¼��֤�Ŀͻ���[%1]��ȡ��ʷ��������").arg(session->get_display_text());
        return retVal;
    } else {
        pRig = static_pointer_cast<CRig>(pUser->GetRig());
        Q_ASSERT(pRig);

        pReq = static_cast<CMessageGetDeviceDataCountReq *>(sessionMessageIn->m_pBody);
        Q_ASSERT(pReq);

        QDateTime begin = QDateTime::fromMSecsSinceEpoch(pReq->m_struct.begin);
        QDateTime end = QDateTime::fromMSecsSinceEpoch(pReq->m_struct.end);
        if (begin.date() != end.date()) {// �����ֹʱ���Ƿ�Ϊͬһ��
            retVal = ERR_INVALID_PERIOD;
            pRepErrorInfo = new CMessageErrorInfo();
            pRepErrorInfo->m_qstrErrorInfo = QStringLiteral("��ʼʱ�������ʱ�䲻��ͬһ��");
            responseMessageOut->SetMessageBody(pRepErrorInfo);
            LOG_WARING() << QStringLiteral("����[%1]���û�[%2]�����ȡ��ʷ������������ʼʱ��[%2]�����ʱ��[%2]����ͬһ��")
                .arg(pRig->GetDisplayText()).arg(pUser->GetDisplayText())
                .arg(begin.toString("yyyy-MM-dd HH:mm:ss.zzz"))
                .arg(end.toString("yyyy-MM-dd HH:mm:ss.zzz"));
            return retVal;
        }

        // ����Ƿ�Ϊ��ѯ�����豸������
        QString qstrDeviceID = pReq->GetDeviceID();
        QVector<QString> vecDeviceIDs;
        if (!qstrDeviceID.isEmpty()) {
            vecDeviceIDs.push_back(qstrDeviceID);
        } else {
            // δָ���豸ID�����ѯ�����������豸����ʷ����
            QList<CDeviceInfo> lst;
            bool ret = CDBService::GetInstance()->GetDeviceListByRigID(pRig->id, lst);
            if (ret) {
                for (CDeviceInfo info : lst) {
                    vecDeviceIDs.push_back(info.id);
                }
            }
        }

        int count = 0;
        bool ret = CDBService::GetInstance()->GetDeviceHistoryDataCount(pReq->m_struct.begin, pReq->m_struct.end, vecDeviceIDs, count);
        if (!ret) {
            retVal = ERR_DB_ERROR;
            pRepErrorInfo = new CMessageErrorInfo();
            pRepErrorInfo->m_qstrErrorInfo = QStringLiteral("���������ݿ��������");
            responseMessageOut->SetMessageBody(pRepErrorInfo);
            LOG_WARING() << QStringLiteral("����[%1]���û�[%2]�����ȡ��ʷ���������������ݿ��ȡ����ʱ����")
                .arg(pRig->GetDisplayText()).arg(pUser->GetDisplayText());
            return retVal;
        } else {
            CMessageGetDeviceDataCountRep * pRep = new CMessageGetDeviceDataCountRep();
            pRep->m_struct.deivceDataCount = count;
            responseMessageOut->SetMessageBody(pRep);
            LOG_INFO() << QStringLiteral("����[%1]���û�[%2]�����ȡ[%3 �� %4]֮�����ʷ���������������ݿ��ѯ��[%5]��")
                .arg(pRig->GetDisplayText()).arg(pUser->GetDisplayText())
                .arg(begin.toString("yyyy-MM-dd HH:mm:ss.zzz")).arg(end.toString("yyyy-MM-dd HH:mm:ss.zzz"))
                .arg(count);
        }
    }

    return retVal;
}


//******************************************************
//** ������:   HandlePCClientGetHistoryDeviceData
//** ���ܼ���: ����PC�ͻ��˻�ȡ��ʷ�豸����
//** �������: 
//   tcp_session_ptr session:
//   message_ptr sessionMessageIn:
//   message_ptr responseMessageOut:
//
//** ����ֵ: 
//   int:
//
//** �������ڣ�2016/1/4
//** �����ˣ�  ����
//** �޸ļ�¼��
//   ����        �汾      �޸���    �޸�����
//
//** ����˵����
//******************************************************
int CBusinessHandleService::HandlePCClientGetHistoryDeviceData(tcp_session_ptr session, 
                                                               message_ptr sessionMessageIn, 
                                                               message_ptr responseMessageOut)
{
    Q_ASSERT(session);
    Q_ASSERT(sessionMessageIn);

    int retVal = ERR_NONE;
    CMessageErrorInfo * pRepErrorInfo = NULL;
    CMessageGetDeviceDataReq * pReq;
    rig_ptr pRig;
    user_ptr pUser = static_pointer_cast<CUser>(session->m_entity);
    if (!pUser) {
        retVal = ERR_INVALID_SESSION;
        pRepErrorInfo = new CMessageErrorInfo();
        pRepErrorInfo->m_qstrErrorInfo = QStringLiteral("�ܾ�δ������¼��֤�Ŀͻ��˻�ȡ��ʷ����");
        responseMessageOut->SetMessageBody(pRepErrorInfo);
        LOG_WARING() << QStringLiteral("�ܾ�δ������¼��֤�Ŀͻ���[%1]��ȡ��ʷ����").arg(session->get_display_text());
        return retVal;
    }

    pRig = static_pointer_cast<CRig>(pUser->GetRig());
    Q_ASSERT(pRig);

    pReq = static_cast<CMessageGetDeviceDataReq *>(sessionMessageIn->m_pBody);
    Q_ASSERT(pReq);

    // ��ȡ���ݵ�ʱ��α�����ͬһ����
    QDateTime begin = QDateTime::fromMSecsSinceEpoch(pReq->m_struct.begin);
    QDateTime end = QDateTime::fromMSecsSinceEpoch(pReq->m_struct.end);
    if (begin.date() != end.date()) {
        retVal = ERR_INVALID_PERIOD;
        pRepErrorInfo = new CMessageErrorInfo();
        pRepErrorInfo->m_qstrErrorInfo = QStringLiteral("��ʼʱ�������ʱ�䲻��ͬһ��");
        responseMessageOut->SetMessageBody(pRepErrorInfo);
        LOG_WARING() << QStringLiteral("����[%1]���û�[%2]�����ȡ��ʷ���ݣ���ʼʱ��[%2]�����ʱ��[%2]����ͬһ��")
            .arg(pRig->GetDisplayText()).arg(pUser->GetDisplayText())
            .arg(begin.toString("yyyy-MM-dd HH:mm:ss.zzz"))
            .arg(end.toString("yyyy-MM-dd HH:mm:ss.zzz"));
        return retVal;
    }

    // ����ҳ���Ƿ�Ϊ0
    if (pReq->m_struct.pageIndex == 0) {
        retVal = ERR_INVALID_PARAM;
        pRepErrorInfo = new CMessageErrorInfo();
        pRepErrorInfo->m_qstrErrorInfo = QStringLiteral("�����ҳ�Ų���Ϊ0");
        responseMessageOut->SetMessageBody(pRepErrorInfo);
        LOG_WARING() << QStringLiteral("����[%1]���û�[%2]�����ȡ��ʷ���ݣ���ҳ��Ϊ0")
            .arg(pRig->GetDisplayText()).arg(pUser->GetDisplayText());
        return retVal;
    }

    // ���ÿҳ�ļ�¼���Ƿ�Ϊ0
    if (pReq->m_struct.rowsOfPerPage == 0) {
        retVal = ERR_INVALID_PARAM;
        pRepErrorInfo = new CMessageErrorInfo();
        pRepErrorInfo->m_qstrErrorInfo = QStringLiteral("ÿҳ�ļ�¼������ָ��Ϊ0");
        responseMessageOut->SetMessageBody(pRepErrorInfo);
        LOG_WARING() << QStringLiteral("����[%1]���û�[%2]�����ȡ��ʷ���ݣ�ָ��ÿҳ���ؼ�¼��Ϊ0")
            .arg(pRig->GetDisplayText()).arg(pUser->GetDisplayText());
        return retVal;
    }

    // ����Ƿ�Ϊ��ѯ�����豸������
    QString qstrDeviceID = pReq->GetDeviceID();
    QVector<QString> vecDeviceIDs;
    if (!qstrDeviceID.isEmpty()) {
        vecDeviceIDs.push_back(qstrDeviceID);
    } else {
        // δָ���豸ID�����ѯ�����������豸����ʷ����
        QList<CDeviceInfo> lst;
        bool ret = CDBService::GetInstance()->GetDeviceListByRigID(pRig->id, lst);
        if (ret) {
            for (CDeviceInfo info : lst) {
                vecDeviceIDs.push_back(info.id);
            }
        }
    }

    QList<CMessageDeviceDataItem *> lst;
    bool ret = CDBService::GetInstance()->GetDeviceHistoryData(pReq->m_struct.begin, pReq->m_struct.end, 
        pReq->m_struct.pageIndex, pReq->m_struct.rowsOfPerPage, vecDeviceIDs, lst);
    if (!ret) {
        retVal = ERR_DB_ERROR;
        pRepErrorInfo = new CMessageErrorInfo();
        pRepErrorInfo->m_qstrErrorInfo = QStringLiteral("���������ݿ��������");
        responseMessageOut->SetMessageBody(pRepErrorInfo);
        LOG_WARING() << QStringLiteral("����[%1]���û�[%2]�����ȡ��ʷ���ݣ������ݿ��ȡ����ʱ����")
            .arg(pRig->GetDisplayText()).arg(pUser->GetDisplayText());
        return retVal;
    } else {
        CMessageGetDeviceDataRep * pRep = new CMessageGetDeviceDataRep();
        pRep->m_lstDeviceData = lst;
        pRep->m_struct.pageIndex = pReq->m_struct.pageIndex;
        pRep->m_struct.rowsOfCurrentPage = lst.count();
        responseMessageOut->SetMessageBody(pRep);
        LOG_INFO() << QStringLiteral("����[%1]���û�[%2]�����ȡ[%3��%4֮���]��ʷ���ݣ�ÿҳ[%5]������ȡ��[%6]ҳ�����ݣ�������[%7]������")
            .arg(pRig->GetDisplayText()).arg(pUser->GetDisplayText()).arg(begin.toString("yyyy-MM-dd HH:mm:ss.zzz"))
            .arg(end.toString("yyyy-MM-dd HH:mm:ss.zzz")).arg(pReq->m_struct.rowsOfPerPage).arg(pReq->m_struct.pageIndex)
            .arg(lst.count());
    }

    return retVal;
}

//******************************************************
//** ������:   HandleTcpSessionClosed
//** ���ܼ���: ����socket���ӶϿ�
//** �������: 
//   tcp_session_ptr session:
//
//** ����ֵ: 
//   void:
//
//** �������ڣ�2015/08/05
//** �����ˣ�  ����
//** �޸ļ�¼��
//   ����        �汾      �޸���    �޸�����
//
//** ����˵����
//******************************************************
void CBusinessHandleService::HandleTcpSessionClosed(tcp_session_ptr session)
{
    Q_ASSERT(session);

    base_entity_ptr pEntity = session->m_entity;
    if (pEntity) {
        switch (pEntity->type)
        {
        case ENTITY_USER:
            HandlePCClientSessionClosed(session);
            break;

        case ENTITY_DEVICE:
            HandleDeviceSessionClosed(session);
            break;

        default:
            break;
        }
    }

    session->stop();
    session->set_entity(NULL);
}

//******************************************************
//** ������:   HandlePCClientSessionClosed
//** ���ܼ���: 
//** �������: 
//   tcp_session_ptr session:
//
//** ����ֵ: 
//   void:
//
//** �������ڣ�2015/08/05
//** �����ˣ�  ����
//** �޸ļ�¼��
//   ����        �汾      �޸���    �޸�����
//
//** ����˵����
//******************************************************
void CBusinessHandleService::HandlePCClientSessionClosed(tcp_session_ptr session)
{
    Q_ASSERT(session);
    Q_ASSERT(session->m_entity);

    user_ptr pUser = std::static_pointer_cast<CUser>(session->m_entity);
    Q_ASSERT(pUser);

    rig_ptr pRig = std::static_pointer_cast<CRig>(pUser->GetRig());
    Q_ASSERT(pRig);

    LOG_WARING() << QStringLiteral("����[%1]���û�[%2]�쳣���ߣ��Զ��ͷ�����[%3]")
        .arg(pRig->GetDisplayText()).arg(pUser->GetDisplayText())
        .arg(session->get_display_text());

    // �ر�socket����socket�Ӿ��ӵĿͻ��˻Ự�����Ƴ�
    pRig->RemoveClientSession(session);
    pUser->SetSession(NULL);

    LOG_WARING() << pRig->GetDevicesOnlineStatusDisplayText();
    LOG_WARING() << QStringLiteral("����[%1]�������û�").arg(pRig->GetDisplayText())
        << pRig->GetOnlineUsersDisplayText();
}

//******************************************************
//** ������:   HandleDeviceSessionClosed
//** ���ܼ���: 
//** �������: 
//   tcp_session_ptr session:
//
//** ����ֵ: 
//   void:
//
//** �������ڣ�2015/08/05
//** �����ˣ�  ����
//** �޸ļ�¼��
//   ����        �汾      �޸���    �޸�����
//
//** ����˵����
//******************************************************
void CBusinessHandleService::HandleDeviceSessionClosed(tcp_session_ptr session)
{
    Q_ASSERT(session);
    Q_ASSERT(session->m_entity);

    device_ptr pDevice = std::static_pointer_cast<CDevice>(session->m_entity);
    Q_ASSERT(pDevice);

    rig_ptr pRig = std::static_pointer_cast<CRig>(pDevice->GetRig());
    Q_ASSERT(pRig);

    LOG_WARING() << QStringLiteral("����[%1]�Ĳɼ��豸[%2]�쳣���ߣ��Զ��ͷ�����[%3]")
        .arg(pRig->GetDisplayText()).arg(pDevice->GetDisplayText())
        .arg(session->get_display_text());

    // �ر�socket���� ���豸�Ự�Ӿ��ӵ��豸�Ự�����Ƴ�
    pRig->RemoveDeviceSession(session);
    session->set_entity(NULL);
    pDevice->SetSession(NULL);

    QString qstrOnlineUsers = pRig->GetOnlineUsersDisplayText();
    if (!qstrOnlineUsers.isEmpty()) {
        LOG_INFO() << QStringLiteral("�򾮶�[%1]�������û�[%2]�����豸[%3]�쳣����֪ͨ")
            .arg(pRig->GetDisplayText()).arg(qstrOnlineUsers).arg(pDevice->GetDisplayText());
        CMessageBroadcastDeviceOnlineStatus * pBroadcastMsg = new CMessageBroadcastDeviceOnlineStatus();
        CMessageGetDeviceOnlineStatusRepItem * pStatusItem = new CMessageGetDeviceOnlineStatusRepItem();
        memcpy_s((char *)(pStatusItem->m_struct.deviceID), sizeof(pStatusItem->m_struct.deviceID), pDevice->id.toLocal8Bit(), pDevice->id.toLocal8Bit().length());
        pStatusItem->m_struct.deviceOnlineStatus = DEVICE_ONLINE_STATUS_OFFLINE;// �豸����
        pBroadcastMsg->m_struct.itemCount = 0;
        pBroadcastMsg->m_lstDeviceIDs.push_back(pStatusItem);

        message_ptr broadcast_msg = std::make_shared<CMessage>();
        broadcast_msg->m_header.m_struct.cmd = PC_SERVER_CMD_BROADCAST_DEVICE_STATUS;
        broadcast_msg->SetMessageBody(pBroadcastMsg);
        pDevice->BroadcastToPCClients(broadcast_msg);
    }
}











