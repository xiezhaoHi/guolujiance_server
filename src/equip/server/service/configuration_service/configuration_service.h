#ifndef _CONFIGURATION_SERVICE_H_
#define _CONFIGURATION_SERVICE_H_

#include "service/i_service.h"
#include "service/log_service/log_service.h"

class CConfigrationService : public IInternalService
{
public:

    static CConfigrationService * GetInstance();

    static void DestroyInstance();

    virtual int StartInternalService();

    virtual int StopInternalService();

    virtual int NotifyServiceToStop();

    virtual QString GetServiceName();

    //////////////////////////////////////////////////////////////////////////
    // �����ṩ�Ĳ����ӿ�

    // ��ȡ��־����
    E_LOG_LEVEL GetLogLevel();

    // ������־����
    void SetLogLevel(E_LOG_LEVEL level);

    // ��ȡTCP�����˿�
    unsigned short GetPort();

    // ��ȡ�豸���ӵ�������ʱ��
    int GetDevConnMaxIdleTime();

    // ��ȡ���ݿ������ַ���
    QString GetDBConnStr();

    // ��ȡ���ݿ��¼�˺�
    QString GetDBUser();

    // ��ȡ���ݿ��¼����
    QString GetDBPassword();

    // ��ȡ���ݿ�������
    QString GetDBHostName();

    // ��ȡ���ݿ��������˿�
    int GetDBPort();

    // ��ȡ��ЯʽH2S����ǵ����ͱ���
    QString GetTypeCodeOfPortableH2SDetector();

    // ��ȡ�̶�ʽH2S����ǵ����ͱ���
    QString GetTypeCodeOfStaticH2SDetector();

private:

    CConfigrationService();

    ~CConfigrationService();

    // �������ļ��м�������
    void LoadConfiguration();

private:
    static CConfigrationService * m_pInstance;
    E_LOG_LEVEL         m_eLogLevel;    // ��־�������
    unsigned short      m_unPort;       // �������󶨵Ķ˿�
    unsigned int        m_unDevConnMaxIdleTime;// �豸����������ʱ��
    QString             m_qstrDBConnStr;// ���ݿ������ַ���
    QString             m_qstrDBUser;   // ���ݿ��¼�û���
    QString             m_qstrDBPassword;//���ݿ��¼����
    QString             m_qstrDBHostName;// ���ݿ��������������IP��
    int                 m_nDBPort;      // ���ݿ����Ӷ˿�
    QString             m_qstrTypeCodeOfPortableH2SDetector;// ��ЯʽH2S������豸������
    QString             m_qstrTypeCodeOfStaticH2SDetector;// �̶�ʽH2S�����
};

#endif//!_CONFIGURATION_SERVICE_H_