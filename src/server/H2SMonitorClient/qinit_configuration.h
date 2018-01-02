/*****************************************************
** 文件名：qinit_configuration.h
** 版 权：XXX 版权所有
** 版 本：
** 内容简述：初始化配置信息头文件 
** 创建日期：2015年7月
** 创建人：荣永
** 修改记录：
日期        版本      修改人    修改内容   
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

    // 设置最近一次导出文件存放目录
    void SetLastExportPath(const QString & exportPath);

    // 获取最近一次导出文件存放目录
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


