#include "db_service.h"
#include "service/log_service/log_service.h"
#include "service/configuration_service/configuration_service.h"
#include "service/task_service/task_service.h"
#include "registration/registration.h"
#include "util/global_util.h"
#include <atltime.h>  
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDateTime>
#include <QThread>
#include <QJsonObject>
#include <QJsonDocument>
#include  "mydatabase.h"

// �û� SYS_User(F_LoginName, F_Pwd, F_PersonID)
// ������Ա Rig_RigPersonInfo(ID, RigID, )
// ���ӻ�����Ϣ Rig_RigBaseInfo(ID, Name, Address, Longitude, Latitude)
/// ����ʽH2S����� DEV_PORTABLEh2sdetector
/// �̶�ʽH2S����� DEV_StaticH2SDetector
// �豸��Ϣ�� Rig_DeviceInfo(ID, RigID, DeviceType[guidcode], State)
// 


enum mamos_ch_index
{
	MAMOS_O2 = 0,		// 20.95%
	MAMOS_CO,			// 12ppm
	MAMOS_NO,			// 24ppm
	MAMOS_SO2,			// 56ppm
	MAMOS_NO2,			// 888ppm
	MAMOS_CO2,			// 23.13%
	MAMOS_CH_NUM,
	MAMOS_ID = MAMOS_CH_NUM // ID
};

static char*  gMamosKey[] = { "V_PER_O2","V_PPM_CO", "V_PPM_NO", "V_PPM_SO2"
, "V_PPM_NO2", "V_PER_CO2", nullptr };

CDBService * CDBService::m_pInstance = NULL;


CDBService::CDBService()
{
    m_pDBConnectionForReadOnly = NULL;
    m_pDBConnectionForReadWrite = NULL;
    m_pMutexForSet = new QMutex(QMutex::Recursive);
    m_pDBConnectionLock = new QMutex();
}

CDBService::~CDBService()
{
    delete m_pMutexForSet;
    if (m_pDBConnectionForReadOnly) {
        if (m_pDBConnectionForReadOnly->pDB->isOpen()) {
            m_pDBConnectionForReadOnly->pDB->close();
            QSqlDatabase::removeDatabase(m_pDBConnectionForReadOnly->connectionName);
        }
    }

    if (m_pDBConnectionForReadWrite) {
        if (m_pDBConnectionForReadWrite->pDB->isOpen()) {
            m_pDBConnectionForReadWrite->pDB->close();
            QSqlDatabase::removeDatabase(m_pDBConnectionForReadWrite->connectionName);
        }
    }

    delete m_pDBConnectionLock;
}

CDBService * CDBService::GetInstance()
{
    if (!m_pInstance) {
        m_pInstance = new CDBService();
    }
    return m_pInstance;
}

void CDBService::DestroyInstance()
{
    if (m_pInstance) {
        m_pInstance->DestroyDBConnections();
        delete m_pInstance;
        m_pInstance = NULL;
    }
}

int CDBService::StartInternalService()
{
#ifdef USEMYSQL

#else
    db_con_ptr con = GetDBConnection(DB_CON_READONLY);
    if (!con) {
        return 1;
    }
    m_pDBConnectionForReadOnly = con;
#endif
    // ����ר�Ų���ɼ����ݵ��߳�
    CTaskService::GetInstance()->CommitTask(new CDBHandleTask());
    return 0;
}

int CDBService::StopInternalService()
{
    NotifyServiceToStop();
    return 0;
}

int CDBService::NotifyServiceToStop()
{
    IInternalService::NotifyServiceToStop();
    return 0;
}

QString CDBService::GetServiceName()
{
    return QStringLiteral("db service");
}

db_con_ptr CDBService::GetDBConnection(E_DB_OPEN_FLAG flag)
{
    QMutexLocker lock(m_pDBConnectionLock);
    db_con_ptr pCon;

    if (DB_CON_READONLY == flag) {
        pCon = m_pDBConnectionForReadOnly;
    } else if (DB_CON_READWRITE == flag) {
        pCon = m_pDBConnectionForReadWrite;
    } else {
        return NULL;
    }
	BOOL t1, t2;
	
	if (pCon && pCon->pDB)
	{
		t1 = pCon->pDB->isOpen();
		t2 = pCon->pDB->isValid();
	}
	
    if (pCon && pCon->pDB&& pCon->pDB->isOpen()) {
        // ���ݿ�����������ֱ�ӷ���ԭ�����ݿ�����
    } else {
        if (pCon && pCon->pDB && !pCon->pDB->isOpen()) {// ���ݿ����ӶϿ�����Ҫ��������
            pCon->pDB->close();
            QSqlDatabase::removeDatabase(pCon->connectionName);
            LOG_WARING() << QStringLiteral("���ݿ�����[%1]�ѶϿ�,������Ϣ[%2]")
                .arg(pCon->pDB->connectionName())
                .arg(pCon->pDB->lastError().text());
        }

        QString qstrConnectionName;
        sqldb_ptr db = CreateDBConnection(qstrConnectionName);
        if (!db) {
            return NULL;
        }
        pCon = std::make_shared<T_DB_CON>(db, qstrConnectionName, flag);

        if (DB_CON_READONLY == flag) {
            m_pDBConnectionForReadOnly = pCon;
        } else if (DB_CON_READWRITE == flag) {
            m_pDBConnectionForReadWrite = pCon;
        }
    }

    return pCon;
}

sqldb_ptr CDBService::CreateDBConnection(QString & connectionName)
{
    // �����Ȩ��֤
    IS_REGISTED(NULL);

    QString qstrConnectionName =  QDateTime::currentDateTime().toLocalTime()
        .toString(QStringLiteral("con_yyyyMMdd_HHmmss_zzz"));
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC", qstrConnectionName);
    db.setDatabaseName(CConfigrationService::GetInstance()->GetDBConnStr());
	//db.setDatabaseName(QString::fromLocal8Bit("QTDSN"));
    db.setHostName(CConfigrationService::GetInstance()->GetDBHostName());
    db.setUserName(CConfigrationService::GetInstance()->GetDBUser());
    db.setPassword(CConfigrationService::GetInstance()->GetDBPassword());
    db.setPort(CConfigrationService::GetInstance()->GetDBPort());
    
    if (!db.open()) {
        LOG_WARING() << QStringLiteral("�����ݿ�ʧ�� error:") << db.lastError().text();
        return NULL;
    }

    LOG_INFO() << QStringLiteral("�½����ݿ�����[%1]�ɹ�,���ݿ������ַ���[%2]")
        .arg(db.connectionName())
        .arg(CConfigrationService::GetInstance()->GetDBConnStr());

    connectionName = qstrConnectionName;
    return std::make_shared<QSqlDatabase>(db);
}

void CDBService::DestroyDBConnections()
{
    if (m_pDBConnectionForReadWrite) {
        if (m_pDBConnectionForReadWrite->pDB->isOpen()) {
            m_pDBConnectionForReadWrite->pDB->close();
            LOG_INFO() << QStringLiteral("�رտɶ�д���ݿ�����");
        }
        m_pDBConnectionForReadWrite->pDB = NULL;
        m_pDBConnectionForReadWrite = NULL;
    }

    if (m_pDBConnectionForReadOnly) {
        if (m_pDBConnectionForReadOnly->pDB->isOpen()) {
            m_pDBConnectionForReadOnly->pDB->close();
            LOG_INFO() << QStringLiteral("�ر�ֻ�����ݿ�����");
        }
        m_pDBConnectionForReadOnly->pDB = NULL;
        m_pDBConnectionForReadOnly = NULL;
    }
}

//******************************************************
//** ������:   IsMatchedAccount
//** ���ܼ���: �ж��û����������Ƿ�ƥ��
//** �������: 
//   const QString & account:
//   const QString & password:
//
//** ����ֵ: 
//   bool:
//
//** �������ڣ�2015/07/26
//** �����ˣ�  ����
//** �޸ļ�¼��
//   ����        �汾      �޸���    �޸�����
//
//** ����˵����
//******************************************************
bool CDBService::IsMatchedAccount(const QString & account, const QString & password)
{
    Q_ASSERT(account.length() > 0 && password.length() > 0);

    db_con_ptr con = GetDBConnection(DB_CON_READONLY);
    if (!con) {
        return false;
    }

    con->AcquireWrite();
    QSqlQuery query(*(con->pDB));
    QString qstrSQL = QStringLiteral("SELECT * FROM SYS_User WHERE F_LoginName = ? AND F_Pwd = ?");

    if (!query.prepare(qstrSQL)) {
        LOG_WARING() << "SQLQuery prepare failed. error:" << con->pDB->lastError().text()
            << " sql:"  << qstrSQL;
        con->Release();
        return false;
    }

    query.bindValue(0, account);
    query.bindValue(1, password);

    if (!query.exec()) {
        LOG_WARING() << "SQLQuery exec failed. error:" << con->pDB->lastError().text()
            << " sql:"  << qstrSQL;
        con->Release();
        return false;
    }

    bool ret = query.record().count() > 0;
    con->Release();

    if (!ret) {
        LOG_WARING() << QStringLiteral("��ѯ�û���������ʧ��. error:") << con->pDB->lastError().text()
            << " sql:"  << query.executedQuery();
    }

    return ret;
}

//******************************************************
//** ������:   IsValidUser
//** ���ܼ���: �ж��Ƿ�Ϊ��Ч�û�
//** �������: 
//   const QString & account:
//
//** ����ֵ: 
//   bool:
//
//** �������ڣ�2015/07/26
//** �����ˣ�  ����
//** �޸ļ�¼��
//   ����        �汾      �޸���    �޸�����
//
//** ����˵����
//******************************************************
bool CDBService::IsValidUser(const QString & account)
{
    Q_ASSERT(account.length() > 0);

    db_con_ptr con = GetDBConnection(DB_CON_READONLY);
    if (!con) {
        return false;
    }

    con->AcquireWrite();
    QSqlQuery query(*(con->pDB));
    QString qstrSQL = QStringLiteral("SELECT * FROM SYS_User WHERE F_LoginName = ?");

    if (!query.prepare(qstrSQL)) {
        LOG_WARING() << "SQLQuery prepare failed. error:" << con->pDB->lastError().text()
            << " sql:"  << qstrSQL;
        con->Release();
        return false;
    }

    query.bindValue(0, account);
    if (!query.exec()) {
        LOG_WARING() << "SQLQuery exec failed. error:" << con->pDB->lastError().text()
            << " sql:"  << qstrSQL;
        con->Release();
        return false;
    }

    bool ret = query.next();
    con->Release();
    return ret;
}

