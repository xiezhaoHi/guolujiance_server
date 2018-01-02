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
    // 处理PC客户端命令

    // 处理登录消息的回复
    CTcpSessionMessageHandlerHub::GetInstance()->RegisterSessionMessageHandler(PC_CLIENT_CMD_LOGIN, this, 
        static_cast<ITcpSessionMessageHandler::HandleSessionMessageV2>(&CBusinessHandleService::HandlePCClientLogin));
    // 获取设备列表
    CTcpSessionMessageHandlerHub::GetInstance()->RegisterSessionMessageHandler(PC_CLIENT_CMD_GET_DEVICE_LIST, this, 
        static_cast<ITcpSessionMessageHandler::HandleSessionMessageV2>(&CBusinessHandleService::HandlePCClientGetDeviceList));
    // 获取设备详情
    CTcpSessionMessageHandlerHub::GetInstance()->RegisterSessionMessageHandler(PC_CLIENT_CMD_GET_DEVICE_INFO, this, 
        static_cast<ITcpSessionMessageHandler::HandleSessionMessageV2>(&CBusinessHandleService::HandlePCClientGetDeviceInfo));
    // 客户端退出
    CTcpSessionMessageHandlerHub::GetInstance()->RegisterSessionMessageHandler(PC_CLIENT_CMD_EXIT, this, 
        static_cast<ITcpSessionMessageHandler::HandleSessionMessage>(&CBusinessHandleService::HandlePCClientExit));
    // 设置设备参数
    CTcpSessionMessageHandlerHub::GetInstance()->RegisterSessionMessageHandler(PC_CLIENT_CMD_SET_DEVICE_PARAM, this, 
        static_cast<ITcpSessionMessageHandler::HandleSessionMessageV2>(&CBusinessHandleService::HandlePCClientSetDeviceParam));
    // 获取设备在线状态
    CTcpSessionMessageHandlerHub::GetInstance()->RegisterSessionMessageHandler(PC_CLIENT_CMD_GET_DEVICE_STATUS, this, 
        static_cast<ITcpSessionMessageHandler::HandleSessionMessageV2>(&CBusinessHandleService::HandlePCClientGetDeviceOnlineStatus));
    // 客户端心跳
    CTcpSessionMessageHandlerHub::GetInstance()->RegisterSessionMessageHandler(PC_CLIENT_CMD_SEND_HEARTBEAT, this, 
        static_cast<ITcpSessionMessageHandler::HandleSessionMessageV2>(&CBusinessHandleService::HandlePCClientSendHeartbeat));
    // 客户端请求获取历史数据的条数
    CTcpSessionMessageHandlerHub::GetInstance()->RegisterSessionMessageHandler(PC_CLIENT_CMD_GET_HISTORY_DEVICE_DATA_COUNT, this, 
        static_cast<ITcpSessionMessageHandler::HandleSessionMessageV2>(&CBusinessHandleService::HandlePCClientGetHistoryDeviceDataCount));
    // 客户端请求获取历史数据
    CTcpSessionMessageHandlerHub::GetInstance()->RegisterSessionMessageHandler(PC_CLIENT_CMD_GET_HISTORY_DEVICE_DATA, this, 
        static_cast<ITcpSessionMessageHandler::HandleSessionMessageV2>(&CBusinessHandleService::HandlePCClientGetHistoryDeviceData));

    //////////////////////////////////////////////////////////////////////////
    // 处理设备命令

    // 设备登录
    CTcpSessionMessageHandlerHub::GetInstance()->RegisterSessionMessageHandler(DEVICE_CMD_LOGIN, this, 
        static_cast<ITcpSessionMessageHandler::HandleSessionMessageV2>(&CBusinessHandleService::HandleDeviceLogin));
    // 设备上传实时采集数据
    CTcpSessionMessageHandlerHub::GetInstance()->RegisterSessionMessageHandler(DEVICE_CMD_REALTIME_DATA, this, 
        static_cast<ITcpSessionMessageHandler::HandleSessionMessage>(&CBusinessHandleService::HandleDeviceRealTimeData));
    // 设备批量上传实时采集数据
    CTcpSessionMessageHandlerHub::GetInstance()->RegisterSessionMessageHandler(DEVICE_CMD_REALTIME_BATCHED_DATA, this, 
        static_cast<ITcpSessionMessageHandler::HandleSessionMessageV2>(&CBusinessHandleService::HandleDeviceRealTimeBatchedData));
    // 设备退出
    CTcpSessionMessageHandlerHub::GetInstance()->RegisterSessionMessageHandler(DEVICE_CMD_EXIT, this, 
        static_cast<ITcpSessionMessageHandler::HandleSessionMessage>(&CBusinessHandleService::HandleDeviceExit));
    // 设备心跳，获取服务器时间
    CTcpSessionMessageHandlerHub::GetInstance()->RegisterSessionMessageHandler(DEVICE_CMD_SEND_HEARTBEAT, this, 
        static_cast<ITcpSessionMessageHandler::HandleSessionMessageV2>(&CBusinessHandleService::HandleDeviceSendHeartbeat));

    CBusinessModel::GetInstance();// 业务模型类

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
//** 函数名:   HandleMessageLogin
//** 功能简述: 处理PC客户端登录消息
//** 输入参数: 
//   CMessage * sessionMessageIn:请求消息
//   CMessage & responseMessageOut:回复消息
//
//** 返回值: 
//   int:成功时返回ERR_NONE，否则返回相应的错误码
//
//** 创建日期：2015/07/26
//** 创建人：  唐桥
//** 修改记录：
//   日期        版本      修改人    修改内容
//
//** 其它说明：
//******************************************************
int CBusinessHandleService::HandlePCClientLogin(tcp_session_ptr session, 
                                                message_ptr sessionMessageIn,
                                                message_ptr responseMessageOut)
{
    // 软件授权验证
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
            retVal = ERR_INVALID_ACCOUNT;// 用户名不存在
            pRepErrorInfo = new CMessageErrorInfo();
            pRepErrorInfo->m_qstrErrorInfo = QStringLiteral("用户名不存在");
            responseMessageOut->SetMessageBody(pRepErrorInfo);
        } else {
            retVal = ERR_UNMATCHED_ACCOUNT;// 用户名、密码不匹配
            pRepErrorInfo = new CMessageErrorInfo();
            pRepErrorInfo->m_qstrErrorInfo = QStringLiteral("用户名、密码不匹配");
            responseMessageOut->SetMessageBody(pRepErrorInfo);
        }
    } else {
        QString accountID;// 用户ID
        ret = CDBService::GetInstance()->GetAccountID(pMessageLogin->m_qstrAccount, accountID);
        if (!ret || accountID.isEmpty()) {
            retVal = ERR_DB_ERROR;// 数据库记录错误
            pRepErrorInfo = new CMessageErrorInfo();
            pRepErrorInfo->m_qstrErrorInfo = QStringLiteral("数据库记录错误");
            responseMessageOut->SetMessageBody(pRepErrorInfo);
        } else {
            // 用户登录成功后，设置tcp连接为已验证状态，将用户与所属井队绑定

            // 获取用户所属井队、所属油田
            oil_field_ptr pOilField = NULL;
            rig_ptr pRig = NULL;
            user_ptr pUser = NULL;

            QString qstrRigID;
            ret = CDBService::GetInstance()->GetRigIDByAccountID(accountID, qstrRigID);
            if (ret) {
                QString qstrOilFieldID;
                ret = CDBService::GetInstance()->GetOilFieldIDByRigID(qstrRigID, qstrOilFieldID);
                if (ret) {
                    // 获取油田对象
                    pOilField = CBusinessModel::GetInstance()->FindOilFieldByID(qstrOilFieldID);
                    if (!pOilField) {
                        pOilField = CBusinessModel::GetInstance()->GetOrCreateOilField(qstrOilFieldID);
                    }

                    Q_ASSERT(pOilField);

                    // 获取井队对象
                    pRig = pOilField->FindRigByID(qstrRigID);
                    if (!pRig) {
                        pRig = pOilField->GetOrCreateRig(qstrRigID);
                    }

                    Q_ASSERT(pRig);

                    // 获取用户对象
                    pUser = pRig->FindUserByID(accountID);
                    if (!pUser) {
                        pUser = pRig->GetOrCreateUser(accountID);
                        pUser->name = pMessageLogin->m_qstrAccount;
                    }

                    Q_ASSERT(pUser);

                    pUser->SetSession(session);// 设置客户端会话，内部自动将其加入井队的在线客户端列表
                    // 记录用户登录时间
                    pUser->SetLoginTime(std::make_shared<QDateTime>(QDateTime::currentDateTime()));

                    // 设置回复消息
                    CMessageLoginRep * pResponse = new CMessageLoginRep();
                    // 获取当前服务器时间，并设置
                    pResponse->m_struct.serverTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
                    responseMessageOut->SetMessageBody(pResponse);

                    LOG_WARING() << pRig->GetDevicesOnlineStatusDisplayText();
                    LOG_WARING() << QStringLiteral("井队[%1]的在线用户").arg(pRig->GetDisplayText())
                        << pRig->GetOnlineUsersDisplayText();
                }
            }
            else
            {
                retVal = ERR_INVALID_ACCOUNT;// 用户名不存在
                pRepErrorInfo = new CMessageErrorInfo();
                pRepErrorInfo->m_qstrErrorInfo = QStringLiteral("用户非任何井队人员");
                responseMessageOut->SetMessageBody(pRepErrorInfo);
            }
        }
    }

    if (retVal != ERR_NONE) {
        Q_ASSERT(pRepErrorInfo);

        LOG_WARING() << QStringLiteral("PC客户端登录失败 错误：") << pRepErrorInfo->m_qstrErrorInfo
            << QStringLiteral("{登录名:") << pMessageLogin->m_qstrAccount
            << QStringLiteral(", 密码:") << pMessageLogin->m_qstrPassword 
            << QStringLiteral("}");
    } else  {
        LOG_INFO() << QStringLiteral("PC客户端登录成功 ")
            << QStringLiteral("{登录名:") << pMessageLogin->m_qstrAccount
            << QStringLiteral(", 密码:") << pMessageLogin->m_qstrPassword 
            << QStringLiteral("}");
    }

    return retVal;
}



