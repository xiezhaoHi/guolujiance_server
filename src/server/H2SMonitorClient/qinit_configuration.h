/*****************************************************
** �ļ�����qinit_configuration.h
** �� Ȩ��XXX ��Ȩ����
** �� ����
** ���ݼ�������ʼ��������Ϣͷ�ļ� 
** �������ڣ�2015��7��
** �����ˣ�����
** �޸ļ�¼��
����        �汾      �޸���    �޸�����   
*****************************************************/ 
#ifndef INIT_CONFIGURATION_H
#define INIT_CONFIGURATION_H

#include <string>
using namespace std;
#include <QString>


class QInitConfiguration
{
private:
	QInitConfiguration(void);
	~QInitConfiguration(void);
	static QInitConfiguration* m_pInstance;

public:

    // �������һ�ε����ļ����Ŀ¼
    void SetLastExportPath(const QString & exportPath);

    // ��ȡ���һ�ε����ļ����Ŀ¼
    QString GetLastExportPath();

	QString m_registCode; 
	QString m_aveTime;
	QString m_warnType;
	QString m_strIp;
	QString m_strPort;
    QString m_qstrLastExportPath;
    QString m_qstrInstPath;
	static QInitConfiguration* GetInitInstance();
	static void DestroyInstance();
	void setWarnInfo(QString , QString  );
	void setRegistCode(QString);
};

#endif //INIT_CONFIGURATION_H