//******************************************************
//** ������:   GetAccountID
//** ���ܼ���: ��ȡ�û�ID
//** �������: 
//   const QString & account:
//   QString & accountID:
//
//** ����ֵ: 
//   bool:
//
//** �������ڣ�2015/07/26
//** �����ˣ�  ����
//** �޸ļ�¼��
//   ����        �汾      �޸���    �޸�����
//
//** ����˵����
//******************************************************
bool CDBService::GetAccountID(const QString & account, QString & accountID)
{
    Q_ASSERT(account.length() > 0);

    db_con_ptr con = GetDBConnection(DB_CON_READONLY);
    if (!con) {
        return false;
    }

    con->AcquireWrite();
    QSqlQuery query(*(con->pDB));
    QString qstrSQL =  QStringLiteral("SELECT F_ID FROM SYS_User WHERE F_LoginName = ?");

    if (!query.prepare(qstrSQL)) {
        LOG_WARING() << "SQLQuery prepare failed. error:" << con->pDB->lastError().text()
            << " sql:"  << qstrSQL;
        con->Release();
        return false;
    }

    query.bindValue(0, account);
    if (!query.exec()) {
        LOG_WARING() << "SQLQuery exec failed. error:" << con->pDB->lastError().text()
            << " sql:"  << qstrSQL;
        con->Release();
        return false;
    }

    bool ret = query.next();
    if (ret) {
        accountID.clear();
        accountID = query.value(0).toString();
    }

    con->Release();
    return ret;
}


//******************************************************
//** ������:   GetRigIDByAccountID
//** ���ܼ���: ͨ���û�ID��ȡ����������ID
//** �������: 
//   const QString & accountID:
//   QString & rigID:
//
//** ����ֵ: 
//   bool:
//
//** �������ڣ�2015/07/30
//** �����ˣ�  ����
//** �޸ļ�¼��
//   ����        �汾      �޸���    �޸�����
//
//** ����˵����
//******************************************************
bool CDBService::GetRigIDByAccountID(const QString & accountID, QString & rigID)
{
    Q_ASSERT(accountID.length() > 0);

    db_con_ptr con = GetDBConnection(DB_CON_READONLY);
    if (!con) {
        return false;
    }

    con->AcquireWrite();
    QSqlQuery query(*(con->pDB));
    QString qstrSQL = QStringLiteral("SELECT RigID FROM Rig_RigPersonInfo WHERE UserId = ?");

    if (!query.prepare(qstrSQL)) {
        LOG_WARING() << "SQLQuery prepare failed. error:" << con->pDB->lastError().text()
            << " sql:"  << qstrSQL;
        con->Release();
        return false;
    }

    query.bindValue(0, accountID);
    if (!query.exec()) {
        LOG_WARING() << "SQLQuery exec failed. error:" << con->pDB->lastError().text()
            << " sql:"  << qstrSQL;
        con->Release();
        return false;
    }

    bool ret = query.next();
    if (ret) {
        rigID.clear();
        rigID = query.value(0).toString();
    }
    con->Release();

    return ret;
}

//******************************************************
//** ������:   GetRigInfoByID
//** ���ܼ���: ���ݾ���ID��ȡ������ϸ��Ϣ
//** �������: 
//   const QString & rigID:
//   CRigInfo & rigInfo:
//
//** ����ֵ: 
//   bool:
//
//** �������ڣ�2015/07/30
//** �����ˣ�  ����
//** �޸ļ�¼��
//   ����        �汾      �޸���    �޸�����
//
//** ����˵����
//******************************************************
bool CDBService::GetRigInfoByID(const QString & rigID, CRigInfo & rigInfo)
{
    // TODO
    return false;
}

//******************************************************
//** ������:   GetOilFieldIDByRigID
//** ���ܼ���: ���ݾ���ID��ȡ��������ID
//** �������: 
//   const QString & rigID:
//   QString & oilFieldID:
//
//** ����ֵ: 
//   bool:
//
//** �������ڣ�2015/07/30
//** �����ˣ�  ����
//** �޸ļ�¼��
//   ����        �汾      �޸���    �޸�����
//
//** ����˵����
//******************************************************
bool CDBService::GetOilFieldIDByRigID(const QString & rigID, QString & oilFieldID)
{
	return true;
    Q_ASSERT(rigID.length() > 0);

    db_con_ptr con = GetDBConnection(DB_CON_READONLY);
    if (!con) {
        return false;
    }

    con->AcquireWrite();
    QSqlQuery query(*(con->pDB));
    QString qstrSQL = QStringLiteral("SELECT OilFieldID FROM Rig_RigBaseInfo WHERE ID = ?");

    if (!query.prepare(qstrSQL)) {
        LOG_WARING() << "SQLQuery prepare failed. error:" << con->pDB->lastError().text()
            << " sql:"  << qstrSQL;
        con->Release();
        return false;
    }

    query.bindValue(0, rigID);
    if (!query.exec()) {
        LOG_WARING() << "SQLQuery exec failed. error:" << con->pDB->lastError().text()
            << " sql:"  << qstrSQL;
        con->Release();
        return false;
    }

    bool ret = query.next();
    if (ret) {
        oilFieldID.clear();
        oilFieldID = query.value(0).toString();
    }
    con->Release();

    return true;
}


//******************************************************
//** ������:   GetOilFieldInfoByID
//** ���ܼ���: ��������ID��ȡ������Ϣ
//** �������: 
//   const QString & oilFieldID:
//   COilFieldInfo & oilFieldInfo:
//
//** ����ֵ: 
//   bool:
//
//** �������ڣ�2015/07/30
//** �����ˣ�  ����
//** �޸ļ�¼��
//   ����        �汾      �޸���    �޸�����
//
//** ����˵����
//******************************************************
bool CDBService::GetOilFieldInfoByID(const QString & oilFieldID, COilFieldInfo & oilFieldInfo)
{

    // TODO
    return false;
}


//******************************************************
//** ������:   GetDeviceListByAccountID
//** ���ܼ���: ��ȡ�û�ID
//** �������: 
//   const QString & account:
//   QList<CMessageDevicesListItem *> lstDevices:
//
//** ����ֵ: 
//   bool:
//
//** �������ڣ�2015/07/26
//** �����ˣ�  ���
//** �޸ļ�¼��
//   ����        �汾      �޸���    �޸�����
//
//** ����˵����
//******************************************************
bool CDBService::GetH2SDeviceListByPersonID(const QString & accountID, QList<CMessageDevicesListItem *> & lstDevices)
{
    Q_ASSERT(accountID.length() > 0);

    db_con_ptr con = GetDBConnection(DB_CON_READONLY);
    if (!con) {
        return false;
    }

    con->AcquireWrite();
    QSqlQuery query(*(con->pDB));

    QString qstrSQL = QStringLiteral("SELECT\
                   ID, Code, Name, WarningLine1st, WarningLine2nd, MaxRange, SampleRate, DevType\
                   FROM\
                   (\
                   (\
                   SELECT a.RigID, a.ID, a.Code, a.Name, b.WarningLine1st, b.WarningLine2nd, b.MaxRange, b.SampleRate, 1 as DevType\
                   FROM\
                   Rig_DeviceInfo a\
                   INNER JOIN DEV_PortableH2SDetector b ON a.ID = b.DeviceID\
                   WHERE\
                   a.DeviceType = ?\
                   ) a\
                   INNER JOIN (\
                   SELECT\
                   RigID\
                   FROM\
                   Rig_RigPersonInfo\
                   WHERE\
                   UserId = ?\
                   ) b ON a.RigID = b.RigID\
                   )\
                   UNION\
                   SELECT ID, Code, Name, WarningLine1st, WarningLine2nd, MaxRange, SampleRate, 2 as DevType\
                   FROM\
                   (\
                   (\
                   SELECT a.RigID, a.ID, a.Code, a.Name, b.WarningLine1st, b.WarningLine2nd, b.MaxRange, b.SampleRate\
                   FROM\
                   Rig_DeviceInfo a\
                   INNER JOIN DEV_StaticH2SDetector b ON a.ID = b.DeviceID\
                   WHERE\
                   a.DeviceType = ?\
                   ) a\
                   INNER JOIN (\
                   SELECT\
                   RigID\
                   FROM\
                   Rig_RigPersonInfo\
                   WHERE\
                   UserId = ?\
                   ) b ON a.RigID = b.RigID\
                   ) ORDER BY DevType");

    if (!query.prepare(qstrSQL)) {
        LOG_WARING() << "SQLQuery prepare failed. error:" << con->pDB->lastError().text()
            << " sql:"  << qstrSQL;
        con->Release();
        return false;
    }

    query.bindValue(0, CConfigrationService::GetInstance()->GetTypeCodeOfPortableH2SDetector());
    query.bindValue(1, accountID);
    query.bindValue(2, CConfigrationService::GetInstance()->GetTypeCodeOfStaticH2SDetector());
    query.bindValue(3, accountID);

    if (!query.exec()) {
        LOG_WARING() << "SQLQuery exec failed. error:" << con->pDB->lastError().text()
            << " sql:"  << qstrSQL;
        con->Release();
        return false;
    }

    bool ret = query.next();
    while (ret) {
        CMessageDevicesListItem * deviceItem = new CMessageDevicesListItem();
        strcpy((char *)deviceItem->m_struct.deviceId, query.value(0).toString().toLocal8Bit().data());
        strcpy((char *)deviceItem->m_struct.deviceNo, query.value(1).toString().toLocal8Bit().data());
        strcpy((char *)deviceItem->m_struct.deviceName, query.value(2).toString().toLocal8Bit().data());
        strcpy((char *)deviceItem->m_struct.warningLine1st, query.value(3).toString().toLocal8Bit().data());
        strcpy((char *)deviceItem->m_struct.warningLine2nd, query.value(4).toString().toLocal8Bit().data());
        strcpy((char *)deviceItem->m_struct.maxRange, query.value(5).toString().toLocal8Bit().data());
        deviceItem->SetSampleRate(query.value(6).toString());
        deviceItem->m_struct.devType = query.value(7).toInt();
        lstDevices.append(deviceItem);
        ret = query.next();
    }

    con->Release();
    return true;
}
//******************************************************
//** ������:   GetDeviceInfoByDeviceID
//** ���ܼ���: ��ȡ�û�ID
//** �������: 
//   const QString & account:
//   QString & accountID:
//
//** ����ֵ: 
//   bool:
//
//** �������ڣ�2015/07/26
//** �����ˣ�  ����
//** �޸ļ�¼��
//   ����        �汾      �޸���    �޸�����
//
//** ����˵����
//******************************************************
bool CDBService::GetDeviceInfoByDeviceID(const QString & deviceID,CMessageDeviceItemInfo * pDevice)
{
	return true;
    Q_ASSERT(deviceID.length() > 0);

    db_con_ptr con = GetDBConnection(DB_CON_READONLY);
    if (!con) {
        return false;
    }

    con->AcquireWrite();
    QSqlQuery query(*(con->pDB));
    // ɸѡ�������豸��code��Ϊ�豸ID tangqiao 2015/07/30
    QString qstrSQL = QStringLiteral("SELECT a.ID,a.Code,a.Name,a.Model,a.State,a.Manufacturers,a.MainParams,b.Name as RigName,b.Company,b.Address FROM Rig_DeviceInfo a INNER JOIN Rig_RigBaseInfo b ON a.RigID=b.ID WHERE a.ID= ?");

    if (!query.prepare(qstrSQL)) {
        LOG_WARING() << "SQLQuery prepare failed. error:" << con->pDB->lastError().text()
            << " sql:"  << qstrSQL;
        con->Release();
        return false;
    }

    query.bindValue(0, deviceID);
    if (!query.exec()) {
        LOG_WARING() << "SQLQuery exec failed. error:" << con->pDB->lastError().text()
            << " sql:"  << qstrSQL;
        con->Release();
        return false;
    }

    bool ret = query.next();
    if (ret) {
        strcpy((char *)pDevice->m_struct.deviceId, query.value(0).toString().toLocal8Bit().data());
        strcpy((char *)pDevice->m_struct.deviceNo, query.value(1).toString().toLocal8Bit().data());
        strcpy((char *)pDevice->m_struct.deviceName, query.value(2).toString().toLocal8Bit().data());
        strcpy((char *)pDevice->m_struct.deviceModel, query.value(3).toString().toLocal8Bit().data());
        strcpy((char *)pDevice->m_struct.deviceState, query.value(4).toString().toLocal8Bit().data());
        strcpy((char *)pDevice->m_struct.deviceManufacturers, query.value(5).toString().toLocal8Bit().data());
        strcpy((char *)pDevice->m_struct.deviceMainParams, query.value(6).toString().toLocal8Bit().data());
        strcpy((char *)pDevice->m_struct.deviceRigName, query.value(7).toString().toLocal8Bit().data());
        strcpy((char *)pDevice->m_struct.deviceCompany, query.value(8).toString().toLocal8Bit().data());
        strcpy((char *)pDevice->m_struct.deviceAddress, query.value(9).toString().toLocal8Bit().data());
    }

    con->Release();
    return ret;
}