//******************************************************
//** 函数名:   HandlePCClientGetDeviceList
//** 功能简述: 返回所有设备ID列表
//** 输入参数: 
//   CMessage * sessionMessageIn:
//   CMessage & responseMessageOut:
//
//** 返回值: 
//   int:
//
//** 创建日期：2015/07/27
//** 创建人：  唐桥
//** 修改记录：
//   日期        版本      修改人    修改内容
//
//** 其它说明：
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
        pRepErrorInfo->m_qstrErrorInfo = QStringLiteral("拒绝非经过登录验证的客户端获取设备列表");
        responseMessageOut->SetMessageBody(pRepErrorInfo);
    } else {// 再由用户ID获取到用户能访问的所有设备ID
        Q_ASSERT(pUser);
        QString qstrUserID = pUser->id;// 由session获取到用户ID

        // 获取本账号所在井队的H2S设备ID列表
        bool ret = CDBService::GetInstance()->GetH2SDeviceListByPersonID(qstrUserID, lstDeviceItems);
        if (!ret) {
            retVal = ERR_INVALID_ACCOUNT;
            pRepErrorInfo = new CMessageErrorInfo();
            pRepErrorInfo->m_qstrErrorInfo = QStringLiteral("根据用户名找不到可以使用的设备列表");
            responseMessageOut->SetMessageBody(pRepErrorInfo);
        } else {
            pRig = pUser->GetRig();
            Q_ASSERT(pRig);

            // 更新业务实体模型里的设备信息
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

            // 用CMessageGetDeviceListRep返回结果
            CMessageGetDeviceListRep * pResponse = new CMessageGetDeviceListRep();
            pResponse->m_lstDevices = lstDeviceItems;
            pResponse->m_struct.deviceCount = lstDeviceItems.count();
            // 设置回复消息内容
            responseMessageOut->SetMessageBody(pResponse);
        }
    }

    if (retVal != ERR_NONE) {
        Q_ASSERT(pRepErrorInfo);

        if (pUser) {
            LOG_WARING() << QStringLiteral("PC客户端[%1]查询设备列表失败,错误信息：%2")
                .arg(pUser->GetDisplayText()).arg(pRepErrorInfo->m_qstrErrorInfo);
        } else {
            LOG_WARING() << QStringLiteral("PC客户端[%1]查询设备列表失败,错误信息：%2")
                .arg(session->get_display_text())<< pRepErrorInfo->m_qstrErrorInfo;
        }
    } else  {
        Q_ASSERT(pUser);
        LOG_INFO() << QStringLiteral("PC客户端[%1]查询设备列表成功,共返回[%2]条设备信息")
            .arg(pUser->GetDisplayText()).arg(lstDeviceItems.size());
    }

    return retVal;
}

