#include "service_hub.h"
#include "service/i_service.h"
#include "service/socket_service/socket_service.h"
#include "service/log_service/log_service.h"
#include "service/configuration_service/configuration_service.h"
#include "service/db_service/db_service.h"
#include "service/task_service/task_service.h"
#include "service/business_handle_service/business_handle_service.h"
#include "registration/registration.h"
#include <QString>
#include <QDebug>

CServiceHub * CServiceHub::m_pInstance = NULL;


CServiceHub::CServiceHub()
{
    m_bIsServicesRunning = false;
}

CServiceHub::~CServiceHub()
{

}

CServiceHub * CServiceHub::GetInstance()
{
    if (!m_pInstance) {
        m_pInstance = new CServiceHub();
    }

    return m_pInstance;
}

void CServiceHub::DestroyInstance()
{
    if (m_pInstance) {
        delete m_pInstance;
        m_pInstance = NULL;
    }
}

bool CServiceHub::StartServices()
{
    //IS_REGISTED(false);

    E_SERVICE_START_ORDER process = NO_SERVICE;

    int ret = 0;

    // ��־�������
    ret = CLogService::GetInstance()->StartInternalService();
    if (0 != ret) {
        goto DESTROY_SERVICES;
    } else {
        process = LOG_SERVICE;
        LOG_INFO() << QStringLiteral("H2S����������ʼ����");
        LOG_INFO() << CLogService::GetInstance()->GetServiceName() 
            << QStringLiteral("�����ɹ�");
    }

    // �����������÷���
    ret = CConfigrationService::GetInstance()->StartInternalService();
    if (0 != ret) {
        goto DESTROY_SERVICES;
    } else {
        process = CONFIGURATION_SERVICE;
        LOG_INFO() << CConfigrationService::GetInstance()->GetServiceName() 
            << QStringLiteral("�����ɹ�");
    }

    // ���̷߳���
    ret = CTaskService::GetInstance()->StartInternalService();
    if (0 != ret) {
        goto DESTROY_SERVICES;
    } else {
        process = TASK_SERVICE;
        LOG_INFO() << CTaskService::GetInstance()->GetServiceName() 
            << QStringLiteral("�����ɹ�");
    }

    // ���ݿ����
	//20180129 ���� ���ݿ����� ʧ�� �ȴ���Сʱ ��������
	while (1)
	{
		ret = CDBService::GetInstance()->StartInternalService();
		if (0 != ret) {
			//goto DESTROY_SERVICES;
			Sleep(30 * 60 * 1000);
			continue;
		}
		else {
			process = DB_SERVICE;
			LOG_INFO() << CDBService::GetInstance()->GetServiceName()
				<< QStringLiteral("�����ɹ�");
			break;
		}
	}
    // ҵ�������
    ret = CBusinessHandleService::GetInstance()->StartInternalService();
    if (0 != ret) {
        goto DESTROY_SERVICES;
    } else {
        process = BUSINESS_HANDLE_SERVICE;
        LOG_INFO() << CBusinessHandleService::GetInstance()->GetServiceName() 
            << QStringLiteral("�����ɹ�");
    }

    // socket���ӷ���

		ret = CSocketService::GetInstance()->StartInternalService();
		if (0 != ret) {
			goto DESTROY_SERVICES;
		}
		else {
			process = SOCKET_SERVICE;
			LOG_INFO() << CSocketService::GetInstance()->GetServiceName()
				<< QStringLiteral("�����ɹ�");
		}
	
    

    LOG_INFO() << QStringLiteral("H2S���������������");
    m_bIsServicesRunning = true;
    return true;


    // �����ڲ�����Ĺ����г����˴��������Ѿ���õ���Դ
DESTROY_SERVICES:
    LOG_WARING() << QStringLiteral("��������ʧ��,����ֵΪ") << ret;

    switch(process)
    {
    case SOCKET_SERVICE:
        // TODO
    case LOG_SERVICE:
        // TODO

    default:
        break;
    }
    return false;
}

void CServiceHub::StopServices()
{
    m_bIsServicesRunning = false;
    CSocketService::GetInstance()->StopInternalService();
    CBusinessHandleService::GetInstance()->StopInternalService();
    CDBService::GetInstance()->StopInternalService();
    CTaskService::GetInstance()->StopInternalService();
    CConfigrationService::GetInstance()->StopInternalService();
    CLogService::GetInstance()->StopInternalService();
}

void CServiceHub::CreateServicesOnlyOnce()
{
    CLogService::GetInstance();
    CConfigrationService::GetInstance();
    CDBService::GetInstance();
    CTaskService::GetInstance();
    CBusinessHandleService::GetInstance();
    CSocketService::GetInstance();
}

void CServiceHub::DestroyServicesForEver()
{
    CSocketService::DestroyInstance();
    CBusinessHandleService::DestroyInstance();
    CTaskService::DestroyInstance();
    CDBService::DestroyInstance();
    CConfigrationService::DestroyInstance();
    CLogService::DestroyInstance();
}

bool CServiceHub::IsServicesRunning()
{
    return m_bIsServicesRunning;
}