//******************************************************
//** ������:   GetRigIDByDeviceID
//** ���ܼ���: ͨ���豸ID��ȡ��������ID
//** �������: 
//   const QString & deviceID:
//   QString & rigID:
//
//** ����ֵ: 
//   bool:
//
//** �������ڣ�2015/07/30
//** �����ˣ�  ����
//** �޸ļ�¼��
//   ����        �汾      �޸���    �޸�����
//
//** ����˵����
//******************************************************
bool CDBService::GetRigIDByDeviceID(const QString & deviceID, QString & rigID)
{
	return true;
    Q_ASSERT(deviceID.length() > 0);

    db_con_ptr con = GetDBConnection(DB_CON_READONLY);
    if (!con) {
        return false;
    }

    con->AcquireWrite();
    QSqlQuery query(*(con->pDB));
    QString qstrSQL = QStringLiteral("SELECT RigID FROM Rig_DeviceInfo WHERE ID = ?");

    if (!query.prepare(qstrSQL)) {
        LOG_WARING() << "SQLQuery prepare failed. error:" << con->pDB->lastError().text()
            << " sql:"  << qstrSQL;
        con->Release();
        return false;
    }

    query.bindValue(0, deviceID);
    if (!query.exec()) {
        LOG_WARING() << "SQLQuery exec failed. error:" << con->pDB->lastError().text()
            << " sql:"  << qstrSQL;
        con->Release();
        return false;
    }

    bool ret = query.next();
    if (ret) {
        rigID.clear();
        rigID = query.value(0).toString();
    }
    con->Release();

    return ret;
}


bool CDBService::IsValidDevice(const QString & account)
{
    // TOOD �豸���&�豸����

    return true;
}

bool CDBService::IsExistedDevice(const QString & deviceCode, const QString & password)
{
	
     Q_ASSERT(deviceCode.length() > 0);
     Q_ASSERT(deviceCode.length() > 0 && password.length() > 0);

     db_con_ptr con = GetDBConnection(DB_CON_READONLY);
     if (!con) {
         return false;
     }

     con->AcquireWrite();
     QSqlQuery query(*(con->pDB)); 
    // QString qstrSQL = QStringLiteral("SELECT * FROM Rig_DeviceInfo WHERE Code = ?");
		 QString qstrSQL = QStringLiteral("SELECT * FROM Device WHERE Number = ?");
     if (!query.prepare(qstrSQL)) {
         LOG_WARING() << "SQLQuery prepare failed. error:" << con->pDB->lastError().text()
             << " sql:"  << qstrSQL;
         con->Release();
         return false;
     }

     query.bindValue(0, deviceCode);

     if (!query.exec()) {
         LOG_WARING() << "SQLQuery exec failed. error:" << con->pDB->lastError().text()
             << " sql:"  << qstrSQL;
         con->Release();
         return false;
     }

     bool ret = query.record().count() > 0;
     con->Release();
     return ret;
}


bool CDBService::GetDeviceIDByDeviceCode(const QString & deviceCode, QString & deviceID)
{
	
    Q_ASSERT(deviceCode.length() > 0);

    db_con_ptr con = GetDBConnection(DB_CON_READONLY);
    if (!con) {
        return false;
    }

    con->AcquireWrite();
    QSqlQuery query(*(con->pDB)); 
    //QString qstrSQL = QStringLiteral("SELECT ID FROM Rig_DeviceInfo WHERE Code = ?");
		QString qstrSQL = QStringLiteral("SELECT ID FROM Device WHERE Number = ?");

    if (!query.prepare(qstrSQL)) {
        LOG_WARING() << "SQLQuery prepare failed. error:" << con->pDB->lastError().text()
            << " sql:"  << qstrSQL;
        con->Release();
        return false;
    }

    query.bindValue(0, deviceCode);

    if (!query.exec()) {
        LOG_WARING() << "SQLQuery exec failed. error:" << con->pDB->lastError().text()
            << " sql:"  << qstrSQL;
        con->Release();
        return false;
    }

    bool ret = query.next();
    if (ret) {
        deviceID.clear();
        deviceID = query.value(0).toString();
    }

    con->Release();
    return ret;
}


//******************************************************
//** ������:   GetOilFieldList
//** ���ܼ���: ȡ������������Ϣ
//** �������: 
//   QList<T_TREE_NODE_INFO> lstResult:
//
//** ����ֵ: 
//   bool:
//
//** �������ڣ�2015/07/26
//** �����ˣ�  ���
//** �޸ļ�¼��
//   ����        �汾      �޸���    �޸�����
//
//** ����˵����
//******************************************************
bool CDBService::GetOilFieldList(QList<CBaseEntity> & lstResult)
{
	return true;
    db_con_ptr con = GetDBConnection(DB_CON_READONLY);
    if (!con) {
        return false;
    }

    con->AcquireWrite();
    QSqlQuery query(*(con->pDB));
    QString qstrSQL = QStringLiteral("SELECT id,name FROM Rig_OilFieldInfo");

    if (!query.prepare(qstrSQL)) {
        LOG_WARING() << "SQLQuery prepare failed. error:" << con->pDB->lastError().text()
            << " sql:"  << qstrSQL;
        con->Release();
        return false;
    }

    if (!query.exec()) {
        LOG_INFO() << "SQLQuery exec failed. error:" << con->pDB->lastError().text()
            << " sql:"  << qstrSQL;
        con->Release();
        return false;
    }

    bool ret = query.next();
    while (ret) {
        CBaseEntity listItem;
        listItem.id=query.value(0).toString();
        listItem.name=query.value(1).toString();
        lstResult.append(listItem);
        ret = query.next();
    }

    con->Release();
    return ret;
}

//******************************************************
//** ������:   GetRigListByOilFieldID
//** ���ܼ���: ͨ������IDȡ������������Ϣ
//** �������: 
//   const QString & oilFieldID:
//   QList<T_TREE_NODE_INFO> lstResult:
//
//** ����ֵ: 
//   bool:
//
//** �������ڣ�2015/07/26
//** �����ˣ�  ���
//** �޸ļ�¼��
//   ����        �汾      �޸���    �޸�����
//
//** ����˵����
//******************************************************
bool CDBService::GetRigListByOilFieldID(const QString & oilFieldID,QList<CBaseEntity> & lstResult)
{
	return true;
    Q_ASSERT(oilFieldID.length() > 0);

    db_con_ptr con = GetDBConnection(DB_CON_READONLY);
    if (!con) {
        return false;
    }

    con->AcquireWrite();
    QSqlQuery query(*(con->pDB));
    QString qstrSQL = QStringLiteral("SELECT id,name FROM Rig_RigBaseInfo WHERE OilFieldID = ?");

    if (!query.prepare(qstrSQL)) {
        LOG_WARING() << "SQLQuery prepare failed. error:" << con->pDB->lastError().text()
            << " sql:"  << qstrSQL;
        con->Release();
        return false;
    }

    query.bindValue(0, oilFieldID);
    if (!query.exec()) {
        LOG_WARING() << "SQLQuery exec failed. error:" << con->pDB->lastError().text()
            << " sql:"  << qstrSQL;
        con->Release();
        return false;
    }

    bool ret = query.next();
    while (ret) {
        CBaseEntity listItem;
        listItem.id=query.value(0).toString();
        listItem.name=query.value(1).toString();
        lstResult.append(listItem);
        ret = query.next();
    }

    con->Release();
    return ret;
}

//******************************************************
//** ������:   GetDeviceListByRigID
//** ���ܼ���: ͨ������IDȡ�������豸��Ϣ
//** �������: 
//   const QString & rigID:
//   QList<T_TREE_NODE_INFO> lstResult:
//
//** ����ֵ: 
//   bool:
//
//** �������ڣ�2015/07/26
//** �����ˣ�  ���
//** �޸ļ�¼��
//   ����        �汾      �޸���    �޸�����
//
//** ����˵����
//******************************************************
bool CDBService::GetDeviceListByRigID(const QString & rigID,QList<CDeviceInfo> & lstResult)
{
	return true;
    Q_ASSERT(rigID.length() > 0);

    db_con_ptr con = GetDBConnection(DB_CON_READONLY);
    if (!con) {
        return false;
    }

    con->AcquireWrite();
    QSqlQuery query(*(con->pDB));
    QString qstrSQL = QStringLiteral("SELECT a.id, a.code, a.name FROM dbo.Rig_DeviceInfo a INNER JOIN (SELECT ID FROM dbo.Rig_DeviceTypes WHERE Name LIKE '%H2S%') b ON a.DeviceType = b.ID WHERE a.RigID = ? ORDER BY a.DeviceType");

    if (!query.prepare(qstrSQL)) {
        LOG_WARING() << "SQLQuery prepare failed. error:" << con->pDB->lastError().text()
            << " sql:"  << qstrSQL;
        con->Release();
        return false;
    }

    query.bindValue(0, rigID);
    if (!query.exec()) {
        LOG_WARING() << "SQLQuery exec failed. error:" << con->pDB->lastError().text()
            << " sql:"  << qstrSQL;
        con->Release();
        return false;
    }

    bool ret = query.next();
    while (ret) {
        CDeviceInfo listItem;
        listItem.id = query.value(0).toString();
        listItem.code = query.value(1).toString().trimmed();
        listItem.name = query.value(2).toString().trimmed();
        lstResult.append(listItem);
        ret = query.next();
    }

    con->Release();
    return true;
}