//******************************************************
//** 函数名:   HandlePCClientGetDeviceInfo
//** 功能简述: 返回给定设备ID集合对应的设备详情
//** 输入参数: 
//   CMessage * sessionMessageIn:
//   CMessage & responseMessageOut:
//
//** 返回值: 
//   int:
//
//** 创建日期：2015/07/27
//** 创建人：  唐桥
//** 修改记录：
//   日期        版本      修改人    修改内容
//
//** 其它说明：
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
        pRepErrorInfo->m_qstrErrorInfo = QStringLiteral("拒绝未经过登录验证的客户端获取设备详细信息");
        responseMessageOut->SetMessageBody(pRepErrorInfo);
    } else {
        Q_ASSERT(pUser);
        // 用CMessageGetDeviceListRep返回结果
        CMessageGetDevicesInfoRep * pResponse = new CMessageGetDevicesInfoRep();
        QString qstrUserID = pUser->id;// 由session获取到设备ID
        
        bool ret = true;
        for (int i = 0; i < pRequest->m_struct.deviceCount; i++) {
            CMessageDeviceID * pDeviceID = pRequest->m_lstDeviceID.at(i);
            qstrDeviceID = QByteArray((char *)&(pDeviceID->m_struct), sizeof(T_MSG_DEVICE_ID));

            CMessageDeviceItemInfo * pDeviceInfo = new CMessageDeviceItemInfo();
            ret = CDBService::GetInstance()->GetDeviceInfoByDeviceID(qstrDeviceID, pDeviceInfo);
            if (!ret) {
                retVal = ERR_INVALID_ID;// 无效的设备ID
                pRepErrorInfo = new CMessageErrorInfo();
                pRepErrorInfo->m_qstrErrorInfo = QStringLiteral("根据设备编号[%1]找不到设备详细信息").arg(qstrDeviceID);
                responseMessageOut->SetMessageBody(pRepErrorInfo);
                break;
            } else {
                pResponse->m_lstDeviceItemInfo.push_back(pDeviceInfo);
            }
        }

        if (ret) {
            // 设置回复消息内容
            responseMessageOut->SetMessageBody(pResponse);
        } else {
            delete pResponse;
        }
    }

    if (retVal != ERR_NONE) {
        Q_ASSERT(pRepErrorInfo);

        if (pUser) {
            LOG_WARING() << QStringLiteral("PC客户端[%1]查询设备[%2]信息失败,错误信息：%3")
                .arg(pUser->GetDisplayText()).arg(qstrDeviceID)
                .arg(pRepErrorInfo->m_qstrErrorInfo);
        } else {
            LOG_WARING() << QStringLiteral("PC客户端[%1]查询设备信息失败,错误信息：%3")
                .arg(session->get_display_text()).arg(pRepErrorInfo->m_qstrErrorInfo);
        }
    } else {
        LOG_INFO() << QStringLiteral("PC客户端[%1]查询设备[%2]信息成功 ")
            .arg(pUser->GetDisplayText()).arg(qstrDeviceID);
    }

    return retVal;
}

//******************************************************
//** 函数名:   HandlePCClientExit
//** 功能简述: PC客户端退出
//** 输入参数: 
//   CMessage * sessionMessageIn:
//
//** 返回值: 
//   int:
//
//** 创建日期：2015/07/27
//** 创建人：  唐桥
//** 修改记录：
//   日期        版本      修改人    修改内容
//
//** 其它说明：
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

    LOG_INFO() << QStringLiteral("井队[%1]的用户[%2]退出,关闭连接[%3]")
        .arg(pRig->GetDisplayText()).arg(pUser->GetDisplayText())
        .arg(session->get_display_text());

    // 将当前会话移除出井队的客户端会话组,并关闭连接
    pRig->RemoveClientSession(session);
    session->stop();
    session->set_entity(NULL);
    pUser->SetSession(NULL);

    return 0;
}


