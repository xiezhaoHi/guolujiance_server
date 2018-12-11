#ifndef MYDATABASE_H
#define MYDATABASE_H

#include"connectionpool.h"
#include<QRunnable>
#include<QThread>
#include<QTimer>
#include<QObject>
#include "../db_service/db_service.h"
/*
 * 数据库操作库
 * 实现数据库基本操作
 * 增:addData(QString const& strSql)
 * 删:deleteData(QString const& strSql)
 * 改:changeData(QString const& strSql)
 * 查:selectData(QString const& strSql)
 *
 */

#define MAX_THREAD 5
class MyDatabase:public QObject
{
    Q_OBJECT
private:
    //QString m_strPath;  //配置文件的路径
public:
    static MyDatabase*  m_singleton;
    static MyDatabase* GetInstance(void);
    ~MyDatabase();
    /*开启服务线程*/
    void Start();

    /*destroy服务*/
    void Destroy();

    /*是否停止*/
    bool IsStop();

    /*通知停止*/
    void NotifyStop();

     bool AddData(const QString &strSql);
    bool DeleteData(QString const& strSql);
    bool ChangeData(QString const& strSql);
    bool SelectData(QString const& strSql, QStringList **&pList, int &count,int flag = 1);
    bool TableExists(const QString &strDBName, QString const& strTableName);

    /*
     * 判断数据库 是否有 指定数据库 和 指定数据表
     */
    bool    FindDBTableAndCreate(int flag, QString const& strDBName, QString const& strTableName);
    /*
     * 创建指定的表
     */
    bool    CreateFlagTable(int flag,QString const& strDBName, QString const& strTableName);

	/*
	*tableName:暂时不用.
	*lst:气体字符串
	*/
	bool InsertDataTable(const QString & tableName, T_DEVICE_DATA&  lst);

	/*
	获取设备信息
	*/
	bool GetDeviceDataByDeviceCode(const QString & deviceCode
		, DeviceInfo & deviceInfo);

	/*
	1.初始化设备关联的锅炉信息
	2.初始化字段参数表
	*/
	bool InitDevConfigMap(QString const&);

	bool InitParamConfigMap();
private:
    
    MyDatabase();
    QString m_strDBName;
    QString m_strTableName[5];//暂时没有用
   //  friend class Database_task;
     bool   m_stop;
     QTimer  m_examine_timer;
	 QMap<QString, DEVICE_MSG>      m_deviceMsgMap; //设备关联的设备信息表
	 QStringList					m_paramsList; //字段参数关联字段
public slots:
     /*
      * 对外 提供 借口 检查 数据库 和 表
      */
     void  Examine_DB_Table();
};

#endif // MYDATABASE_H