//******************************************************
//** ������:   IsExistTable
//** ���ܼ���: �Ƿ���ڱ�
//** �������: 
//   const QString & tableName:
//
//** ����ֵ: 
//   bool:
//
//** �������ڣ�2015/07/26
//** �����ˣ�  ���
//** �޸ļ�¼��
//   ����        �汾      �޸���    �޸�����
//
//** ����˵����
//******************************************************
bool CDBService::IsExistTable(const QString & tableName)
{
    Q_ASSERT(tableName.length() > 0);

    {// �Ȳ��һ�����ı���
        QMutexLocker lock(m_pMutexForSet);
        QSet<QString>::iterator it = m_setTableName.find(tableName);
        if (it != m_setTableName.end()) {
            return true;
        }
    }

    db_con_ptr con = GetDBConnection(DB_CON_READWRITE);
    if (!con) {
        return false;
    }

    con->AcquireWrite();
    QSqlQuery query(*(con->pDB));
    QString qstrSQL = QStringLiteral("SELECT Name FROM SysObjects Where XType='U' AND Name = ? ORDER BY Name");

    if (!query.prepare(qstrSQL)) {
        LOG_INFO() << "SQLQuery prepare failed. error:" << con->pDB->lastError().text()
            << " sql:"  << qstrSQL;
        con->Release();
		con->CloseCon();
        return false;
    }

    query.bindValue(0, tableName);
    if (!query.exec()) {
        LOG_INFO() << "SQLQuery exec failed. error:" << con->pDB->lastError().text()
            << " sql:"  << qstrSQL;
        con->Release();
		con->CloseCon();
        return false;
    }

    bool ret = query.next();
    con->Release();

    if (ret) {// ���»�����ı�
        QMutexLocker lock(m_pMutexForSet);
        m_setTableName.insert(tableName);
    }

    return ret;
}

//******************************************************
//** ������:   CreateDataTable
//** ���ܼ���: �������ݱ�
//** �������: 
//   const QString & tableName:
//
//** ����ֵ: 
//   bool:
//
//** �������ڣ�2015/07/26
//** �����ˣ�  ���
//** �޸ļ�¼��
//   ����        �汾      �޸���    �޸�����
//
//** ����˵����
//******************************************************
bool CDBService::CreateDataTable(const QString & tableName, int flag )
{
    Q_ASSERT(tableName.length() > 0);
	 LOG_DEBUG() << QStringLiteral("��ʼ�������ݱ�0>>>>>>>>>>>>>>>>>>>>>");
    db_con_ptr con = GetDBConnection(DB_CON_READONLY);
    if (!con) {
        return false;
    }

    con->AcquireWrite();
    QSqlQuery query(*(con->pDB));
    //QString qstrSQLFormat(QStringLiteral("CREATE TABLE %1 (ID INT IDENTITY (1, 1) NOT NULL,DeviceID VARCHAR (50) NOT NULL,ChannelNo INT NOT NULL,DataType INT NOT NULL,TIMESTAMP BIGINT NOT NULL,Longitude DOUBLE PRECISION NOT NULL,Latitude DOUBLE PRECISION NOT NULL,Concentration DECIMAL(12,6) NOT NULL,MeasurementUnit INT NOT NULL)"));
    LOG_DEBUG() << QStringLiteral("��ʼ�������ݱ�1>>>>>>>>>>>>>>>>>>>>>");
	
	QString qstrSQLFormat;
#ifdef OLD_TYPE_DB
	if (0 == flag)
	{
		qstrSQLFormat = QStringLiteral("CREATE TABLE %1 (\
										 DeviceId uniqueidentifier NOT NULL,\
										 CompanyName nvarchar(50) NULL,\
										 Address nvarchar(255) NULL,\
										 Longitude DECIMAL(9,6) NOT NULL,\
										 Latitude DECIMAL(9,6) NOT NULL,\
										 SulfurDioxide BIGINT NOT NULL,\
										 NitricOxide BIGINT NOT NULL,\
										 CarbonMonoxide BIGINT NOT NULL,\
										 Hydrogen BIGINT NOT NULL,\
										 Oxygen DECIMAL(4,2) NOT NULL,\
										 CarbonDioxide DECIMAL(4,2) NOT NULL,\
										 CurrentTime datetime NOT NULL,\
										 BoilerId uniqueidentifier NULL)");
	}
	else
#endif
	{
		qstrSQLFormat = QStringLiteral("CREATE TABLE  %1 (\
										 DeviceId uniqueidentifier NOT NULL,\
										 CompanyName nvarchar(50) NULL,\
										 Address nvarchar(255) NULL,\
										 Longitude DECIMAL(9,6) NOT NULL,\
										 Latitude DECIMAL(9,6) NOT NULL,\
										 Data nvarchar(MAX) NOT NULL,\
										 CurrentTime datetime NOT NULL,\
										 BoilerId uniqueidentifier NULL)");
	}

	

    QString qstrSQL = qstrSQLFormat.arg(tableName);
    if (!query.prepare(qstrSQL)) {
        LOG_INFO() << "SQLQuery prepare failed. error:" << con->pDB->lastError().text()
            << " sql:"  << qstrSQL;
        con->Release();
		con->CloseCon();
        return false;
    }
    if (!query.exec()) {
        LOG_INFO() << "SQLQuery exec failed. error:" << con->pDB->lastError().text()
            << " sql:"  << qstrSQL;
        con->Release();
        return false;
    }

    bool ret = IsExistTable(tableName);

    con->Release();
    return ret;
}

//******************************************************
//** ������:   InsertDataTable
//** ���ܼ���: �����ݱ��в�������
//** �������: 
//   const QString & tableName:
//   const CMessageDeviceRealtimeData & deviceData:
//
//** ����ֵ: 
//   bool:
//
//** �������ڣ�2015/07/26
//** �����ˣ�  ���
//** �޸ļ�¼��
//   ����        �汾      �޸���    �޸�����
//
//** ����˵����
//******************************************************
bool CDBService::InsertDataTable(const QString & tableName, const QString deviceID, const REALTIME_DATA & deviceData)
{
//     Q_ASSERT(tableName.length() > 0);
// 
//     db_con_ptr con = GetDBConnection(DB_CON_READWRITE);
//     if (!con) {
//         return false;
//     }
// 
//     con->AcquireWrite();
//     QSqlQuery query(*(con->pDB));
//     QString qstrSQLFormat(QStringLiteral("INSERT INTO %1 ([DeviceId],[ChannelNo],[DataType],[TimeStamp],[Longitude],[Latitude],[Concentration],[MeasurementUnit]) VALUES (?,?,?,?,?,?,?,?)"));
//     QString qstrSQL = qstrSQLFormat.arg(tableName);
// 
//     if (!query.prepare(qstrSQL)) {
//         LOG_INFO() << "SQLQuery prepare failed. error:" << con->pDB->lastError().text()
//             << " sql:"  << qstrSQL;
//         con->Release();
//         return false;
//     }
// 
//     // ��������
//     int gasType = deviceData.m_struct.data_[5];
// 
//     // ����Ũ��
//     double concentration = 0.0;
//     {
//         U8 buf[8];
//         U8 * pData = (U8 *)&(deviceData.m_struct.data_);
//         memcpy(buf, pData, 7);
// 
//         concentration = Convert7ByteDataToD64(buf);
//     }
// 
//     // ������λ
//     int unitOfMeasurement = deviceData.m_struct.data_[6];
// 
//     query.bindValue(0, deviceID);
//     query.bindValue(1, deviceData.m_struct.channelNo);
//     query.bindValue(2, gasType);
//     query.bindValue(3, deviceData.m_struct.timestamp);
//     query.bindValue(4, deviceData.m_struct.longitude);
//     query.bindValue(5, deviceData.m_struct.latitude);
//     query.bindValue(6, concentration);    // ����Ũ��
//     query.bindValue(7, unitOfMeasurement);// ������λ
// 
//     bool ret = query.exec();
//     if (!ret) {
//         LOG_INFO() << "SQLQuery exec failed. error:" << con->pDB->lastError().text()
//             << " sql:"  << qstrSQL;
//         con->Release();
//         return false;
//     }
// 
//     con->Release();
//     return ret;
	return false;
}

//******************************************************
//** ������:   InsertDataTable
//** ���ܼ���: 
//** �������: 
//   const QString & tableName:
//   QList<T_DEVICE_DATA> & lst:
//
//** ����ֵ: 
//   bool:
//
//** �������ڣ�2015/08/12
//** �����ˣ�  ����
//** �޸ļ�¼��
//   ����        �汾      �޸���    �޸�����
//
//** ����˵����
//******************************************************

//int to string  
string inttostring(int in)  
{  
    stringstream ss;  
    string str;  
    ss << in;  
    ss >> str;  
    return str;  
} 

//void testdatetime()
//{
//	//get the current time  
//	CTime t = CTime::GetCurrentTime();//#include <atltime.h>  
//	string mytime = inttostring(t.GetYear()) + "-" + inttostring(t.GetMonth()) + "-" + inttostring(t.GetDay())  
//		+ " " + inttostring(t.GetHour()) + ":" + inttostring(t.GetMinute()) + ":" + inttostring(t.GetSecond());  
//	//cout << mytime << endl;  
//  
//	char sdate[30];  
//	strcpy_s(sdate, mytime.c_str());  
//  
//	char mysql[] = "insert into tb_report(source_id,created_time) values(1,'%s')";  
//	char mysqlBuf[1024];  
//	sprintf_s(mysqlBuf, mysql, sdate);  
//	res = mysql_query(&myCont, mysqlBuf); 
//}

string string_To_UTF8(const string & str)  
{  
	int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);  
  
	wchar_t * pwBuf = new wchar_t[nwLen + 1];//һ��Ҫ��1����Ȼ�����β��  
	ZeroMemory(pwBuf, nwLen * 2 + 2);  
  
	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);  
  
	int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);  
  
	char * pBuf = new char[nLen + 1];  
	ZeroMemory(pBuf, nLen + 1);  
  
	::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);  
  
	string retStr(pBuf);  
  
	delete []pwBuf;  
	delete []pBuf;  
  
	pwBuf = NULL;  
	pBuf = NULL;  
  
	return retStr;  
}  