//******************************************************
//** 函数名:   HandlePCClientGetDeviceOnlineStatus
//** 功能简述: 
//** 输入参数: 
//   tcp_session_ptr session:
//   message_ptr sessionMessageIn:
//   message_ptr responseMessageOut:
//
//** 返回值: 
//   int:
//
//** 创建日期：2015/08/05
//** 创建人：  唐桥
//** 修改记录：
//   日期        版本      修改人    修改内容
//
//** 其它说明：
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

    if (!pUser) {// 客户端未登录
        retVal = ERR_INVALID_SESSION;
        pRepErrorInfo = new CMessageErrorInfo();
        pRepErrorInfo->m_qstrErrorInfo = QStringLiteral("拒绝未经过登录验证的客户端获取设备在线状态");
        responseMessageOut->SetMessageBody(pRepErrorInfo);
    } else {
        CMessageGetDeviceOnlineStatus * pReq = static_cast<CMessageGetDeviceOnlineStatus *>(sessionMessageIn->m_pBody);
        Q_ASSERT(pReq);

        if (pReq->m_lstDeviceIDs.empty()) {// 请求中的设备ID列表为空
            retVal = ERR_INVALID_ID;
            pRepErrorInfo = new CMessageErrorInfo();
            pRepErrorInfo->m_qstrErrorInfo = QStringLiteral("客户端请求获取在线状态的设备ID列表为空");
            responseMessageOut->SetMessageBody(pRepErrorInfo);
        } else {
            CMessageGetDeviceOnlineStatusRep * pRep = new CMessageGetDeviceOnlineStatusRep();
            for (CMessageDeviceID * pDeviceID : pReq->m_lstDeviceIDs)
            {
                CMessageGetDeviceOnlineStatusRepItem * pStatusItem = new CMessageGetDeviceOnlineStatusRepItem();
                memcpy_s(pStatusItem->m_struct.deviceID, sizeof(pStatusItem->m_struct.deviceID), pDeviceID->m_struct.deviceID, sizeof(pDeviceID->m_struct.deviceID));

                QString qstrDeviceID = QString::fromLocal8Bit((char *)(pDeviceID->m_struct.deviceID), sizeof(pDeviceID->m_struct.deviceID)).trimmed();
                device_ptr pDevice = pRig->FindDeviceByID(qstrDeviceID);
                if (!pDevice) {// 未查找到对应ID的设备
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
            LOG_WARING() << QStringLiteral("拒绝未经过登录验证的客户端[%1]获取设备在线状态")
                .arg(session->get_display_text());
        } else {
            LOG_WARING() << QStringLiteral("井队[%1]的用户[%2]获取设备在线状态失败,错误信息[%3]")
                .arg(pRig->GetDisplayText()).arg(pUser->GetDisplayText())
                .arg(pRepErrorInfo->m_qstrErrorInfo);
        }
    } else {
        LOG_INFO() << QStringLiteral("井队[%1]的用户[%2]获取设备在线状态成功,共返回[%3]条在线状态信息")
            .arg(pRig->GetDisplayText()).arg(pUser->GetDisplayText()).arg(3);// TODO 
    }

    return retVal;
}


//******************************************************
//** 函数名:   HandleDeviceLogin
//** 功能简述: 处理下位机登录消息
//** 输入参数: 
//   CMessage * sessionMessageIn:请求消息
//   CMessage & responseMessageOut:回复消息
//
//** 返回值: 
//   int:成功时返回ERR_NONE，否则返回相应的错误码
//
//** 创建日期：2015/07/26
//** 创建人：  唐桥
//** 修改记录：
//   日期        版本      修改人    修改内容
//
//** 其它说明：
//******************************************************
int CBusinessHandleService::HandleDeviceLogin(tcp_session_ptr session, 
                                              message_ptr sessionMessageIn,
                                              message_ptr responseMessageOut)
{
	//return ERR_NONE;
    // 软件授权验证
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
	LOG_WARING() << QStringLiteral("设备登录_number#") << pMessageLogin->m_qstrAccount;
    bool ret = CDBService::GetInstance()->IsExistedDevice(pMessageLogin->m_qstrAccount, pMessageLogin->m_qstrPassword);
    if (!ret) {
        retVal = ERR_INVALID_ACCOUNT;// 设备未注册
        CMessageErrorInfo * pResponse = new CMessageErrorInfo();
        pResponse->m_qstrErrorInfo = QStringLiteral("未注册的设备ID");
        responseMessageOut->SetMessageBody(pResponse);
    } else {
        // 设备登录成功后，设置tcp连接为已验证状态，设置连接的用户信息
        // 获取设备ID
        QString qstrDeviceID;
        ret = CDBService::GetInstance()->GetDeviceIDByDeviceCode(pMessageLogin->m_qstrAccount, qstrDeviceID);
		LOG_WARING() << "business_handle_service.cpp 664 __id#" << session->get_display_text()  << "#" <<qstrDeviceID;
        if (!ret) {
            pResponse = new CMessageErrorInfo();
            pResponse->m_qstrErrorInfo = QStringLiteral("数据库记录错误");
            responseMessageOut->SetMessageBody(pResponse);
            retVal = ERR_DB_ERROR;
		}
		else {
#ifndef RIGID //井队 油田 没有啊,先注释掉 2017/6/9
			// 构建内存里的实体业务模型，获取用户所属井队、所属油田
			QString qstrRigID;
			ret = CDBService::GetInstance()->GetRigIDByDeviceID(qstrDeviceID, qstrRigID);
			if (ret) {
				QString qstrOilFieldID;
				ret = CDBService::GetInstance()->GetOilFieldIDByRigID(qstrRigID, qstrOilFieldID);
				if (ret) {
					// 获取油田对象
					pOilField = CBusinessModel::GetInstance()->FindOilFieldByID(qstrOilFieldID);
					if (!pOilField) {
						pOilField = CBusinessModel::GetInstance()->GetOrCreateOilField(qstrOilFieldID);
					}

					Q_ASSERT(pOilField);

					// 获取井队对象
					pRig = pOilField->FindRigByID(qstrRigID);
					if (!pRig) {
						pRig = pOilField->GetOrCreateRig(qstrRigID);
					}

					Q_ASSERT(pRig);
#endif
					// 获取设备对象
					pDevice = pRig->FindDeviceByID(qstrDeviceID);
					if (!pDevice) {
						pDevice = pRig->GetOrCreateDevice(qstrDeviceID);
					}

					Q_ASSERT(pDevice);

					if (pDevice->GetSession()
						&& session->get_display_text() != pDevice->GetSession()->get_display_text()) {
						// 顶掉已登录的相同ID的设备 tangqiao 2016/05/17
						//retVal = ERR_INVALID_SESSION;// 相同ID的设备已登录
						//pResponse = new CMessageErrorInfo();
						//pResponse->m_qstrErrorInfo = QStringLiteral("相同编号的设备已登录服务器");
						//responseMessageOut->SetMessageBody(pResponse);
						LOG_WARING() << QStringLiteral("编号[%1]的设备已登录服务器，新连接[%2]顶掉旧连接[%3]")
							.arg(qstrDeviceID).arg(session->get_display_text())
							.arg(pDevice->GetSession()->get_display_text());

						pDevice->SetSession(session);// 设置设备会话，内部自动将其加入井队的在线设备列表

						// 设置回复消息
						CMessageLoginRep * pResponse = new CMessageLoginRep();
						// 获取当前服务器时间
						pResponse->m_struct.serverTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
						responseMessageOut->SetMessageBody(pResponse);

						LOG_WARING() << pRig->GetDevicesOnlineStatusDisplayText();
					}
					else {
						pDevice->SetSession(session);// 设置设备会话，内部自动将其加入井队的在线设备列表

						// 设置回复消息
						CMessageLoginRep * pResponse = new CMessageLoginRep();
						// 获取当前服务器时间
						pResponse->m_struct.serverTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
						responseMessageOut->SetMessageBody(pResponse);

						LOG_WARING() << pRig->GetDevicesOnlineStatusDisplayText();
					}

				}
			}
		}
    }
	

	return retVal;

    QString format = QStringLiteral("设备[%1]登录");
    if (retVal == ERR_NONE) {
        LOG_INFO() << format.arg(pMessageLogin->m_qstrAccount) << QStringLiteral("business_handle_service.cpp_744_成功");

        pDevice->SetLoginTime(std::make_shared<QDateTime>(QDateTime::currentDateTime()));

        // 广播设备上线的通知到井队的所有登录客户端
        CMessageBroadcastDeviceOnlineStatus * pBroadcastMsg = new CMessageBroadcastDeviceOnlineStatus();
        CMessageGetDeviceOnlineStatusRepItem * pStatusItem = new CMessageGetDeviceOnlineStatusRepItem();
        memcpy_s((char *)(pStatusItem->m_struct.deviceID), sizeof(pStatusItem->m_struct.deviceID), pDevice->id.toLocal8Bit(), pDevice->id.toLocal8Bit().length());
        pStatusItem->m_struct.deviceOnlineStatus = DEVICE_ONLINE_STATUS_ONLINE;
        pBroadcastMsg->m_struct.itemCount = 0;
        pBroadcastMsg->m_lstDeviceIDs.push_back(pStatusItem);

        message_ptr broadcast_msg = std::make_shared<CMessage>();
        broadcast_msg->m_header.m_struct.cmd = PC_SERVER_CMD_BROADCAST_DEVICE_STATUS;
        broadcast_msg->SetMessageBody(pBroadcastMsg);
        pDevice->BroadcastToPCClients(broadcast_msg);// 发送广播
		
    } else {
        LOG_WARING() << format.arg(pMessageLogin->m_qstrAccount) << QStringLiteral("失败，错误：")
            << pResponse->m_qstrErrorInfo;
    }

    
}

//******************************************************
//** 函数名:   HandleDeviceRealTimeData
//** 功能简述: 处理设备上传的实时采集数据
//** 输入参数: 
//   CMessage * sessionMessageIn:
//
//** 返回值: 
//   int:
//
//** 创建日期：2015/07/27
//** 创建人：  唐桥
//** 修改记录：
//   日期        版本      修改人    修改内容
//
//** 其它说明：
//******************************************************
int CBusinessHandleService::HandleDeviceRealTimeData(tcp_session_ptr session, 
                                                     message_ptr sessionMessageIn)
{
	
    Q_ASSERT(sessionMessageIn);
	
	LOG_WARING() << QStringLiteral("设备实时上传(单条数据)DEVICE_CMD_REALTIME_DATA_%1 ### cmd_%2##").arg(DEVICE_CMD_REALTIME_DATA)
		.arg(sessionMessageIn->m_header.m_struct.cmd) << GetCurrentThreadId();
    if (sessionMessageIn->m_header.m_struct.cmd != DEVICE_CMD_REALTIME_DATA) {
        return ERR_UNMATCHED_CMD;
    }
    int retVal = ERR_NONE;

    // 设备上传的实时数据
    CMessageDeviceRealtimeData * pMessageData = static_cast<CMessageDeviceRealtimeData *>(sessionMessageIn->m_pBody);

    // 获取对应的设备对象
    device_ptr pDevice = std::static_pointer_cast<CDevice>(session->m_entity);
    if (!pDevice)
    {
        CMessageErrorInfo * pResponse = new CMessageErrorInfo();
        pResponse->m_qstrErrorInfo = QStringLiteral("拒绝未经过登录验证的设备上传实时数据");
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

    // 修正数据上传时间
    pMessageData->m_struct.timestamp = QDateTime::currentMSecsSinceEpoch();

    D64 data = 0;
    {
        U8 buf[8];
        U8 * pData = (U8 *)&(pMessageData->m_struct.data_);
        memcpy(buf, pData, 7);

        LOG_DEBUG() << QStringLiteral("采集数据{工作状态[%1];实时浓度[%2 %3 %4];小数点位[%5];气体名称[%6];测量单位[%7]}")
            .arg(buf[0])
            .arg(buf[1]).arg(buf[2]).arg(buf[3])
            .arg(buf[4])
            .arg(buf[5])
            .arg(buf[6]);
        data = Convert7ByteDataToD64(buf);
    }

    // 获取设备所属井队
    rig_ptr pRig = pDevice->GetRig();
    Q_ASSERT(pRig);

    QString qstrOnlineUsers = pRig->GetOnlineUsersDisplayText();
    if (qstrOnlineUsers.isEmpty()) {
        LOG_INFO() << QStringLiteral("收到井队[%1]的H2S检测设备[%2]的实时采集数据")
            .arg(pRig->GetDisplayText()).arg(pDevice->GetDisplayText());
    } else {
        // 构造广播消息
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
        // 向井队的在线客户端广播消息
        pDevice->BroadcastToPCClients(pMessageBroadcast);

        LOG_INFO() << QStringLiteral("收到井队[%1]的H2S检测设备[%2]的实时采集数据,向井队的在线用户[%3]广播实时采集数据")
            .arg(pRig->GetDisplayText()).arg(pDevice->GetDisplayText())
            .arg(qstrOnlineUsers);
		
    }
	LOG_INFO() << "business_handle_service.cpp 872##" << pDevice->GetDisplayText() << "#" <<  pDevice->id;
    // 实时数据入库
	bool ret;// = CDBService::GetInstance()->DataQueuePushBack(pDevice->id, *pMessageData);
    if (!ret) {
        retVal = ERR_DB_ERROR;// 设备未注册
        // 数据进入处理队列失败
    } else {
        // 数据进入处理队列成功
    }
    return retVal;
}

//******************************************************
//** 函数名:   HandleDeviceExit
//** 功能简述: 处理设备退出
//** 输入参数: 
//   CMessage * sessionMessageIn:
//
//** 返回值: 
//   int:
//
//** 创建日期：2015/07/27
//** 创建人：  唐桥
//** 修改记录：
//   日期        版本      修改人    修改内容
//
//** 其它说明：
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
        LOG_WARING() << QStringLiteral("拒绝未登录的设备[%1]发送退出命令")
            .arg(session->get_display_text());
        return ERR_NONE;
    }

    Q_ASSERT(pDevice);

    rig_ptr pRig =  pDevice->GetRig();
    Q_ASSERT(pRig);

    // 关闭下位机的通信会话
    LOG_INFO() << QStringLiteral("井队[%1]的H2S检测设备[%2]退出,关闭会话[%3]")
        .arg(pRig->GetDisplayText()).arg(pDevice->GetDisplayText())
        .arg(session->get_display_text());
    pRig->RemoveDeviceSession(session);
    session->stop();
    session->set_entity(NULL);
    pDevice->SetSession(NULL);

    // 通知井队的所有客户端，当前下位机的状态
    QString qstrOnlineUsers = pRig->GetOnlineUsersDisplayText();
    if (!qstrOnlineUsers.isEmpty()) {
        LOG_INFO() << QStringLiteral("向井队[%1]的在线用户[%2]发送广播通知,采集设备[%3]已退出")
            .arg(pRig->GetDisplayText()).arg(qstrOnlineUsers).arg(pDevice->GetDisplayText());
        
        CMessageBroadcastDeviceOnlineStatus * pBroadcastMsg = new CMessageBroadcastDeviceOnlineStatus();
        CMessageGetDeviceOnlineStatusRepItem * pStatusItem = new CMessageGetDeviceOnlineStatusRepItem();
        memcpy_s((char *)(pStatusItem->m_struct.deviceID), sizeof(pStatusItem->m_struct.deviceID), pDevice->id.toLocal8Bit(), pDevice->id.toLocal8Bit().length());
        pStatusItem->m_struct.deviceOnlineStatus = DEVICE_ONLINE_STATUS_OFFLINE;// 设备下线
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
//** 函数名:   HandleDeviceSendHeartbeat
//** 功能简述: 处理下位机发送心跳，服务器返回当前时间
//** 输入参数: 
//   tcp_session_ptr session:
//   message_ptr sessionMessageIn:
//   message_ptr responseMessageOut:
//
//** 返回值: 
//   int:
//
//** 创建日期：2015/12/29
//** 创建人：  唐桥
//** 修改记录：
//   日期        版本      修改人    修改内容
//
//** 其它说明：
//******************************************************
int CBusinessHandleService::HandleDeviceSendHeartbeat(tcp_session_ptr session, 
                                                      message_ptr sessionMessageIn, 
                                                      message_ptr responseMessageOut)
{
    Q_ASSERT(session);

    // 收到设备心跳时返回服务器时间的北京时间表示，年月日时分秒
    QDateTime current = QDateTime::currentDateTime();// 取得的时间为当前时区的时间
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
        LOG_INFO() << QStringLiteral("接收到来自井队[%1]设备[%2]的心跳，向其返回服务器当前时间")
            .arg(pRig->GetDisplayText())
            .arg(pDevice->GetDisplayText());

        LOG_WARING() << pRig->GetDevicesOnlineStatusDisplayText();
        LOG_WARING() << QStringLiteral("井队[%1]的在线用户").arg(pRig->GetDisplayText())
            << pRig->GetOnlineUsersDisplayText();
    }
    else
    {
        LOG_WARING() << QStringLiteral("接收到未登录设备[%1]的心跳，向其返回服务器当前时间").arg(session->get_display_text());
    }

    return ERR_NONE;
}


//******************************************************
//** 函数名:   HandleDeviceRealTimeBatchedData
//** 功能简述: 处理设备批量上传采集数据
//** 输入参数: 
//   tcp_session_ptr session:
//   message_ptr sessionMessageIn:
//
//** 返回值: 
//   int:
//
//** 创建日期：2016/1/11
//** 创建人：  唐桥
//** 修改记录：
//   日期        版本      修改人    修改内容
//
//** 其它说明：
//******************************************************
int CBusinessHandleService::HandleDeviceRealTimeBatchedData(tcp_session_ptr session, 
                                                            message_ptr sessionMessageIn, 
                                                            message_ptr responseMessageOut)
{
    Q_ASSERT(sessionMessageIn);
	//LOG_WARING() << QStringLiteral("设备批量上传_DEVICE_CMD_REALTIME_BATCHED_DATA_%1 ### cmd_%2###").arg(DEVICE_CMD_REALTIME_BATCHED_DATA)
	//	.arg(sessionMessageIn->m_header.m_struct.cmd) << GetCurrentThreadId();
    if (sessionMessageIn->m_header.m_struct.cmd != DEVICE_CMD_REALTIME_BATCHED_DATA) {
        return ERR_UNMATCHED_CMD;
    }
    int retVal = ERR_NONE;

    // 设备上传的实时数据
    CMessageDeviceRealtimeBatchedData * pMessageData = static_cast<CMessageDeviceRealtimeBatchedData *>(sessionMessageIn->m_pBody);

    // 获取对应的设备对象
    device_ptr pDevice = std::static_pointer_cast<CDevice>(session->m_entity);
    if (!pDevice)
    {
        CMessageErrorInfo * pResponse = new CMessageErrorInfo();
        pResponse->m_qstrErrorInfo = QStringLiteral("拒绝未经过登录验证的设备批量上传实时数据");
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

    // 修正数据上传时间
    pMessageData->m_struct.timestamp = QDateTime::currentMSecsSinceEpoch();

    QVector<message_device_realtime_data_ptr> vec;
    pMessageData->SplitBatchedMessage(vec);

	LOG_DEBUG() << QStringLiteral("%1设备批量上传数据!").arg(pDevice->id);

	// 实时数据入库
	bool ret = CDBService::GetInstance()->DataQueuePushBack(pDevice->id, vec);


#ifdef RIG_TEXT
   //  获取设备所属井队
    rig_ptr pRig = pDevice->GetRig();
    Q_ASSERT(pRig);

    LOG_DEBUG() << QStringLiteral("收到井队[%1]的H2S检测设备[%2]批量上传的实时采集数据[%3]")
        .arg(pRig->GetDisplayText()).arg(pDevice->GetDisplayText())
        .arg(pMessageData->ToString());

    QString qstrOnlineUsers = pRig->GetOnlineUsersDisplayText();
    if (!qstrOnlineUsers.isEmpty()) {
        // 将批量上传的数据拆分成多个通知
        for (message_device_realtime_data_ptr pRealtimeData : vec)
        {
            D64 data = Convert7ByteDataToD64(pRealtimeData->m_struct.data_);

            // 构造广播消息
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
            // 向井队的在线客户端广播消息
            pDevice->BroadcastToPCClients(pMessageBroadcast);

            LOG_INFO() << QStringLiteral("将井队[%1]的H2S检测设备[%2]实时采集的数据,向井队的在线用户[%3]广播")
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

        LOG_DEBUG() << QStringLiteral("采集数据{工作状态[%1];实时浓度[%2 %3 %4];小数点位[%5];气体名称[%6];测量单位[%7]}")
            .arg(buf[0])
            .arg(buf[1]).arg(buf[2]).arg(buf[3])
            .arg(buf[4])
            .arg(buf[5])
            .arg(buf[6]);
        data = Convert7ByteDataToD64(buf);
    }

        // 实时数据入库
        bool ret = CDBService::GetInstance()->DataQueuePushBack(pDevice->id, *pRealtimeData);
        if (!ret) {
            retVal = ERR_DB_ERROR;// 设备未注册
			//LOG_DEBUG() << QStringLiteral("数据进入处理队列失败");
            // 数据进入处理队列失败
        } else {
            // 数据进入处理队列成功
			//LOG_DEBUG() << QStringLiteral("数据进入处理队列成功");
        }
    }
#endif 
    return retVal;
}

//******************************************************
//** 函数名:   HandlePCClientSetDeviceParam
//** 功能简述: 处理PC客户端设置设备参数 
//** 输入参数: 
//   tcp_session_ptr session:
//   message_ptr sessionMessageIn:
//   message_ptr responseMessageOut:
//
//** 返回值: 
//   int:
//
//** 创建日期：2015/08/04
//** 创建人：  唐桥
//** 修改记录：
//   日期        版本      修改人    修改内容
//
//** 其它说明：
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
        pRepErrorInfo->m_qstrErrorInfo = QStringLiteral("拒绝未经过登录验证的客户端[设置设备参数");
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
            pRepErrorInfo->m_qstrErrorInfo = QStringLiteral("设备ID无效");
            responseMessageOut->SetMessageBody(pRepErrorInfo);
        } else {
            // 更新设备名
            bool updateDeviceNameSucceed = CDBService::GetInstance()->SetDeviceName(qstrDeviceID, pParam->GetDeviceName());
            if (!updateDeviceNameSucceed) {
                retVal = ERR_DB_ERROR;
                pRepErrorInfo = new CMessageErrorInfo();
                pRepErrorInfo->m_qstrErrorInfo = QStringLiteral("数据库更新设备名称出错");
                responseMessageOut->SetMessageBody(pRepErrorInfo);
            } else {
                // 更新数据库中指定设备的设备参数
                bool ret = CDBService::GetInstance()->SetDeviceParam(pDevice->deviceType, qstrDeviceID, *pParam);
                if (!ret) {
                    retVal = ERR_DB_ERROR;
                    pRepErrorInfo = new CMessageErrorInfo();
                    pRepErrorInfo->m_qstrErrorInfo = QStringLiteral("数据库更新设备参数出错");
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
        LOG_WARING() << QStringLiteral("井队[%1]的用户[%2]设置设备参数失败,错误信息[%3]")
            .arg(pRig->GetDisplayText()).arg(pUser->GetDisplayText())
            .arg(pRepErrorInfo->m_qstrErrorInfo);
    } else {
        LOG_INFO() << QStringLiteral("井队[%1]的用户[%2]设置H2S检测设备[%3]的参数成功，参数内容[%4]")
            .arg(pRig->GetDisplayText()).arg(pUser->GetDisplayText())
            .arg(pDevice->GetDisplayText()).arg(pParam->ToString());
    }

    return retVal;
}


//******************************************************
//** 函数名:   HandlePCClientSendHeartbeat
//** 功能简述: 处理PC客户端发送心跳
//** 输入参数: 
//   tcp_session_ptr session:
//   message_ptr sessionMessageIn:
//   message_ptr responseMessageOut:
//
//** 返回值: 
//   int:
//
//** 创建日期：2015/12/28
//** 创建人：  唐桥
//** 修改记录：
//   日期        版本      修改人    修改内容
//
//** 其它说明：
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
        pRepErrorInfo->m_qstrErrorInfo = QStringLiteral("拒绝未经过登录验证的客户端[发送心跳]");
        responseMessageOut->SetMessageBody(pRepErrorInfo);
        LOG_WARING() << QStringLiteral("拒绝未经过登录验证的客户端[%1][发送心跳]").arg(session->get_display_text());
    } else {
        CMessageHeartbeat * heartbeat = new CMessageHeartbeat();
        heartbeat->m_struct.heartbeatTime = QDateTime::currentMSecsSinceEpoch();
        responseMessageOut->SetMessageBody(heartbeat);
        LOG_INFO() << QStringLiteral("用户[%2]发送心跳").arg(pUser->GetDisplayText());
    }

    return retVal;
}


