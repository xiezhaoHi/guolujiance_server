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

// ���ݿ����Ӷ�д��ʶ
typedef enum _E_DB_OPEN_FLAG
{
    DB_CON_READONLY = 0,
    DB_CON_READWRITE,
}E_DB_OPEN_FLAG;

typedef std::shared_ptr<QSqlDatabase> sqldb_ptr;

// ���ݿ�����
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


//20180123 ���������ݽṹ
//�ɰ����ݺ;� ����ʽ   0
//�°����ݺ���Э�� ����ʽ 1
#define OLD_TYPE 0
#define NEW_TYPE 1

typedef struct  _NEW_DEVICE_DATA
{
	U16 m_flag; //�����ʶ
	QString m_strFlag;//�ַ�����ʾ
	U32 m_data; //����ֵ
	_NEW_DEVICE_DATA(short flag, int data,QString strFlag) 
		:m_flag(flag), m_data(data),m_strFlag(strFlag)
	{}
}NEW_DEVICE_DATA;
typedef QList<NEW_DEVICE_DATA> REALTIME_DATA_NEW;
// �豸�ɼ�����
typedef struct _T_DEVICE_DATA
{
    QString                      qstrDeviceID; //�豸ID
	// CMessageDeviceRealtimeData   deviceData;  �ɰ�
	REALTIME_DATA m_vectorData; //�°� 2017 12 21
	REALTIME_DATA_NEW m_vectorDataNew; //20180123 ���� ������Э���������
	//20180123 ����һ�� �汾��ʶ  0�ϰ汾  1�°汾
	unsigned char  m_flag;

	//Ĭ�����ϰ汾
    _T_DEVICE_DATA(QString deviceID, const REALTIME_DATA& deviceData, unsigned char flag = OLD_TYPE)
        : qstrDeviceID(deviceID),m_flag(flag)
    {
        this->m_vectorData = deviceData;
    }
	//20180123 ��Э��� ������ ����
	_T_DEVICE_DATA(QString deviceID, const REALTIME_DATA_NEW& deviceData, unsigned char flag = NEW_TYPE)
		: qstrDeviceID(deviceID), m_flag(flag)
	{
		this->m_vectorDataNew = deviceData;
	}
}T_DEVICE_DATA;



/*
�豸�������豸��Ϣ
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
    // ����Ĳ����ӿ�

    // �û����������Ƿ�ƥ��
    bool IsMatchedAccount(const QString & account, const QString & password);

    // �Ƿ�Ϊ��ע����豸
    bool IsExistedDevice(const QString & deviceCode, const QString & password);

    // �û����Ƿ����
    bool IsValidUser(const QString & account);

    // �豸�Ƿ���Ч
    bool IsValidDevice(const QString & account);

    // ��ȡ�˺�ID
    bool GetAccountID(const QString & account, QString & accountID);

    // ��ȡ�豸ID
    bool GetDeviceIDByDeviceCode(const QString & deviceCode, QString & deviceID);

    // ͨ���˺�ID��ȡ��������ID
    bool GetRigIDByAccountID(const QString & accountID, QString & rigID);

    // ͨ������ID��ȡ������Ϣ
    bool GetRigInfoByID(const QString & rigID, CRigInfo & rigInfo);

    // ͨ������ID��ȡ��������ID
    bool GetOilFieldIDByRigID(const QString & rigID, QString & oilFieldID);

    // ͨ������ID����ȡ������Ϣ
    bool GetOilFieldInfoByID(const QString & oilFieldID, COilFieldInfo & oilFieldInfo);

    // ͨ���˺Ż�ȡ�豸�б�
    bool GetH2SDeviceListByPersonID(const QString & accountID,QList<CMessageDevicesListItem *> & lstDevices);

    // ͨ���豸IDȡ���豸��Ϣ
    bool GetDeviceInfoByDeviceID(const QString & deviceID, CMessageDeviceItemInfo * pDevice);

    // ͨ���豸ID��ȡ��������ID
    bool GetRigIDByDeviceID(const QString & deviceID, QString & rigID);

    // ȡ������������Ϣ
    bool GetOilFieldList(QList<CBaseEntity> & lstResult);

    // ͨ������IDȡ������������Ϣ
    bool GetRigListByOilFieldID(const QString & oilFieldID, QList<CBaseEntity> & lstResult);

    // ͨ������IDȡ�������豸��Ϣ
    bool GetDeviceListByRigID(const QString & rigID,QList<CDeviceInfo> & lstResult);

    // �����豸����
    bool SetDeviceParam(E_DEVICE_TYPE deviceType, const QString & deviceID, CMessageSetDeviceParam & deviceParam);

    // �����豸��
    bool SetDeviceName(const QString & deviceID, const QString & deviceName);

    // ��ȡ�����豸��ʷ������ָ��ʱ����������
    bool GetDeviceHistoryDataCount(const quint64 & begin, const quint64 & end, QVector<QString> deviceIDs, int & count);

    // ��ȡ�����豸��ʷ����
    bool GetDeviceHistoryData(const quint64 & begin, const quint64 & end, int pageIndex, int itemsInPerPage, QVector<QString> deviceIDs, QList<CMessageDeviceDataItem *> & lst);

    // �Ƿ���ڱ�
    bool IsExistTable(const QString & tableName);

    // �������ݱ�
	//20180123 ����.һ���ֱ��־ 0 �ϱ� 1 �±� Ĭ�����±�
    bool CreateDataTable(const QString & tableName,int flag = NEW_TYPE);

    // �����ݱ��в�������
    bool InsertDataTable(const QString & tableName, const QString deviceID, const REALTIME_DATA & deviceData);

    // �����ݱ���������������
    bool InsertDataTable(const QString & tableName, QList<T_DEVICE_DATA> & lst);

    //���ݴ����жӼ�������
	//20180123 ��������, ����һ���汾����
    bool DataQueuePushBack(const QString & deviceID, const REALTIME_DATA& data
		,unsigned char flag = OLD_TYPE);
	bool DataQueuePushBack(const QString & deviceID, const REALTIME_DATA_NEW& data
		, unsigned char flag = NEW_TYPE);

    // ���ݴ����ж��ų�����
    T_DEVICE_DATA& DataQueuePopFront();

    // ��ȡ���ݴ�����ж���Ԫ��
    T_DEVICE_DATA& DataQueueFront();




private:

    CDBService();

    ~CDBService();

    // ��ȡ���ݿ�����
    db_con_ptr GetDBConnection(E_DB_OPEN_FLAG flag);

    // �����µ����ݿ�����
    sqldb_ptr CreateDBConnection(QString & connectionName);

    // �ر��������ݿ�����
    void DestroyDBConnections();

private:
	QMap<QString, DEVICE_MSG>      m_deviceMsgMap; //�豸�������豸��Ϣ��

    static CDBService *             m_pInstance;

    QMutex                        * m_pMutexForSet;
    QSet<QString>                   m_setTableName;

    // ������������ӵĹ���ֻ�ṩһ���ɶ��ĺ�һ����д�����ݿ�����
    QMutex                        * m_pDBConnectionLock;
    db_con_ptr                      m_pDBConnectionForReadOnly;// ֻ�����ݿ�����
    db_con_ptr                      m_pDBConnectionForReadWrite;// �ɶ�д���ݿ�����

    CSyncCondQueue<T_DEVICE_DATA>    m_dataQueue;// �ɼ����ݶ���

    friend class CDBHandleTask;
};



class CDBHandleTask : public QRunnable
{
    virtual void run();
};


#endif//!_DB_SERVICE_H_