bool CDBService::InsertDataTable(const QString & tableName, T_DEVICE_DATA&  lst)
{
	if (lst == nullptr) {
		return true;
	}
	Q_ASSERT(lst != nullptr);

	QVariantList lstDeviceID;
	QVariantList lstCompanyName;
	QVariantList lstAddress;
	QVariantList lstLongitude;
	QVariantList lstLatitude;
	QVariantList lstTimestamp;
	QVariantList lstBoilerId;

	QVariantList SulfurDioxide;//��������
	QVariantList NitricOxide;//һ������
	QVariantList CarbonMonoxide;//һ����̼
	QVariantList NitrogenHioxide;//��������
	QVariantList Oxygen;//����
	QVariantList CarbonDioxide;//������̼

	double concentration;
	char sz[12] = { 0 };
	UINT16 maskOfData = 0;
	bool ret = false;
	bool  selectflag = false; //��ʾ��ѯ���޴�
	LOG_DEBUG() << QStringLiteral("��ʼ��������>>>>>>>>>>>>>>>>>>>>>");

	//˵��  ��Ȼ������List ʵ�� ֻ��һ��ֵ, ��Ϊ�˲���ԭ���Ĵ���
	T_DEVICE_DATA& data = lst;

	{
	QString CompanyNameS;// = QString::fromLocal8Bit("����ơ�Ƴ�");
	QString AddressS;// = QString::fromLocal8Bit("����·1198��");


					 //get the current time  
	CTime t = CTime::GetCurrentTime();
	string mytime = inttostring(t.GetYear()) + "-" + inttostring(t.GetMonth()) + "-" + inttostring(t.GetDay())
		+ " " + inttostring(t.GetHour()) + ":" + inttostring(t.GetMinute()) + ":" + inttostring(t.GetSecond());

	lstTimestamp << mytime.c_str();

	QString strDeviceID = data->qstrDeviceID;
	lstDeviceID << strDeviceID;

	if (m_deviceMsgMap.contains(strDeviceID))
	{
		lstCompanyName << m_deviceMsgMap[strDeviceID].m_companyName;
		lstLongitude << m_deviceMsgMap[strDeviceID].m_longitude;
		lstLatitude << m_deviceMsgMap[strDeviceID].m_latitude;
		lstBoilerId << m_deviceMsgMap[strDeviceID].m_boilerId;
		lstAddress << m_deviceMsgMap[strDeviceID].m_address;
		LOG_DEBUG() << QStringLiteral("�豸[%1]��Ϣ�ѻ���,ֱ��ʹ��!").arg(strDeviceID);
	}
	else
	{
		db_con_ptr conDef = GetDBConnection(DB_CON_READONLY);
		if (!conDef) {
			return false;
		}

		conDef->AcquireWrite();
		QSqlQuery queryDef(*(conDef->pDB));
		//QString qstrSQL = QStringLiteral("SELECT ID FROM Rig_DeviceInfo WHERE Code = ?");
		QString qstrSQL = QStringLiteral("SELECT Name,Longitude,Latitude,BoilerId,AreaName FROM Device WHERE ID = ?");

		if (!queryDef.prepare(qstrSQL)) {
			LOG_WARING() << "SQLQuery prepare failed. error:" << conDef->pDB->lastError().text()
				<< " sql:" << qstrSQL;
			conDef->Release();
			return false;
		}

		queryDef.bindValue(0, strDeviceID);
		LOG_WARING() << "db_service.cpp 1364###" << strDeviceID;
		if (!queryDef.exec()) {
			LOG_WARING() << "SQLQuery exec failed. error:" << conDef->pDB->lastError().text()
				<< " sql:" << qstrSQL;
			conDef->Release();
			return false;
		}



		bool ret = queryDef.next();
		if (!ret) {
			LOG_WARING() << "SQLQuery exec failed. error:" << conDef->pDB->lastError().text()
				<< " sql:" << qstrSQL;
			conDef->Release();
			return false;
		}
		lstCompanyName << queryDef.value(0).toString();
		lstLongitude << queryDef.value(1).toString();
		lstLatitude << queryDef.value(2).toString();
		lstBoilerId << queryDef.value(3).toString();
		lstAddress << queryDef.value(4).toString();

		m_deviceMsgMap[strDeviceID].m_companyName = queryDef.value(0).toString();
		m_deviceMsgMap[strDeviceID].m_longitude = queryDef.value(1).toString();
		m_deviceMsgMap[strDeviceID].m_latitude = queryDef.value(2).toString();
		m_deviceMsgMap[strDeviceID].m_boilerId = queryDef.value(3).toString();
		m_deviceMsgMap[strDeviceID].m_address = queryDef.value(4).toString();
		LOG_DEBUG() << QStringLiteral("�豸��һ�ε�½,�ɹ���ȡ�豸��Ϣ������!");

		conDef->Release();
	}


}

	int flag; //��ʶ 0 �ɰ� 1�°�
	flag = data->m_flag;
	QString qstrSQLFormat;
#ifdef OLD_TYPE_DB
	if (OLD_TYPE== flag) //�ɰ�
	{
		//                                                    �豸ID        ��˾��        ��ַ       ����        γ��       ��������      һ������       һ����̼        ��������   ����       ������̼      ʱ��           ��¯ID
		qstrSQLFormat = QStringLiteral("INSERT INTO %1 ([DeviceId],[CompanyName],[Address],[Longitude],[Latitude],[SulfurDioxide],[NitricOxide],[CarbonMonoxide],[Hydrogen],[Oxygen],[CarbonDioxide],[CurrentTime],[BoilerId]) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?)");
	}

	else if(NEW_TYPE == flag)// �°�
#endif
		qstrSQLFormat = QStringLiteral("INSERT INTO %1  VALUES (?,?,?,?,?,?,?,?)");


	Q_ASSERT(m_deviceMsgMap.contains(data->qstrDeviceID));
	
	
	// ������������
	db_con_ptr con = GetDBConnection(DB_CON_READWRITE);
	if (!con) {
		return false;
	}

	con->AcquireWrite();
	QSqlQuery query(*(con->pDB));

	
	QString qstrSQL = qstrSQLFormat.arg(tableName);

	if (!query.prepare(qstrSQL)) {
		LOG_INFO() << "SQLQuery prepare failed. error:" << con->pDB->lastError().text()
			<< " sql:" << qstrSQL;
		con->Release();
		con->CloseCon();
		return false;
	}


	//������������
	if(OLD_TYPE== flag)
	{
		QVariantList listDeviceData;
		QString strData; //����ÿ���������ɵ�json�ַ���
						 //����ÿ������ �е����ݱ�

		QJsonObject json;
		// 		json.insert("name", QString("Qt"));
		// 		json.insert("version", 5);
		// 		json.insert("windows", true);

		QJsonDocument document;
		double doubleO2 = -1;
		for (message_device_realtime_data_ptr realData : data->m_vectorData) {
			if (realData->m_struct.channelNo != MAMOS_ID)
			{
				U8 buf[8];
				U8 * pData = (U8 *)&(realData->m_struct.data_);
				memcpy(buf, pData, 7);

				concentration = Convert7ByteDataToD64(buf);
				//����С��0�������ڷ���
				if (concentration < 0)
				{
					concentration = 0;
				}
				//����:��Э���ϴ����� ����0x8000 ����
				if (realData->m_struct.channelNo == MAMOS_CO2)
				{
					if (fabs(concentration - 327.68) < 0.000001 || fabs(concentration - 0x8000) < 0.000001)
					{
						concentration = 0;
					}
				}
				/*	����:20180404
				author:xiezhao
				�޸�:ȫ�мҾ� ��װCO2����
				*/
				//1.��ȡ����ֵ
				if ("C595900C-687D-909D-B7BA-64A7A0032074" == data->qstrDeviceID
					&&realData->m_struct.channelNo == MAMOS_O2)	//
				{
					doubleO2 = concentration;
				}
				//2.20-����ֵ Ϊco2ֵ ���ж� С�� 1 Ϊ0
				if ("C595900C-687D-909D-B7BA-64A7A0032074" == data->qstrDeviceID
					&&realData->m_struct.channelNo == MAMOS_CO2 && doubleO2 >0)	//
				{
					concentration = 20 - doubleO2 ;
					concentration = concentration < 1 ? 0 : concentration;
				}
			}

#ifdef OLD_TYPE_DB
			switch (realData->m_struct.channelNo)
			{

			case MAMOS_O2:
			{

				Oxygen << concentration;
				LOG_DEBUG() << QStringLiteral("����(%)��%1��").arg(concentration);

				maskOfData += MAMOS_O2;
			}

			break;
			case MAMOS_CO:
				if (concentration > 60000)
				{
					concentration = 60000;
				}
				CarbonMonoxide << concentration;
				LOG_DEBUG() << QStringLiteral("һ����̼(ppm)��%1��").arg(concentration);

				maskOfData += MAMOS_CO;
				break;
			case MAMOS_NO:
				if (concentration > 60000)
				{
					concentration = 60000;
				}
				NitricOxide << concentration;
				LOG_DEBUG() << QStringLiteral("һ������(ppm)��%1��").arg(concentration);

				maskOfData += MAMOS_NO;
				break;
			case MAMOS_SO2:
				if (concentration > 60000)
				{
					concentration = 60000;
				}
				SulfurDioxide << concentration;
				LOG_DEBUG() << QStringLiteral("��������(ppm)��%1��").arg(concentration);
				maskOfData += MAMOS_SO2;
				break;
			case MAMOS_NO2:
				if (concentration > 60000)
				{
					concentration = 60000;
				}
				NitrogenHioxide << concentration;
				LOG_DEBUG() << QStringLiteral("��������(ppm)��%1��").arg(concentration);
				maskOfData += MAMOS_NO2;
				break;
			case MAMOS_CO2:
				if (concentration > 99.99)
				{
					concentration = 99.99;
				}
				CarbonDioxide << concentration;
				LOG_DEBUG() << QStringLiteral("������̼(%)��%1��").arg(concentration);
				maskOfData += MAMOS_CO2;
				break;
			default:
				break;
			}
		}
		query.addBindValue(lstDeviceID);
		query.addBindValue(lstCompanyName);
		query.addBindValue(lstAddress);
		query.addBindValue(lstLongitude);
		query.addBindValue(lstLatitude);
		query.addBindValue(SulfurDioxide);//��������
		query.addBindValue(NitricOxide);//һ������
		query.addBindValue(CarbonMonoxide);//һ����̼
		query.addBindValue(NitrogenHioxide);//��������
		query.addBindValue(Oxygen);//����
		query.addBindValue(CarbonDioxide);//������̼
		query.addBindValue(lstTimestamp);
		query.addBindValue(lstBoilerId);

		ret = query.execBatch();
		if (!ret) {
			LOG_INFO() << "SQLQuery execBatch failed. error:" << con->pDB->lastError().text()
				<< " sql:" << qstrSQL;
			con->Release();
			return false;
		}
#endif
			/*	����:20180404
				author:xiezhao
				�޸�:ȫ�мҾ� �ϴ������� NO2 �ĳ�NOX
				*/
			if ("C595900C-687D-909D-B7BA-64A7A0032074" == data->qstrDeviceID 
				&&realData->m_struct.channelNo == MAMOS_NO2)	//V_PPM_NOx
			{
				json.insert("V_PPM_NOx", concentration);
			}
			else
				json.insert(gMamosKey[realData->m_struct.channelNo], concentration);
		
		}
		document.setObject(json);
		QByteArray byte_array = document.toJson(QJsonDocument::Compact);
		QString json_str(byte_array);
		listDeviceData << json_str;

		{
			LOG_DEBUG() << QStringLiteral("��Э��_�����������,��ʼ���>>>>>>>>>>>>>>>>>>>>>");

			query.addBindValue(lstDeviceID);
			query.addBindValue(lstCompanyName);
			query.addBindValue(lstAddress);
			query.addBindValue(lstLongitude);
			query.addBindValue(lstLatitude);
			query.addBindValue(listDeviceData);
			query.addBindValue(lstTimestamp);
			query.addBindValue(lstBoilerId);

			ret = query.execBatch();
			if (!ret) {
				LOG_INFO() << "SQLQuery execBatch failed. error:" << con->pDB->lastError().text()
					<< " sql:" << qstrSQL;
				con->Release();
				con->CloseCon();
				return false;
			}
		}
		
	}
	else if(NEW_TYPE == flag)
	{
		QVariantList listDeviceData;
		QString strData; //����ÿ���������ɵ�json�ַ���
						 //����ÿ������ �е����ݱ�

		QJsonObject json;
		// 		json.insert("name", QString("Qt"));
		// 		json.insert("version", 5);
		// 		json.insert("windows", true);

		QJsonDocument document;
		// 		document.setObject(json);
		// 		QByteArray byte_array = document.toJson(QJsonDocument::Compact);
		// 		QString json_str(byte_array);

// 		for (message_device_realtime_data_ptr realData : data.m_vectorData)
// 		{
// 			//������Ч ���������Ѷ��� 
// 			if (realData->m_struct.channelNo < MAMOS_ID)
// 			{
// 				U8 buf[8];
// 				U8 * pData = (U8 *)&(realData->m_struct.data_);
// 				memcpy(buf, pData, 7);
// 
// 				concentration = Convert7ByteDataToD64(buf);
// 				//����С��0�������ڷ���
// 				if (concentration < 0)
// 				{
// 					concentration = 0;
// 				}
// 				//һЩ�������� ���� ��Щ���⴦��
// 
// 
// 				if (realData->m_struct.channelNo == MAMOS_O2)
// 				{
// 					//lstBoilerId << "dfeff938-d00d-4ee8-aad3-39d21a637665";
// 					/*
// 					* date:20170918
// 					* data:������������20.0%������
// 					*/
// 					if (concentration >= 20.0)
// 					{
// 						LOG_INFO() << QStringLiteral("##��������Ϊ[%1]>=20.0%,����!##").arg(concentration);
// 						con->Release();
// 						return true;
// 					}
// 
// 					LOG_DEBUG() << QStringLiteral("����(%)��%1��").arg(concentration);
// 				}
// 
// 				//д�� json����
// 				json.insert(gMamosKey[realData->m_struct.channelNo], concentration);
// 
// 
// 			}
// 
// 		}
		//һ������д��,��¼��list

		for (std::shared_ptr<NEW_DEVICE_DATA> realData : data->m_vectorDataNew)
		{
			json.insert(realData->m_strFlag, realData->m_data);
		}

		document.setObject(json);
		QByteArray byte_array = document.toJson(QJsonDocument::Compact);
		QString json_str(byte_array);
		listDeviceData << json_str;

		{
			LOG_DEBUG() << QStringLiteral("�����������,��ʼ���>>>>>>>>>>>>>>>>>>>>>");

			query.addBindValue(lstDeviceID);
			query.addBindValue(lstCompanyName);
			query.addBindValue(lstAddress);
			query.addBindValue(lstLongitude);
			query.addBindValue(lstLatitude);
			query.addBindValue(listDeviceData);
			query.addBindValue(lstTimestamp);
			query.addBindValue(lstBoilerId);

			ret = query.execBatch();
			if (!ret) {
				LOG_INFO() << "SQLQuery execBatch failed. error:" << con->pDB->lastError().text()
					<< " sql:" << qstrSQL;
				con->Release();
				con->CloseCon();
				return false;
			}
		}
	}
		
    con->Release();
    return true;
}