//******************************************************
//** 函数名:   HandlePCClientGetHistoryDeviceDataCount
//** 功能简述: 处理PC向客户端获取历史设备数据条数
//** 输入参数: 
//   tcp_session_ptr session:
//   message_ptr sessionMessageIn:
//   message_ptr responseMessageOut:
//
//** 返回值: 
//   int:
//
//** 创建日期：2016/1/4
//** 创建人：  唐桥
//** 修改记录：
//   日期        版本      修改人    修改内容
//
//** 其它说明：
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
        pRepErrorInfo->m_qstrErrorInfo = QStringLiteral("拒绝未经过登录验证的客户端获取历史数据条数");
        responseMessageOut->SetMessageBody(pRepErrorInfo);
        LOG_WARING() << QStringLiteral("拒绝未经过登录验证的客户端[%1]获取历史数据条数").arg(session->get_display_text());
        return retVal;
    } else {
        pRig = static_pointer_cast<CRig>(pUser->GetRig());
        Q_ASSERT(pRig);

        pReq = static_cast<CMessageGetDeviceDataCountReq *>(sessionMessageIn->m_pBody);
        Q_ASSERT(pReq);

        QDateTime begin = QDateTime::fromMSecsSinceEpoch(pReq->m_struct.begin);
        QDateTime end = QDateTime::fromMSecsSinceEpoch(pReq->m_struct.end);
        if (begin.date() != end.date()) {// 检查起止时间是否为同一天
            retVal = ERR_INVALID_PERIOD;
            pRepErrorInfo = new CMessageErrorInfo();
            pRepErrorInfo->m_qstrErrorInfo = QStringLiteral("开始时间与结束时间不在同一天");
            responseMessageOut->SetMessageBody(pRepErrorInfo);
            LOG_WARING() << QStringLiteral("井队[%1]的用户[%2]请求获取历史数据条数，开始时间[%2]与结束时间[%2]不在同一天")
                .arg(pRig->GetDisplayText()).arg(pUser->GetDisplayText())
                .arg(begin.toString("yyyy-MM-dd HH:mm:ss.zzz"))
                .arg(end.toString("yyyy-MM-dd HH:mm:ss.zzz"));
            return retVal;
        }

        // 检查是否为查询所有设备的数据
        QString qstrDeviceID = pReq->GetDeviceID();
        QVector<QString> vecDeviceIDs;
        if (!qstrDeviceID.isEmpty()) {
            vecDeviceIDs.push_back(qstrDeviceID);
        } else {
            // 未指定设备ID，则查询井队里所有设备的历史数据
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
            pRepErrorInfo->m_qstrErrorInfo = QStringLiteral("服务器数据库操作错误");
            responseMessageOut->SetMessageBody(pRepErrorInfo);
            LOG_WARING() << QStringLiteral("井队[%1]的用户[%2]请求获取历史数据条数，从数据库获取数据时出错")
                .arg(pRig->GetDisplayText()).arg(pUser->GetDisplayText());
            return retVal;
        } else {
            CMessageGetDeviceDataCountRep * pRep = new CMessageGetDeviceDataCountRep();
            pRep->m_struct.deivceDataCount = count;
            responseMessageOut->SetMessageBody(pRep);
            LOG_INFO() << QStringLiteral("井队[%1]的用户[%2]请求获取[%3 至 %4]之间的历史数据条数，从数据库查询到[%5]条")
                .arg(pRig->GetDisplayText()).arg(pUser->GetDisplayText())
                .arg(begin.toString("yyyy-MM-dd HH:mm:ss.zzz")).arg(end.toString("yyyy-MM-dd HH:mm:ss.zzz"))
                .arg(count);
        }
    }

    return retVal;
}


