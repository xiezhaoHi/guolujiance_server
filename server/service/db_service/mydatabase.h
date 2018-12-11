#ifndef MYDATABASE_H
#define MYDATABASE_H

#include"connectionpool.h"
#include<QRunnable>
#include<QThread>
#include<QTimer>
#include<QObject>
#include "../db_service/db_service.h"
/*
 * ���ݿ������
 * ʵ�����ݿ��������
 * ��:addData(QString const& strSql)
 * ɾ:deleteData(QString const& strSql)
 * ��:changeData(QString const& strSql)
 * ��:selectData(QString const& strSql)
 *
 */

#define MAX_THREAD 5
class MyDatabase:public QObject
{
    Q_OBJECT
private:
    //QString m_strPath;  //�����ļ���·��
public:
    static MyDatabase*  m_singleton;
    static MyDatabase* GetInstance(void);
    ~MyDatabase();
    /*���������߳�*/
    void Start();

    /*destroy����*/
    void Destroy();

    /*�Ƿ�ֹͣ*/
    bool IsStop();

    /*ֹ֪ͨͣ*/
    void NotifyStop();

     bool AddData(const QString &strSql);
    bool DeleteData(QString const& strSql);
    bool ChangeData(QString const& strSql);
    bool SelectData(QString const& strSql, QStringList **&pList, int &count,int flag = 1);
    bool TableExists(const QString &strDBName, QString const& strTableName);

    /*
     * �ж����ݿ� �Ƿ��� ָ�����ݿ� �� ָ�����ݱ�
     */
    bool    FindDBTableAndCreate(int flag, QString const& strDBName, QString const& strTableName);
    /*
     * ����ָ���ı�
     */
    bool    CreateFlagTable(int flag,QString const& strDBName, QString const& strTableName);

	/*
	*tableName:��ʱ����.
	*lst:�����ַ���
	*/
	bool InsertDataTable(const QString & tableName, T_DEVICE_DATA&  lst);

	/*
	��ȡ�豸��Ϣ
	*/
	bool GetDeviceDataByDeviceCode(const QString & deviceCode
		, DeviceInfo & deviceInfo);

	/*
	1.��ʼ���豸�����Ĺ�¯��Ϣ
	2.��ʼ���ֶβ�����
	*/
	bool InitDevConfigMap(QString const&);

	bool InitParamConfigMap();
private:
    
    MyDatabase();
    QString m_strDBName;
    QString m_strTableName[5];//��ʱû����
   //  friend class Database_task;
     bool   m_stop;
     QTimer  m_examine_timer;
	 QMap<QString, DEVICE_MSG>      m_deviceMsgMap; //�豸�������豸��Ϣ��
	 QStringList					m_paramsList; //�ֶβ��������ֶ�
public slots:
     /*
      * ���� �ṩ ��� ��� ���ݿ� �� ��
      */
     void  Examine_DB_Table();
};

#endif // MYDATABASE_H