//******************************************************
//** ������:   DataQueuePushBack
//** ���ܼ���: ���ݴ����жӼ�������
//** �������: 
//   const QString data:
//   const CMessageDeviceRealtimeData & deviceData:
//
//** ����ֵ: 
//   bool:
//
//** �������ڣ�2015/07/26
//** �����ˣ�  ���
//** �޸ļ�¼��
//   ����        �汾      �޸���    �޸�����
//
//** ����˵����
//******************************************************
bool CDBService::DataQueuePushBack(const QString & deviceID, 
                                   const REALTIME_DATA& data, int flag)
{
    Q_ASSERT(!deviceID.isEmpty());

    m_dataQueue.Push(std::make_shared<__T_DEVICE_DATA>(deviceID, data, flag));
    return true;
}

bool CDBService::DataQueuePushBack(const QString & deviceID,
	const REALTIME_DATA_NEW& data, int flag)
{
	Q_ASSERT(!deviceID.isEmpty());

	m_dataQueue.Push(std::make_shared<__T_DEVICE_DATA>(deviceID, data, flag));
	return true;
}

//******************************************************
//** ������:   DataQueuePopFront
//** ���ܼ���: ���ݴ����ж��ų�����
//** �������: 
//   const QString data:
//   const CMessageDeviceRealtimeData & deviceData:
//
//** ����ֵ: 
//   bool:
//
//** �������ڣ�2015/07/26
//** �����ˣ�  ���
//** �޸ļ�¼��
//   ����        �汾      �޸���    �޸�����
//
//** ����˵����
//******************************************************
T_DEVICE_DATA& CDBService::DataQueuePopFront()
{
    return m_dataQueue.Take();
}

//******************************************************
//** ������:   SetDeviceParam
//** ���ܼ���: �����豸���� 
//** �������: 
//   const QString & deviceID:
//   CMessageSetDeviceParam & deviceParam:
//
//** ����ֵ: 
//   bool:
//
//** �������ڣ�2015/08/05
//** �����ˣ�  ����
//** �޸ļ�¼��
//   ����        �汾      �޸���    �޸�����
//
//** ����˵����
//******************************************************
bool CDBService::SetDeviceParam(E_DEVICE_TYPE deviceType, 
                                const QString & deviceID, 
                                CMessageSetDeviceParam & deviceParam)
{
    db_con_ptr con = GetDBConnection(DB_CON_READONLY);
    if (!con) {
        return false;
    }

    con->AcquireWrite();

    // �����豸����
    QString qstrSQl;
    if (DEVICE_TYPE_PORTABLE_H2S_DETECTOR == deviceType) {
        qstrSQl = QStringLiteral("UPDATE DEV_PortableH2SDetector SET WarningLine1st = ?, WarningLine2nd = ?, MaxRange = ?, SampleRate = ? WHERE DeviceID = ?");
    } else if (DEVICE_TYPE_STATIC_H2S_DETECTOR == deviceType) {
        qstrSQl = QStringLiteral("UPDATE DEV_StaticH2SDetector SET WarningLine1st = ?, WarningLine2nd = ?, MaxRange = ?, SampleRate = ? WHERE DeviceID = ?");
    } else {
        con->Release();
        return false;
    }

    QSqlQuery query(*(con->pDB));
    if (!query.prepare(qstrSQl)) {
        LOG_WARING() << QStringLiteral("query prepare failed. error[%1], SQL[%2]")
            .arg(con->pDB->lastError().text()).arg(qstrSQl);
        con->Release();
        return false;
    }

    query.bindValue(0, QString::fromLocal8Bit((char *)(deviceParam.m_struct.warningLine1st), sizeof(deviceParam.m_struct.warningLine1st)).trimmed());
    query.bindValue(1, QString::fromLocal8Bit((char *)(deviceParam.m_struct.warningLine2nd), sizeof(deviceParam.m_struct.warningLine2nd)).trimmed());
    query.bindValue(2, QString::fromLocal8Bit((char *)(deviceParam.m_struct.maxRange), sizeof(deviceParam.m_struct.maxRange)).trimmed());
    query.bindValue(3, deviceParam.GetSampleRate());
    query.bindValue(4, deviceID);
    bool ret = query.exec();
    if (!ret) {
        LOG_WARING() << QStringLiteral("query exec failed. error[%1], SQL[%2]")
            .arg(con->pDB->lastError().text()).arg(qstrSQl);
    }
    con->Release();

    return ret;
}

//******************************************************
//** ������:   SetDeviceName
//** ���ܼ���: �����豸��
//** �������: 
//   const QString & deviceID:�豸ID
//   const QString & deviceName:�豸��
//
//** ����ֵ: 
//   bool:
//
//** �������ڣ�2015/09/18
//** �����ˣ�  ����
//** �޸ļ�¼��
//   ����        �汾      �޸���    �޸�����
//
//** ����˵����
//******************************************************
bool CDBService::SetDeviceName(const QString & deviceID, const QString & deviceName)
{
	return true;
    db_con_ptr con = GetDBConnection(DB_CON_READONLY);
    if (!con) {
        return false;
    }

    con->AcquireWrite();

    // �����豸��
    QString qstrSQl("UPDATE Rig_DeviceInfo SET Name = ? WHERE ID = ?");

    QSqlQuery query(*(con->pDB));
    if (!query.prepare(qstrSQl)) {
        LOG_WARING() << QStringLiteral("query prepare failed. error[%1], SQL[%2]")
            .arg(con->pDB->lastError().text()).arg(qstrSQl);
        con->Release();
        return false;
    }

    query.bindValue(0, deviceName);
    query.bindValue(1, deviceID);
    bool ret = query.exec();
    if (!ret) {
        LOG_WARING() << QStringLiteral("query exec failed. error[%1], SQL[%2]")
            .arg(con->pDB->lastError().text()).arg(qstrSQl);
    }

    con->Release();
    return ret;
}

T_DEVICE_DATA CDBService::DataQueueFront()
{
    return m_dataQueue.Front();
}


//******************************************************
//** ������:   GetDeviceHistoryDataCount
//** ���ܼ���: ��ȡ�豸��ʷ��������
//** �������: 
//   const quint64 & begin:
//   const quint64 & end:
//   QVector<QString> deviceIDs:
//   int & count:[����]��ʷ��������
//
//** ����ֵ: 
//   bool:
//
//** �������ڣ�2016/1/4
//** �����ˣ�  ����
//** �޸ļ�¼��
//   ����        �汾      �޸���    �޸�����
//
//** ����˵����
//******************************************************
bool CDBService::GetDeviceHistoryDataCount(const quint64 & begin, 
                                           const quint64 & end, 
                                           QVector<QString> deviceIDs,
                                           int & count)
{
    QString qstrTableName = QDateTime::fromMSecsSinceEpoch(begin).date().toString("DeviceData_yyyyMMdd");
    if (!IsExistTable(qstrTableName)) {// ������ָ�����ڵ����ݱ�ֱ�ӷ���0
        count = 0;
        return true;
    }

    db_con_ptr con = GetDBConnection(DB_CON_READONLY);
    if (!con) {
        return false;
    }

    con->AcquireRead();
    
    QString qstrFormat = "SELECT COUNT(*) FROM %1 WHERE TIMESTAMP >= %2 AND TIMESTAMP <= %3 AND [DeviceID] IN (%4)";
    QString qstrValueSet;

    for (int i = 0; i < deviceIDs.count(); i++)
    {
        if (i != 0)
        {
            qstrValueSet.append(", ");
        }
        qstrValueSet.append(QString("'%1'").arg(deviceIDs.at(i)));
    }

    QString qstrSQL = qstrFormat.arg(qstrTableName).arg(begin).arg(end).arg(qstrValueSet);
    
    QSqlQuery query(*(con->pDB));
    if (!query.prepare(qstrSQL)) {
        LOG_WARING() << QStringLiteral("query prepare failed. error[%1], SQL[%2]")
            .arg(con->pDB->lastError().text()).arg(qstrSQL);
        con->Release();
        return false;
    }

    if (!query.exec()) {
        LOG_INFO() << "SQLQuery exec failed. error:" << con->pDB->lastError().text()
            << " sql:"  << qstrSQL;
        con->Release();
        return false;
    }

    bool ret = query.next();
    if (ret) {
        count = query.value(0).toInt();
    }

    con->Release();

    return true;
}