//******************************************************
//** 函数名:   HandlePCClientGetHistoryDeviceData
//** 功能简述: 处理PC客户端获取历史设备数据
//** 输入参数: 
//   tcp_session_ptr session:
//   message_ptr sessionMessageIn:
//   message_ptr responseMessageOut:
//
//** 返回值: 
//   int:
//
//** 创建日期：2016/1/4
//** 创建人：  唐桥
//** 修改记录：
//   日期        版本      修改人    修改内容
//
//** 其它说明：
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
        pRepErrorInfo->m_qstrErrorInfo = QStringLiteral("拒绝未经过登录验证的客户端获取历史数据");
        responseMessageOut->SetMessageBody(pRepErrorInfo);
        LOG_WARING() << QStringLiteral("拒绝未经过登录验证的客户端[%1]获取历史数据").arg(session->get_display_text());
        return retVal;
    }

    pRig = static_pointer_cast<CRig>(pUser->GetRig());
    Q_ASSERT(pRig);

    pReq = static_cast<CMessageGetDeviceDataReq *>(sessionMessageIn->m_pBody);
    Q_ASSERT(pReq);

    // 获取数据的时间段必须在同一天内
    QDateTime begin = QDateTime::fromMSecsSinceEpoch(pReq->m_struct.begin);
    QDateTime end = QDateTime::fromMSecsSinceEpoch(pReq->m_struct.end);
    if (begin.date() != end.date()) {
        retVal = ERR_INVALID_PERIOD;
        pRepErrorInfo = new CMessageErrorInfo();
        pRepErrorInfo->m_qstrErrorInfo = QStringLiteral("开始时间与结束时间不在同一天");
        responseMessageOut->SetMessageBody(pRepErrorInfo);
        LOG_WARING() << QStringLiteral("井队[%1]的用户[%2]请求获取历史数据，开始时间[%2]与结束时间[%2]不在同一天")
            .arg(pRig->GetDisplayText()).arg(pUser->GetDisplayText())
            .arg(begin.toString("yyyy-MM-dd HH:mm:ss.zzz"))
            .arg(end.toString("yyyy-MM-dd HH:mm:ss.zzz"));
        return retVal;
    }

    // 检查分页号是否为0
    if (pReq->m_struct.pageIndex == 0) {
        retVal = ERR_INVALID_PARAM;
        pRepErrorInfo = new CMessageErrorInfo();
        pRepErrorInfo->m_qstrErrorInfo = QStringLiteral("请求分页号不能为0");
        responseMessageOut->SetMessageBody(pRepErrorInfo);
        LOG_WARING() << QStringLiteral("井队[%1]的用户[%2]请求获取历史数据，分页号为0")
            .arg(pRig->GetDisplayText()).arg(pUser->GetDisplayText());
        return retVal;
    }

    // 检查每页的记录数是否为0
    if (pReq->m_struct.rowsOfPerPage == 0) {
        retVal = ERR_INVALID_PARAM;
        pRepErrorInfo = new CMessageErrorInfo();
        pRepErrorInfo->m_qstrErrorInfo = QStringLiteral("每页的记录数不能指定为0");
        responseMessageOut->SetMessageBody(pRepErrorInfo);
        LOG_WARING() << QStringLiteral("井队[%1]的用户[%2]请求获取历史数据，指定每页返回记录数为0")
            .arg(pRig->GetDisplayText()).arg(pUser->GetDisplayText());
        return retVal;
    }

    // 检查是否为查询所有设备的数据
    QString qstrDeviceID = pReq->GetDeviceID();
    QVector<QString> vecDeviceIDs;
    if (!qstrDeviceID.isEmpty()) {
        vecDeviceIDs.push_back(qstrDeviceID);
    } else {
        // 未指定设备ID，则查询井队里所有设备的历史数据
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
        pRepErrorInfo->m_qstrErrorInfo = QStringLiteral("服务器数据库操作错误");
        responseMessageOut->SetMessageBody(pRepErrorInfo);
        LOG_WARING() << QStringLiteral("井队[%1]的用户[%2]请求获取历史数据，从数据库获取数据时出错")
            .arg(pRig->GetDisplayText()).arg(pUser->GetDisplayText());
        return retVal;
    } else {
        CMessageGetDeviceDataRep * pRep = new CMessageGetDeviceDataRep();
        pRep->m_lstDeviceData = lst;
        pRep->m_struct.pageIndex = pReq->m_struct.pageIndex;
        pRep->m_struct.rowsOfCurrentPage = lst.count();
        responseMessageOut->SetMessageBody(pRep);
        LOG_INFO() << QStringLiteral("井队[%1]的用户[%2]请求获取[%3至%4之间的]历史数据，每页[%5]条，获取第[%6]页的数据，共返回[%7]条数据")
            .arg(pRig->GetDisplayText()).arg(pUser->GetDisplayText()).arg(begin.toString("yyyy-MM-dd HH:mm:ss.zzz"))
            .arg(end.toString("yyyy-MM-dd HH:mm:ss.zzz")).arg(pReq->m_struct.rowsOfPerPage).arg(pReq->m_struct.pageIndex)
            .arg(lst.count());
    }

    return retVal;
}

