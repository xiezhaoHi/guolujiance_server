#ifndef _DB_SERVICE_H_
#define _DB_SERVICE_H_

#include "service/i_service.h"
#include <QSqlDatabase>
#include <QReadWriteLock>
#include <QMutex>
#include <QMutexLocker>
#include <QRunnable>
#include <QQueue>
#include <memory>
#include <QSet>
#include "message/message_devices_list_item.h"
#include "message/message_device_item_info.h"
#include "message/message_device_realtime_data.h"
#include "message/message_set_device_param.h"
#include "message/message_device_data_item.h"
#include "device_tree_node.h"
#include "structs/structs.h"
#include "util/sync_cond_queue.h"
#include<QVector>

// 数据库连接读写标识
typedef enum _E_DB_OPEN_FLAG
{
    DB_CON_READONLY = 0,
    DB_CON_READWRITE,
}E_DB_OPEN_FLAG;

typedef std::shared_ptr<QSqlDatabase> sqldb_ptr;

// 数据库连接
typedef struct _T_DB_CON
{
    sqldb_ptr           pDB;
    QString             connectionName;
    E_DB_OPEN_FLAG      openFlag;
    QReadWriteLock      dbLock;
    QMutex              refLock;
    unsigned int        refCount;

    _T_DB_CON(sqldb_ptr db, QString name, E_DB_OPEN_FLAG flag = DB_CON_READONLY)
    {
        pDB = db;
        connectionName = name;
        openFlag = flag;
        refCount = 0;
    }

    void AcquireRead() 
    {
        Q_ASSERT(pDB);

        dbLock.lockForRead();
        QMutexLocker lock(&refLock);
        refCount++;
    }

    void AcquireWrite()
    {
        Q_ASSERT(pDB);
        //Q_ASSERT(openFlag == DB_CON_READWRITE);

        dbLock.lockForWrite();
        QMutexLocker lock(&refLock);
        refCount++;
    }

    void Release()
    {
        Q_ASSERT(pDB);

        dbLock.unlock();
        QMutexLocker lock(&refLock);
        refCount--;
    }
}T_DB_CON;

typedef std::shared_ptr<T_DB_CON> db_con_ptr;

typedef QList<std::shared_ptr<CMessageDeviceRealtimeData>>  REALTIME_DATA;


//20180123 新增的数据结构
//旧版数据和旧 处理方式   0
//新版数据和新协议 处理方式 1
#define OLD_TYPE 0
#define NEW_TYPE 1

typedef struct  _NEW_DEVICE_DATA
{
	U16 m_flag; //气体标识
	QString m_strFlag;//字符串表示
	U32 m_data; //气体值
	_NEW_DEVICE_DATA(short flag, int data,QString strFlag) 
		:m_flag(flag), m_data(data),m_strFlag(strFlag)
	{}
}NEW_DEVICE_DATA;
typedef QList<NEW_DEVICE_DATA> REALTIME_DATA_NEW;
// 设备采集数据
typedef struct _T_DEVICE_DATA
{
    QString                      qstrDeviceID; //设备ID
	// CMessageDeviceRealtimeData   deviceData;  旧版
	REALTIME_DATA m_vectorData; //新版 2017 12 21
	REALTIME_DATA_NEW m_vectorDataNew; //20180123 新增 保存新协议的数据链
	//20180123 增加一个 版本标识  0老版本  1新版本
	unsigned char  m_flag;

	//默认是老版本
    _T_DEVICE_DATA(QString deviceID, const REALTIME_DATA& deviceData, unsigned char flag = OLD_TYPE)
        : qstrDeviceID(deviceID),m_flag(flag)
    {
        this->m_vectorData = deviceData;
    }
	//20180123 新协议的 数据链 构造
	_T_DEVICE_DATA(QString deviceID, const REALTIME_DATA_NEW& deviceData, unsigned char flag = NEW_TYPE)
		: qstrDeviceID(deviceID), m_flag(flag)
	{
		this->m_vectorDataNew = deviceData;
	}
}T_DEVICE_DATA;



/*
设备关联的设备信息
*/

struct DEVICE_MSG
{
	QString m_companyName;
	QString m_longitude;
	QString m_latitude;
	QString m_boilerId;
	QString m_address;
};


class CDBService : public IInternalService
{
public:

    static CDBService * GetInstance();

    static void DestroyInstance();

    virtual int StartInternalService();

    virtual int StopInternalService();

    virtual int NotifyServiceToStop();

    virtual QString GetServiceName();

    //////////////////////////////////////////////////////////////////////////
    // 具体的操作接口

    // 用户名、密码是否匹配
    bool IsMatchedAccount(const QString & account, const QString & password);

    // 是否为已注册的设备
    bool IsExistedDevice(const QString & deviceCode, const QString & password);