//******************************************************
//** ������:   GetDeviceHistoryData
//** ���ܼ���: ��ȡ�豸��ʷ��¼
//** �������: 
//   const quint64 & begin:
//   const quint64 & end:
//   int pageIndex:
//   int itemsInPerPage:
//   QVector<QString> deviceIDs:
//   QList<CMessageDeviceDataItem *> & lst:[����]��ȡ������ʷ��������
//
//** ����ֵ: 
//   bool:
//
//** �������ڣ�2016/1/4
//** �����ˣ�  ����
//** �޸ļ�¼��
//   ����        �汾      �޸���    �޸�����
//
//** ����˵����
//******************************************************
bool CDBService::GetDeviceHistoryData(const quint64 & begin, const quint64 & end, 
                                      int pageIndex, int rowsInPerPage, 
                                      QVector<QString> deviceIDs, 
                                      QList<CMessageDeviceDataItem *> & lst)
{
    db_con_ptr con = GetDBConnection(DB_CON_READONLY);
    if (!con) {
        return false;
    }

    con->AcquireWrite();

    QString qstrFormat = "SELECT DeviceID, ChannelNo, DataType, TIMESTAMP, Longitude, Latitude, Concentration, MeasurementUnit "
        "FROM (SELECT ROW_NUMBER() OVER(ORDER BY TIMESTAMP ASC) AS rowNum, * FROM %1 WHERE TIMESTAMP > %2 AND TIMESTAMP <= %3 AND [DeviceID] IN (%5)) AS T "
        "WHERE rowNum > %6 AND rowNum <= %7";
    QString qstrTableName = QDateTime::fromMSecsSinceEpoch(begin).date().toString("DeviceData_yyyyMMdd");
    int startRow = (pageIndex - 1) * rowsInPerPage;
    int endRow = pageIndex * rowsInPerPage;
    QString qstrValueSet;

    for (int i = 0; i < deviceIDs.count(); i++)
    {
        if (i != 0)
        {
            qstrValueSet.append(", ");
        }
        qstrValueSet.append(QString("'%1'").arg(deviceIDs.at(i)));
    }

    QString qstrSQL = qstrFormat.arg(qstrTableName).arg(begin).arg(end).arg(qstrValueSet).arg(startRow).arg(endRow);

    QSqlQuery query(*(con->pDB));
    if (!query.prepare(qstrSQL)) {
        LOG_WARING() << QStringLiteral("query prepare failed. error[%1], SQL[%2]")
            .arg(con->pDB->lastError().text()).arg(qstrSQL);
        con->Release();
        return false;
    }

    if (!query.exec()) {
        LOG_INFO() << "SQLQuery exec failed. error:" << con->pDB->lastError().text()
            << " sql:"  << qstrSQL;
        con->Release();
        return false;
    }

    bool ret = query.next();
    while (ret) {
        CMessageDeviceDataItem * item = new CMessageDeviceDataItem();
        item->SetDeviceID(query.value(0).toString());
        item->m_deviceData.m_struct.channelNo = query.value(1).toInt();
        item->m_deviceData.m_struct.datatype = query.value(2).toInt();
        item->m_deviceData.m_struct.timestamp = query.value(3).toLongLong();
        item->m_deviceData.m_struct.longitude = query.value(4).toDouble();
        item->m_deviceData.m_struct.latitude = query.value(5).toDouble();
        item->m_struct.data = query.value(6).toDouble();
        lst.push_back(item);
        ret = query.next();
    }

    con->Release();

    return true;
}


//******************************************************
//** ������:   run
//** ���ܼ���: ���ݴ����жӴ����߳�
//** �������: 
//
//** ����ֵ: 
//   void:
//
//** �������ڣ�2015/07/26
//** �����ˣ�  ���
//** �޸ļ�¼��
//   ����        �汾      �޸���    �޸�����
	// 20.95%

//** ����˵����
//******************************************************

//MAMOS_O2 = 0,	
//MAMOS_CO,			// 12ppm
//MAMOS_NO,			// 24ppm
//MAMOS_SO2,			// 56ppm
//MAMOS_NO2,			// 888ppm
//MAMOS_CO2,			// 23.13%

//�ɰ� ����
// void CDBHandleTask::run()
// {
//     QString qstrFormat(QStringLiteral("%1##%2"));
//     QString taskID = qstrFormat.arg(QDateTime::currentDateTime().toString("dd#HH_mm_ss__zzz"))
//         .arg((int)QThread::currentThreadId());
//     LOG_DEBUG() << "CDBHandleTask " << taskID << "start";
// 
// 
// 	QMap<QString, QList<T_DEVICE_DATA>*>	m_device_map;
//     while (!CDBService::GetInstance()->IsStopSingalTriggered())
//     {
// 		
//       
// 		
//         while (!CDBService::GetInstance()->m_dataQueue.Empty()) {
// 
// 			//2017/6/14 ����һ�� �������� ����
// 			T_DEVICE_DATA fontData = CDBService::GetInstance()->DataQueueFront();
// 				if (!m_device_map.contains(fontData.qstrDeviceID) //�������豸��map
// 					)
// 				{ 
// 					if (fontData.deviceData.m_struct.channelNo == MAMOS_O2) //���豸����ʼ����
// 					{
// 						m_device_map[fontData.qstrDeviceID] = new QList<T_DEVICE_DATA>;
// 						m_device_map[fontData.qstrDeviceID]->push_back(fontData);
// 					}
// 				}
// 				else //�Ѿ����� ���豸������map
// 				{
// 					int size = m_device_map[fontData.qstrDeviceID]->size();
// 					if (fontData.deviceData.m_struct.channelNo == size) //�����ݰ� �պ��Ǹ��豸�� ���� ���� ������
// 					{
// 						m_device_map[fontData.qstrDeviceID]->push_back(fontData);
// 					}
// 					else
// 					{
// 						//С��size
// 						if (fontData.deviceData.m_struct.channelNo == MAMOS_O2) //����һ�����豸��ͷ�����ݰ�
// 						{
// 							m_device_map[fontData.qstrDeviceID]->clear(); //��� ���¼���
// 							m_device_map[fontData.qstrDeviceID]->push_back(fontData); 
// 						}
// 						//����size
// 					}
// 					//m_device_map
// 				}
// 				CDBService::GetInstance()->DataQueuePopFront();//���� ������
// 
// 				if (m_device_map[fontData.qstrDeviceID]->size() < MAMOS_ID) //С����ϰ���С
// 				{
// 					continue;
// 				}
// 
// 
// 
// 
// 				QList<T_DEVICE_DATA> lstBatchData =*(m_device_map[fontData.qstrDeviceID]);
// 				m_device_map[fontData.qstrDeviceID]->clear();
// 
// 				QString batchTableName = (QStringLiteral("DeviceData_%1")).
// 					arg(QDateTime::currentDateTime().toString("yyyyMMdd"));
// /*
// 				for (int i = 0;
// 					!CDBService::GetInstance()->m_dataQueue.Empty() && i < batchSize;
// 					i++) {
// 					T_DEVICE_DATA deviceData = CDBService::GetInstance()->DataQueueFront();
// 					
// 					if (deviceData.deviceData.m_struct.channelNo != i)
// 					{
// 						CDBService::GetInstance()->DataQueuePopFront();
// 						break;
// 					}
// 					
// 					QString todayTableName = QStringLiteral("DeviceData_%1")
// 						.arg(QDateTime::fromMSecsSinceEpoch(deviceData.deviceData.m_struct.timestamp)
// 							.toString("yyyyMMdd"));
// 					if (todayTableName == batchTableName) {
// 						deviceData = CDBService::GetInstance()->DataQueuePopFront();
// 						lstBatchData.push_back(deviceData);
// 					}
// 					else {
// 						break;// ��������������Ҫ����д
// 					}
// 				}
// */
// 				if (lstBatchData.length() > 0) {
// 					bool ret = CDBService::GetInstance()->IsExistTable(batchTableName);
// 					if (!ret) {
// 						ret = CDBService::GetInstance()->CreateDataTable(batchTableName);
// 						if (!ret) {
// 							break;// ����ʧ��
// 						}
// 					}
// 
// 					// ������������
// 					ret = CDBService::GetInstance()->InsertDataTable(batchTableName,
// 						lstBatchData);
// 					if (!ret) {// ��������ʧ��
// 
// 					}
// 				}
// 
// 			
// 		
// 
// #ifdef NOTEST
// 			QList<T_DEVICE_DATA> lstBatchData;
// 			QString batchTableName = (QStringLiteral("DeviceData_%1")).
// 				arg(QDateTime::currentDateTime().toString("yyyyMMdd"));
// 
// 			for (int i = 0;
// 				!CDBService::GetInstance()->m_dataQueue.Empty() && i < batchSize;
// 				i++) {
// 				T_DEVICE_DATA deviceData = CDBService::GetInstance()->DataQueueFront();
// 				QString todayTableName = QStringLiteral("DeviceData_%1")
// 					.arg(QDateTime::fromMSecsSinceEpoch(deviceData.deviceData.m_struct.timestamp)
// 						.toString("yyyyMMdd"));
// 				if (todayTableName == batchTableName) {
// 					deviceData = CDBService::GetInstance()->DataQueuePopFront();
// 					lstBatchData.push_back(deviceData);
// 				}
// 				else {
// 					break;// ��������������Ҫ����д
// 				}
// 			}
// 
// 			if (lstBatchData.length() > 0) {
// 				bool ret = CDBService::GetInstance()->IsExistTable(batchTableName);
// 				if (!ret) {
// 					ret = CDBService::GetInstance()->CreateDataTable(batchTableName);
// 					if (!ret) {
// 						break;// ����ʧ��
// 					}
// 				}
// 
// 				// ������������
// 				ret = CDBService::GetInstance()->InsertDataTable(batchTableName,
// 					lstBatchData);
// 				if (!ret) {// ��������ʧ��
// 
// 				}
// 			}
// #endif
//         }
// 
//         //while (!CDBService::GetInstance()->m_dataQueue.Empty())
//         //{
//         //    QString tableFormat(QStringLiteral("DeviceData_%1"));
//         //    QString todayTableName = tableFormat.arg(QDateTime::currentDateTime().toString("yyyyMMdd"));
//         //    T_DEVICE_DATA deviceData = CDBService::GetInstance()->DataQueuePopFront();
//         //    bool ret = CDBService::GetInstance()->IsExistTable(todayTableName);
//         //    if(!ret) {
//         //        //���ݱ�����,�ȴ����������ݱ�
//         //        ret = CDBService::GetInstance()->CreateDataTable(todayTableName);
//         //        if(!ret) {
//         //            //�����������ݱ�ʧ��
//         //        } else {
//         //            ret = CDBService::GetInstance()->InsertDataTable(todayTableName,
//         //                deviceData.qstrDeviceID, deviceData.deviceData);
//         //            if(!ret) {
// 
//         //            }
//         //        }
//         //    } else {
//         //        ret = CDBService::GetInstance()->InsertDataTable(todayTableName,
//         //            deviceData.qstrDeviceID, deviceData.deviceData);
//         //        if(!ret) {
// 
//         //        }
//         //    }
//         //}
// 
//         CDBService::GetInstance()->m_dataQueue.Wait(1000);
//     }
// 
//    
// 	QMap<QString, QList<T_DEVICE_DATA>*>::iterator begin = m_device_map.begin(), end = m_device_map.end();
// 	for (
// 		; begin != end;
// 		++begin
// 		)
// 	{
// 		if (NULL != begin.value())
// 			delete begin.value();
// 	}
// 	LOG_DEBUG() << "CDBHandleTask " << taskID << "exit";
// }