//******************************************************
//** 函数名:   HandleTcpSessionClosed
//** 功能简述: 处理socket连接断开
//** 输入参数: 
//   tcp_session_ptr session:
//
//** 返回值: 
//   void:
//
//** 创建日期：2015/08/05
//** 创建人：  唐桥
//** 修改记录：
//   日期        版本      修改人    修改内容
//
//** 其它说明：
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
//** 函数名:   HandlePCClientSessionClosed
//** 功能简述: 
//** 输入参数: 
//   tcp_session_ptr session:
//
//** 返回值: 
//   void:
//
//** 创建日期：2015/08/05
//** 创建人：  唐桥
//** 修改记录：
//   日期        版本      修改人    修改内容
//
//** 其它说明：
//******************************************************
void CBusinessHandleService::HandlePCClientSessionClosed(tcp_session_ptr session)
{
    Q_ASSERT(session);
    Q_ASSERT(session->m_entity);

    user_ptr pUser = std::static_pointer_cast<CUser>(session->m_entity);
    Q_ASSERT(pUser);

    rig_ptr pRig = std::static_pointer_cast<CRig>(pUser->GetRig());
    Q_ASSERT(pRig);

    LOG_WARING() << QStringLiteral("井队[%1]的用户[%2]异常下线，自动释放连接[%3]")
        .arg(pRig->GetDisplayText()).arg(pUser->GetDisplayText())
        .arg(session->get_display_text());

    // 关闭socket，将socket从井队的客户端会话组里移除
    pRig->RemoveClientSession(session);
    pUser->SetSession(NULL);

    LOG_WARING() << pRig->GetDevicesOnlineStatusDisplayText();
    LOG_WARING() << QStringLiteral("井队[%1]的在线用户").arg(pRig->GetDisplayText())
        << pRig->GetOnlineUsersDisplayText();
}