    // 用户名是否存在
    bool IsValidUser(const QString & account);

    // 设备是否有效
    bool IsValidDevice(const QString & account);

    // 获取账号ID
    bool GetAccountID(const QString & account, QString & accountID);

    // 获取设备ID
    bool GetDeviceIDByDeviceCode(const QString & deviceCode, QString & deviceID);

    // 通过账号ID获取所属井队ID
    bool GetRigIDByAccountID(const QString & accountID, QString & rigID);

    // 通过井队ID获取井队信息
    bool GetRigInfoByID(const QString & rigID, CRigInfo & rigInfo);

    // 通过井队ID获取所属油田ID
    bool GetOilFieldIDByRigID(const QString & rigID, QString & oilFieldID);

    // 通过油田ID，获取油田信息
    bool GetOilFieldInfoByID(const QString & oilFieldID, COilFieldInfo & oilFieldInfo);

    // 通过账号获取设备列表
    bool GetH2SDeviceListByPersonID(const QString & accountID,QList<CMessageDevicesListItem *> & lstDevices);

    // 通过设备ID取得设备信息
    bool GetDeviceInfoByDeviceID(const QString & deviceID, CMessageDeviceItemInfo * pDevice);

    // 通过设备ID获取所属井队ID
    bool GetRigIDByDeviceID(const QString & deviceID, QString & rigID);

    // 取得所有油田信息
    bool GetOilFieldList(QList<CBaseEntity> & lstResult);

    // 通过油田ID取得下属井队信息
    bool GetRigListByOilFieldID(const QString & oilFieldID, QList<CBaseEntity> & lstResult);

    // 通过井队ID取得下属设备信息
    bool GetDeviceListByRigID(const QString & rigID,QList<CDeviceInfo> & lstResult);

    // 设置设备参数
    bool SetDeviceParam(E_DEVICE_TYPE deviceType, const QString & deviceID, CMessageSetDeviceParam & deviceParam);

    // 设置设备名
    bool SetDeviceName(const QString & deviceID, const QString & deviceName);

    // 获取井队设备历史数据在指定时间段里的条数
    bool GetDeviceHistoryDataCount(const quint64 & begin, const quint64 & end, QVector<QString> deviceIDs, int & count);

    // 获取井队设备历史数据
    bool GetDeviceHistoryData(const quint64 & begin, const quint64 & end, int pageIndex, int itemsInPerPage, QVector<QString> deviceIDs, QList<CMessageDeviceDataItem *> & lst);

    // 是否存在表
    bool IsExistTable(const QString & tableName);

    // 创建数据表
	//20180123 新增.一个分表标志 0 老表 1 新表 默认是新表
    bool CreateDataTable(const QString & tableName,int flag = NEW_TYPE);

    // 在数据表中插入数据
    bool InsertDataTable(const QString & tableName, const QString deviceID, const REALTIME_DATA & deviceData);

    // 在数据表中批量插入数据
    bool InsertDataTable(const QString & tableName, QList<T_DEVICE_DATA> & lst);

    //数据处理列队加入数据
	//20180123 新增更改, 增加一个版本控制
    bool DataQueuePushBack(const QString & deviceID, const REALTIME_DATA& data
		,unsigned char flag = OLD_TYPE);
	bool DataQueuePushBack(const QString & deviceID, const REALTIME_DATA_NEW& data
		, unsigned char flag = NEW_TYPE);

    // 数据处理列队排出数据
    T_DEVICE_DATA& DataQueuePopFront();

    // 获取数据处理队列队首元素
    T_DEVICE_DATA& DataQueueFront();




private:

    CDBService();

    ~CDBService();

    // 获取数据库连接
    db_con_ptr GetDBConnection(E_DB_OPEN_FLAG flag);

    // 创建新的数据库连接
    sqldb_ptr CreateDBConnection(QString & connectionName);

    // 关闭所有数据库连接
    void DestroyDBConnections();

private:
	QMap<QString, DEVICE_MSG>      m_deviceMsgMap; //设备关联的设备信息表

    static CDBService *             m_pInstance;

    QMutex                        * m_pMutexForSet;
    QSet<QString>                   m_setTableName;

    // 懒得做多个连接的管理，只提供一个可读的和一个可写的数据库连接
    QMutex                        * m_pDBConnectionLock;
    db_con_ptr                      m_pDBConnectionForReadOnly;// 只读数据库连接
    db_con_ptr                      m_pDBConnectionForReadWrite;// 可读写数据库连接

    CSyncCondQueue<T_DEVICE_DATA>    m_dataQueue;// 采集数据队列

    friend class CDBHandleTask;
};



class CDBHandleTask : public QRunnable
{
    virtual void run();
};


#endif//!_DB_SERVICE_H_