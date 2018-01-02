/*****************************************************
** �ļ�����qinit_configuration.cpp
** �� Ȩ��XXX ��Ȩ����
** �� ����
** ���ݼ�������ʼ��������Ϣʵ���ļ� 
** �������ڣ�2015��7��
** �����ˣ�����
** �޸ļ�¼��
����        �汾      �޸���    �޸�����   
*****************************************************/ 
#include "qinit_configuration.h"
#include "QSettings"
#include <QFile>
#include <QTextStream>
#include <QCryptographicHash>
#include <windows.h>

QInitConfiguration* QInitConfiguration::m_pInstance = NULL;

QInitConfiguration::QInitConfiguration(void)
{
    WCHAR path[MAX_PATH];
    DWORD dw = GetModuleFileName(NULL, path, MAX_PATH);
    QString qstrInstDir = QString::fromWCharArray(path);
    int index = qstrInstDir.lastIndexOf("\\");
    qstrInstDir.resize(index);

    m_qstrInstPath = qstrInstDir;

    //��ȡ������Ϣ
    QSettings *configIni = new QSettings(QString("%1\\app.ini").arg(qstrInstDir), 
        QSettings::IniFormat);
    m_strIp = (configIni->value("server/ip").toString());
    m_strPort = (configIni->value("server/port").toString());
    m_registCode = (configIni->value("user/registcode").toString());	
    m_aveTime = (configIni->value("user/avetime").toString());
    m_warnType = (configIni->value("user/warntype").toString());
    m_qstrLastExportPath = (configIni->value("path/lastExportPath", QString("%1\\export").arg(qstrInstDir)).toString());
    delete configIni;
}


QInitConfiguration::~QInitConfiguration(void)
{

}

QInitConfiguration* QInitConfiguration::GetInitInstance()
{
    if(m_pInstance == NULL)
    {
        m_pInstance = new QInitConfiguration();
    }
    return m_pInstance;
}

void QInitConfiguration::DestroyInstance()
{
    delete m_pInstance;
    m_pInstance = NULL;
}
void QInitConfiguration::setWarnInfo(QString avetime, QString warnType )
{
    QSettings *configIni = new QSettings("./app.ini", QSettings::IniFormat);
    configIni->beginGroup("user");

    configIni->setValue("avetime", avetime );
    configIni->setValue("warntype", warnType );
    configIni->endGroup();
    delete configIni;
}
void QInitConfiguration::setRegistCode(QString code)
{
    QSettings *configIni = new QSettings("./app.ini", QSettings::IniFormat);
    configIni->beginGroup("user");
    configIni->setValue("registcode", code );
    configIni->endGroup();
    delete configIni;
}

void QInitConfiguration::SetLastExportPath(const QString & exportPath)
{
    if (exportPath.trimmed().isEmpty()) {
        return;
    }
    QSettings configIni(QString("%1\\app.ini").arg(m_qstrInstPath), QSettings::IniFormat);
    configIni.beginGroup("path");
    configIni.setValue("lastExportPathf", exportPath);
    configIni.endGroup();

    m_qstrLastExportPath = exportPath;
}

QString QInitConfiguration::GetLastExportPath()
{
    return m_qstrLastExportPath;
}