//�°洦��
void CDBHandleTask::run()
{
	QString qstrFormat(QStringLiteral("%1##%2"));
	QString taskID = qstrFormat.arg(QDateTime::currentDateTime().toString("dd#HH_mm_ss__zzz"))
		.arg((int)QThread::currentThreadId());
	LOG_DEBUG() << "CDBHandleTask " << taskID << "start";


	QMap<QString, QList<T_DEVICE_DATA>*>	m_device_map;
	while (!CDBService::GetInstance()->IsStopSingalTriggered())
	{
		while (!CDBService::GetInstance()->m_dataQueue.Empty()) {

			
			T_DEVICE_DATA fontData = CDBService::GetInstance()->DataQueueFront();
			int flag = fontData->m_flag;
// 			QList<T_DEVICE_DATA> listData;
// 			listData.clear();
// 			listData.push_back(fontData);

#ifdef USEMYSQL
			MyDatabase::GetInstance()->InsertDataTable(QString(""), fontData);
#else
			QString batchTableName;
			if (OLD_TYPE == flag) //��ʽ��ṹ
			{
				batchTableName = (QStringLiteral("DeviceData_%1")).
					arg(QDateTime::currentDateTime().toString("yyyyMMdd"));
			}
			else if(NEW_TYPE == flag)
			{
				batchTableName = (QStringLiteral("DeviceData_%1")).
					arg(QDateTime::currentDateTime().toString("yyyyMMdd"));
			}
			bool ret = CDBService::GetInstance()->IsExistTable(batchTableName);
			if (!ret) {
				ret = CDBService::GetInstance()->CreateDataTable(batchTableName,flag);
				if (!ret) {
					break;// ����ʧ��
				}
			}

			// ������������
			ret = CDBService::GetInstance()->InsertDataTable(batchTableName,
				fontData);
#endif

			CDBService::GetInstance()->DataQueuePopFront();//���� ������
		}

		CDBService::GetInstance()->m_dataQueue.Wait(1000);
	}


	QMap<QString, QList<T_DEVICE_DATA>*>::iterator begin = m_device_map.begin(), end = m_device_map.end();
	for (
		; begin != end;
		++begin
		)
	{
		if (NULL != begin.value())
			delete begin.value();
	}
	LOG_DEBUG() << "CDBHandleTask " << taskID << "exit";
}



// ��ȡ�����ָ���豸number ����Ϣ
bool CDBService::GetDeviceDataByDeviceCode(const QString & deviceCode
	, DeviceInfo & deviceInfo)
{

	Q_ASSERT(deviceCode.length() > 0);

#ifdef USEMYSQL
	return MyDatabase::GetInstance()->GetDeviceDataByDeviceCode(deviceCode, deviceInfo);
#else
	db_con_ptr con = GetDBConnection(DB_CON_READONLY);
	if (!con) {
		return false;
	}

	con->AcquireWrite();
	QSqlQuery query(*(con->pDB));
	//QString qstrSQL = QStringLiteral("SELECT ID FROM Rig_DeviceInfo WHERE Code = ?");
	QString qstrSQL = QStringLiteral("SELECT ID,CreateDate,Longitude,Latitude FROM Device WHERE CreateDate in \
(select MAX([CreateDate]) from [Device] where [Number] =?) and  [Number] =?");

	if (!query.prepare(qstrSQL)) {
		LOG_WARING() << "SQLQuery prepare failed. error:" << con->pDB->lastError().text()
			<< " sql:" << qstrSQL;
		con->Release();
		return false;
	}

	query.bindValue(0, deviceCode);
	query.bindValue(1, deviceCode);
	if (!query.exec()) {
		LOG_WARING() << "SQLQuery exec failed. error:" << con->pDB->lastError().text()
			<< " sql:" << qstrSQL;
		con->Release();
		return false;
	}

	bool ret = query.next();
	if (ret) {
		
		deviceInfo.m_strID = query.value(0).toString();
		deviceInfo.m_dateTime = query.value(1).toDateTime().toSecsSinceEpoch();
		deviceInfo.m_strJD = query.value(2).toFloat();
		deviceInfo.m_strWD = query.value(3).toFloat();
	}
	con->Release();
	return ret;
#endif
	
}


//�����豸��
bool CDBService::UpdateDeviceInfo(DeviceInfo &info)
{
	//û�оʹ���,�о͸���

	LOG_DEBUG() << QStringLiteral("��ʼ�������ݱ�0>>>>>>>>>>>>>>>>>>>>>");
	db_con_ptr con = GetDBConnection(DB_CON_READONLY);
	if (!con) {
		return false;
	}

	con->AcquireWrite();
	QSqlQuery query(*(con->pDB));
	//QString qstrSQLFormat(QStringLiteral("CREATE TABLE %1 (ID INT IDENTITY (1, 1) NOT NULL,DeviceID VARCHAR (50) NOT NULL,ChannelNo INT NOT NULL,DataType INT NOT NULL,TIMESTAMP BIGINT NOT NULL,Longitude DOUBLE PRECISION NOT NULL,Latitude DOUBLE PRECISION NOT NULL,Concentration DECIMAL(12,6) NOT NULL,MeasurementUnit INT NOT NULL)"));
	LOG_DEBUG() << QStringLiteral("����Device��>>>>>>>>>>>>>>>>>>>>>");

	QString qstrSelect,qstrUpdate,qstrInsert;
	
	qstrSelect = QString("select count(*) from Device where Id = '%1'");
	qstrUpdate = QString("UPDATE Device set [CreateDate] ='%1', [Longitude] ='%2',[Latitude]='%3' where Id='%4' ");
	qstrInsert = QString("Insert into Device ([Id],[Number],[Name],[State],[IsDelete],[CreateDate],[CreateUser],[Longitude],[Latitude]\
				,[Type]) values(");

	QString strTime = (QDateTime::fromSecsSinceEpoch(info.m_dateTime).toString("yyyy-MM-dd hh:mm:ss:zzz"));
	QString qstrSQL = qstrSelect.arg(info.m_strID);
	if (!query.prepare(qstrSQL)) {
		LOG_INFO() << "SQLQuery prepare failed. error:" << con->pDB->lastError().text()
			<< " sql:" << qstrSQL;
		con->Release();
		return false;
	}
	if (!query.exec()) {
		LOG_INFO() << "SQLQuery exec failed. error:" << con->pDB->lastError().text()
			<< " sql:" << qstrSQL;
		con->Release();
		return false;
	}

	bool ret = query.next();
	if (ret) { //�б� �͸���
		//qstrSQL = qstrUpdate.arg(QVariant(info.m_dateTime)).arg(info.m_strJD).arg(info.m_strWD).arg(info.m_strID);
		int value = query.value(0).toInt();
		if (value != 0) 
		{
			qstrUpdate = qstrUpdate.arg(strTime).arg(info.m_strJD).arg(info.m_strWD).arg(info.m_strID);
			if (!query.prepare(qstrUpdate)) {
				LOG_INFO() << "SQLQuery prepare failed. error:" << con->pDB->lastError().text()
					<< " sql:" << qstrUpdate;
				con->Release();
				return false;
			}
			//���²����� ����ṹ
			if (m_deviceMsgMap.contains(info.m_strID))
			{
				m_deviceMsgMap[info.m_strID].m_longitude = QString("%1").arg(info.m_strJD);
				m_deviceMsgMap[info.m_strID].m_latitude = QString("%1").arg(info.m_strWD);
			}
// 			query.addBindValue(QVariant(strTime));
// 			query.addBindValue(QVariant(info.m_strJD));
// 			query.addBindValue(QVariant(info.m_strWD));
// 			query.addBindValue(QVariant(info.m_strID));
			if (!query.exec()) {
				LOG_INFO() << "SQLQuery exec failed. error:" << con->pDB->lastError().text()
					<< " sql:" << qstrUpdate;
				con->Release();
				return false;
			}
		}
		else
		{
			qstrInsert += "'" + info.m_strID+"'";
			qstrInsert += ",'" + info.m_strNumber + "'";
			qstrInsert += ",'" + (info.m_strNumber) + "'";
			qstrInsert += ",'" + QString("1") + "'";
			qstrInsert += ",'" + QString("0") + "'";//isDelete �ֶ�Ĭ��0
			qstrInsert += ",'" + strTime + "'";
			qstrInsert += ",'" + info.m_strID + "'";
			qstrInsert += ",'" + QString("%1").arg(info.m_strJD) + "'";
			qstrInsert += ",'" + QString("%1").arg(info.m_strWD) + "'";
			qstrInsert += ",'" + QString("2") + "')"; //type�ֶ� Ĭ��2
			if (!query.prepare(qstrInsert)) {
				LOG_INFO() << "SQLQuery prepare failed. error:" << con->pDB->lastError().text()
					<< " sql:" << qstrInsert;
				con->Release();
				return false;
			}
			//bind
// 			query.addBindValue((info.m_strID));
// 			query.addBindValue((info.m_strNumber));
// 			query.addBindValue((info.m_strNumber));
// 			query.addBindValue((1));
// 			query.addBindValue((0));
// 			query.addBindValue(strTime);
// 			query.addBindValue((info.m_strID));
// 			query.addBindValue((info.m_strJD));
// 			query.addBindValue((info.m_strWD));
		



			if (!query.exec()) {
				LOG_INFO() << "SQLQuery exec failed. error:" << con->pDB->lastError().text()
					<< " sql:" << qstrInsert;
				con->Release();
				return false;
			}
		}

	}
	

	con->Release();
	return ret;
}