//******************************************************
//** 函数名:   HandleDeviceSessionClosed
//** 功能简述: 
//** 输入参数: 
//   tcp_session_ptr session:
//
//** 返回值: 
//   void:
//
//** 创建日期：2015/08/05
//** 创建人：  唐桥
//** 修改记录：
//   日期        版本      修改人    修改内容
//
//** 其它说明：
//******************************************************
void CBusinessHandleService::HandleDeviceSessionClosed(tcp_session_ptr session)
{
    Q_ASSERT(session);
    Q_ASSERT(session->m_entity);

    device_ptr pDevice = std::static_pointer_cast<CDevice>(session->m_entity);
    Q_ASSERT(pDevice);

    rig_ptr pRig = std::static_pointer_cast<CRig>(pDevice->GetRig());
    Q_ASSERT(pRig);

    LOG_WARING() << QStringLiteral("井队[%1]的采集设备[%2]异常下线，自动释放连接[%3]")
        .arg(pRig->GetDisplayText()).arg(pDevice->GetDisplayText())
        .arg(session->get_display_text());

    // 关闭socket连接 将设备会话从井队的设备会话组里移除
    pRig->RemoveDeviceSession(session);
    session->set_entity(NULL);
    pDevice->SetSession(NULL);

    QString qstrOnlineUsers = pRig->GetOnlineUsersDisplayText();
    if (!qstrOnlineUsers.isEmpty()) {
        LOG_INFO() << QStringLiteral("向井队[%1]的在线用户[%2]发送设备[%3]异常下线通知")
            .arg(pRig->GetDisplayText()).arg(qstrOnlineUsers).arg(pDevice->GetDisplayText());
        CMessageBroadcastDeviceOnlineStatus * pBroadcastMsg = new CMessageBroadcastDeviceOnlineStatus();
        CMessageGetDeviceOnlineStatusRepItem * pStatusItem = new CMessageGetDeviceOnlineStatusRepItem();
        memcpy_s((char *)(pStatusItem->m_struct.deviceID), sizeof(pStatusItem->m_struct.deviceID), pDevice->id.toLocal8Bit(), pDevice->id.toLocal8Bit().length());
        pStatusItem->m_struct.deviceOnlineStatus = DEVICE_ONLINE_STATUS_OFFLINE;// 设备下线
        pBroadcastMsg->m_struct.itemCount = 0;
        pBroadcastMsg->m_lstDeviceIDs.push_back(pStatusItem);

        message_ptr broadcast_msg = std::make_shared<CMessage>();
        broadcast_msg->m_header.m_struct.cmd = PC_SERVER_CMD_BROADCAST_DEVICE_STATUS;
        broadcast_msg->SetMessageBody(pBroadcastMsg);
        pDevice->BroadcastToPCClients(broadcast_msg);
    }
}











