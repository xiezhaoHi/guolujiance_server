#include "db_service.h"
#include "service/log_service/log_service.h"
#include "service/configuration_service/configuration_service.h"
#include "service/task_service/task_service.h"
#include "registration/registration.h"
#include "util/global_util.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDateTime>
#include <QThread>


// 用户 SYS_User(F_LoginName, F_Pwd, F_PersonID)
// 井队人员 Rig_RigPersonInfo(ID, RigID, )
// 井队基本信息 Rig_RigBaseInfo(ID, Name, Address, Longitude, Latitude)
/// 流动式H2S检查仪 DEV_PORTABLEh2sdetector
/// 固定式H2S检查仪 DEV_StaticH2SDetector
// 设备信息表 Rig_DeviceInfo(ID, RigID, DeviceType[guidcode], State)
// 

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
    db_con_ptr con = GetDBConnection(DB_CON_READONLY);
    if (!con) {
        return 1;
    }
    m_pDBConnectionForReadOnly = con;

    // 启动专门插入采集数据的线程
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

    if (pCon && pCon->pDB->isOpen()) {
        // 数据库连接正常，直接返回原有数据库数据
    } else {
        if (pCon && pCon->pDB && !pCon->pDB->isOpen()) {// 数据库连接断开，需要重新连接
            pCon->pDB->close();
            QSqlDatabase::removeDatabase(pCon->connectionName);
            LOG_WARING() << QStringLiteral("数据库连接[%1]已断开,错误信息[%2]")
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
    // 软件授权验证
    IS_REGISTED(NULL);

    QString qstrConnectionName =  QDateTime::currentDateTime().toLocalTime()
        .toString(QStringLiteral("con_yyyyMMdd_HHmmss_zzz"));
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC", qstrConnectionName);
    db.setDatabaseName(CConfigrationService::GetInstance()->GetDBConnStr());
    db.setHostName(CConfigrationService::GetInstance()->GetDBHostName());
    db.setUserName(CConfigrationService::GetInstance()->GetDBUser());
    db.setPassword(CConfigrationService::GetInstance()->GetDBPassword());
    db.setPort(CConfigrationService::GetInstance()->GetDBPort());
    
    if (!db.open()) {
        LOG_WARING() << QStringLiteral("打开数据库失败 error:") << db.lastError().text();
        return NULL;
    }

    LOG_INFO() << QStringLiteral("新建数据库连接[%1]成功,数据库连接字符串[%2]")
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
            LOG_INFO() << QStringLiteral("关闭可读写数据库连接");
        }
        m_pDBConnectionForReadWrite->pDB = NULL;
        m_pDBConnectionForReadWrite = NULL;
    }

    if (m_pDBConnectionForReadOnly) {
        if (m_pDBConnectionForReadOnly->pDB->isOpen()) {
            m_pDBConnectionForReadOnly->pDB->close();
            LOG_INFO() << QStringLiteral("关闭只读数据库连接");
        }
        m_pDBConnectionForReadOnly->pDB = NULL;
        m_pDBConnectionForReadOnly = NULL;
    }
}

//******************************************************
//** 函数名:   IsMatchedAccount
//** 功能简述: 判断用户名、密码是否匹配
//** 输入参数: 
//   const QString & account:
//   const QString & password:
//
//** 返回值: 
//   bool:
//
//** 创建日期：2015/07/26
//** 创建人：  唐桥
//** 修改记录：
//   日期        版本      修改人    修改内容
//
//** 其它说明：
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
        LOG_WARING() << QStringLiteral("查询用户名、密码失败. error:") << con->pDB->lastError().text()
            << " sql:"  << query.executedQuery();
    }

    return ret;
}

//******************************************************
//** 函数名:   IsValidUser
//** 功能简述: 判断是否为有效用户
//** 输入参数: 
//   const QString & account:
//
//** 返回值: 
//   bool:
//
//** 创建日期：2015/07/26
//** 创建人：  唐桥
//** 修改记录：
//   日期        版本      修改人    修改内容
//
//** 其它说明：
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
//** 函数名:   GetAccountID
//** 功能简述: 获取用户ID
//** 输入参数: 
//   const QString & account:
//   QString & accountID:
//
//** 返回值: 
//   bool:
//
//** 创建日期：2015/07/26
//** 创建人：  唐桥
//** 修改记录：
//   日期        版本      修改人    修改内容
//
//** 其它说明：
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
//** 函数名:   GetRigIDByAccountID
//** 功能简述: 通过用户ID获取其所属井队ID
//** 输入参数: 
//   const QString & accountID:
//   QString & rigID:
//
//** 返回值: 
//   bool:
//
//** 创建日期：2015/07/30
//** 创建人：  唐桥
//** 修改记录：
//   日期        版本      修改人    修改内容
//
//** 其它说明：
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
//** 函数名:   GetRigInfoByID
//** 功能简述: 根据井队ID获取井队详细信息
//** 输入参数: 
//   const QString & rigID:
//   CRigInfo & rigInfo:
//
//** 返回值: 
//   bool:
//
//** 创建日期：2015/07/30
//** 创建人：  唐桥
//** 修改记录：
//   日期        版本      修改人    修改内容
//
//** 其它说明：
//******************************************************
bool CDBService::GetRigInfoByID(const QString & rigID, CRigInfo & rigInfo)
{
    // TODO
    return false;
}

//******************************************************
//** 函数名:   GetOilFieldIDByRigID
//** 功能简述: 根据井队ID获取所属油田ID
//** 输入参数: 
//   const QString & rigID:
//   QString & oilFieldID:
//
//** 返回值: 
//   bool:
//
//** 创建日期：2015/07/30
//** 创建人：  唐桥
//** 修改记录：
//   日期        版本      修改人    修改内容
//
//** 其它说明：
//******************************************************
bool CDBService::GetOilFieldIDByRigID(const QString & rigID, QString & oilFieldID)
{
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
//** 函数名:   GetOilFieldInfoByID
//** 功能简述: 根据油田ID获取油田信息
//** 输入参数: 
//   const QString & oilFieldID:
//   COilFieldInfo & oilFieldInfo:
//
//** 返回值: 
//   bool:
//
//** 创建日期：2015/07/30
//** 创建人：  唐桥
//** 修改记录：
//   日期        版本      修改人    修改内容
//
//** 其它说明：
//******************************************************
bool CDBService::GetOilFieldInfoByID(const QString & oilFieldID, COilFieldInfo & oilFieldInfo)
{

    // TODO
    return false;
}


//******************************************************
//** 函数名:   GetDeviceListByAccountID
//** 功能简述: 获取用户ID
//** 输入参数: 
//   const QString & account:
//   QList<CMessageDevicesListItem *> lstDevices:
//
//** 返回值: 
//   bool:
//
//** 创建日期：2015/07/26
//** 创建人：  杨恒
//** 修改记录：
//   日期        版本      修改人    修改内容
//
//** 其它说明：
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
//** 函数名:   GetDeviceInfoByDeviceID
//** 功能简述: 获取用户ID
//** 输入参数: 
//   const QString & account:
//   QString & accountID:
//
//** 返回值: 
//   bool:
//
//** 创建日期：2015/07/26
//** 创建人：  唐桥
//** 修改记录：
//   日期        版本      修改人    修改内容
//
//** 其它说明：
//******************************************************
bool CDBService::GetDeviceInfoByDeviceID(const QString & deviceID,CMessageDeviceItemInfo * pDevice)
{
    Q_ASSERT(deviceID.length() > 0);

    db_con_ptr con = GetDBConnection(DB_CON_READONLY);
    if (!con) {
        return false;
    }

    con->AcquireWrite();
    QSqlQuery query(*(con->pDB));
    // 筛选条件由设备的code改为设备ID tangqiao 2015/07/30
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
//** 函数名:   GetRigIDByDeviceID
//** 功能简述: 通过设备ID获取所属井队ID
//** 输入参数: 
//   const QString & deviceID:
//   QString & rigID:
//
//** 返回值: 
//   bool:
//
//** 创建日期：2015/07/30
//** 创建人：  唐桥
//** 修改记录：
//   日期        版本      修改人    修改内容
//
//** 其它说明：
//******************************************************
bool CDBService::GetRigIDByDeviceID(const QString & deviceID, QString & rigID)
{
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
    // TOOD 设备编号&设备类型

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
     QString qstrSQL = QStringLiteral("SELECT * FROM Rig_DeviceInfo WHERE Code = ?");

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
    QString qstrSQL = QStringLiteral("SELECT ID FROM Rig_DeviceInfo WHERE Code = ?");

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
//** 函数名:   GetOilFieldList
//** 功能简述: 取得所有油田信息
//** 输入参数: 
//   QList<T_TREE_NODE_INFO> lstResult:
//
//** 返回值: 
//   bool:
//
//** 创建日期：2015/07/26
//** 创建人：  杨恒
//** 修改记录：
//   日期        版本      修改人    修改内容
//
//** 其它说明：
//******************************************************
bool CDBService::GetOilFieldList(QList<CBaseEntity> & lstResult)
{

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
//** 函数名:   GetRigListByOilFieldID
//** 功能简述: 通过油田ID取得下属井队信息
//** 输入参数: 
//   const QString & oilFieldID:
//   QList<T_TREE_NODE_INFO> lstResult:
//
//** 返回值: 
//   bool:
//
//** 创建日期：2015/07/26
//** 创建人：  杨恒
//** 修改记录：
//   日期        版本      修改人    修改内容
//
//** 其它说明：
//******************************************************
bool CDBService::GetRigListByOilFieldID(const QString & oilFieldID,QList<CBaseEntity> & lstResult)
{
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
//** 函数名:   GetDeviceListByRigID
//** 功能简述: 通过井队ID取得下属设备信息
//** 输入参数: 
//   const QString & rigID:
//   QList<T_TREE_NODE_INFO> lstResult:
//
//** 返回值: 
//   bool:
//
//** 创建日期：2015/07/26
//** 创建人：  杨恒
//** 修改记录：
//   日期        版本      修改人    修改内容
//
//** 其它说明：
//******************************************************
bool CDBService::GetDeviceListByRigID(const QString & rigID,QList<CDeviceInfo> & lstResult)
{
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
//** 函数名:   IsExistTable
//** 功能简述: 是否存在表
//** 输入参数: 
//   const QString & tableName:
//
//** 返回值: 
//   bool:
//
//** 创建日期：2015/07/26
//** 创建人：  杨恒
//** 修改记录：
//   日期        版本      修改人    修改内容
//
//** 其它说明：
//******************************************************
bool CDBService::IsExistTable(const QString & tableName)
{
    Q_ASSERT(tableName.length() > 0);

    {// 先查找缓存里的表名
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
        return false;
    }

    query.bindValue(0, tableName);
    if (!query.exec()) {
        LOG_INFO() << "SQLQuery exec failed. error:" << con->pDB->lastError().text()
            << " sql:"  << qstrSQL;
        con->Release();
        return false;
    }

    bool ret = query.next();
    con->Release();

    if (ret) {// 更新缓存里的表
        QMutexLocker lock(m_pMutexForSet);
        m_setTableName.insert(tableName);
    }

    return ret;
}

//******************************************************
//** 函数名:   CreateDataTable
//** 功能简述: 创建数据表
//** 输入参数: 
//   const QString & tableName:
//
//** 返回值: 
//   bool:
//
//** 创建日期：2015/07/26
//** 创建人：  杨恒
//** 修改记录：
//   日期        版本      修改人    修改内容
//
//** 其它说明：
//******************************************************
bool CDBService::CreateDataTable(const QString & tableName)
{
    Q_ASSERT(tableName.length() > 0);

    db_con_ptr con = GetDBConnection(DB_CON_READONLY);
    if (!con) {
        return false;
    }

    con->AcquireWrite();
    QSqlQuery query(*(con->pDB));
    QString qstrSQLFormat(QStringLiteral("CREATE TABLE %1 (ID INT IDENTITY (1, 1) NOT NULL,DeviceID VARCHAR (50) NOT NULL,ChannelNo INT NOT NULL,DataType INT NOT NULL,TIMESTAMP BIGINT NOT NULL,Longitude DOUBLE PRECISION NOT NULL,Latitude DOUBLE PRECISION NOT NULL,Concentration DECIMAL(12,6) NOT NULL,MeasurementUnit INT NOT NULL)"));
    QString qstrSQL = qstrSQLFormat.arg(tableName);
    if (!query.prepare(qstrSQL)) {
        LOG_INFO() << "SQLQuery prepare failed. error:" << con->pDB->lastError().text()
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

    bool ret = IsExistTable(tableName);

    con->Release();
    return ret;
}

//******************************************************
//** 函数名:   InsertDataTable
//** 功能简述: 在数据表中插入数据
//** 输入参数: 
//   const QString & tableName:
//   const CMessageDeviceRealtimeData & deviceData:
//
//** 返回值: 
//   bool:
//
//** 创建日期：2015/07/26
//** 创建人：  杨恒
//** 修改记录：
//   日期        版本      修改人    修改内容
//
//** 其它说明：
//******************************************************
bool CDBService::InsertDataTable(const QString & tableName, const QString deviceID, const CMessageDeviceRealtimeData & deviceData)
{
    Q_ASSERT(tableName.length() > 0);

    db_con_ptr con = GetDBConnection(DB_CON_READWRITE);
    if (!con) {
        return false;
    }

    con->AcquireWrite();
    QSqlQuery query(*(con->pDB));
    QString qstrSQLFormat(QStringLiteral("INSERT INTO %1 ([DeviceID],[ChannelNo],[DataType],[TimeStamp],[Longitude],[Latitude],[Concentration],[MeasurementUnit]) VALUES (?,?,?,?,?,?,?,?)"));
    QString qstrSQL = qstrSQLFormat.arg(tableName);

    if (!query.prepare(qstrSQL)) {
        LOG_INFO() << "SQLQuery prepare failed. error:" << con->pDB->lastError().text()
            << " sql:"  << qstrSQL;
        con->Release();
        return false;
    }

    // 气体名称
    int gasType = deviceData.m_struct.data_[5];

    // 气体浓度
    double concentration = 0.0;
    {
        U8 buf[8];
        U8 * pData = (U8 *)&(deviceData.m_struct.data_);
        memcpy(buf, pData, 7);

        concentration = Convert7ByteDataToD64(buf);
    }

    // 测量单位
    int unitOfMeasurement = deviceData.m_struct.data_[6];

    query.bindValue(0, deviceID);
    query.bindValue(1, deviceData.m_struct.channelNo);
    query.bindValue(2, gasType);
    query.bindValue(3, deviceData.m_struct.timestamp);
    query.bindValue(4, deviceData.m_struct.longitude);
    query.bindValue(5, deviceData.m_struct.latitude);
    query.bindValue(6, concentration);    // 气体浓度
    query.bindValue(7, unitOfMeasurement);// 测量单位

    bool ret = query.exec();
    if (!ret) {
        LOG_INFO() << "SQLQuery exec failed. error:" << con->pDB->lastError().text()
            << " sql:"  << qstrSQL;
        con->Release();
        return false;
    }

    con->Release();
    return ret;
}

//******************************************************
//** 函数名:   InsertDataTable
//** 功能简述: 
//** 输入参数: 
//   const QString & tableName:
//   QList<T_DEVICE_DATA> & lst:
//
//** 返回值: 
//   bool:
//
//** 创建日期：2015/08/12
//** 创建人：  唐桥
//** 修改记录：
//   日期        版本      修改人    修改内容
//
//** 其它说明：
//******************************************************
bool CDBService::InsertDataTable(const QString & tableName, QList<T_DEVICE_DATA> & lst)
{
    if (lst.size() == 0) {
        return true;
    }

    // 批量插入数据
    db_con_ptr con = GetDBConnection(DB_CON_READWRITE);
    if (!con) {
        return false;
    }

    con->AcquireWrite();
    QSqlQuery query(*(con->pDB));
    QString qstrSQLFormat(QStringLiteral("INSERT INTO %1 ([DeviceID],[ChannelNo],[DataType],[TimeStamp],[Longitude],[Latitude],[Concentration],[MeasurementUnit]) VALUES (?,?,?,?,?,?,?,?)"));
    QString qstrSQL = qstrSQLFormat.arg(tableName);

    if (!query.prepare(qstrSQL)) {
        LOG_INFO() << "SQLQuery prepare failed. error:" << con->pDB->lastError().text()
            << " sql:"  << qstrSQL;
        con->Release();
        return false;
    }

    QVariantList lstDeviceID;
    QVariantList lstChannelNo;
    QVariantList lstDataType;
    QVariantList lstTimestamp;
    QVariantList lstLongitude;
    QVariantList lstLatitude;
    QVariantList lstConcentration;// 气体浓度
    QVariantList lstMeasurementUnit;// 测量单位

    for (T_DEVICE_DATA data : lst) {
        lstDeviceID << data.qstrDeviceID;
        lstChannelNo << data.deviceData.m_struct.channelNo;
        lstDataType << data.deviceData.m_struct.data_[5];
        lstTimestamp << data.deviceData.m_struct.timestamp;
        lstLongitude << data.deviceData.m_struct.longitude;
        lstLatitude << data.deviceData.m_struct.latitude;
        //lstConcentration << BCD2Double(data.deviceData.m_struct.data_ + 1, 3, data.deviceData.m_struct.data_[4]); 
        {
            U8 buf[8];
            U8 * pData = (U8 *)&(data.deviceData.m_struct.data_);
            memcpy(buf, pData, 7);

            double concentration = Convert7ByteDataToD64(buf);
            lstConcentration << concentration;
        }
        lstMeasurementUnit << data.deviceData.m_struct.data_[6];
    }

    query.addBindValue(lstDeviceID);
    query.addBindValue(lstChannelNo);
    query.addBindValue(lstDataType);
    query.addBindValue(lstTimestamp);
    query.addBindValue(lstLongitude);
    query.addBindValue(lstLatitude);
    query.addBindValue(lstConcentration);
    query.addBindValue(lstMeasurementUnit);

    bool ret = query.execBatch();
    if (!ret) {
        LOG_INFO() << "SQLQuery execBatch failed. error:" << con->pDB->lastError().text()
            << " sql:"  << qstrSQL;
        con->Release();
        return false;
    }

    con->Release();
    return ret;
}

//******************************************************
//** 函数名:   DataQueuePushBack
//** 功能简述: 数据处理列队加入数据
//** 输入参数: 
//   const QString data:
//   const CMessageDeviceRealtimeData & deviceData:
//
//** 返回值: 
//   bool:
//
//** 创建日期：2015/07/26
//** 创建人：  杨恒
//** 修改记录：
//   日期        版本      修改人    修改内容
//
//** 其它说明：
//******************************************************
bool CDBService::DataQueuePushBack(const QString & deviceID, 
                                   const CMessageDeviceRealtimeData data)
{
    Q_ASSERT(!deviceID.isEmpty());

    m_dataQueue.Push(T_DEVICE_DATA(deviceID, data));
    return true;
}

//******************************************************
//** 函数名:   DataQueuePopFront
//** 功能简述: 数据处理列队排出数据
//** 输入参数: 
//   const QString data:
//   const CMessageDeviceRealtimeData & deviceData:
//
//** 返回值: 
//   bool:
//
//** 创建日期：2015/07/26
//** 创建人：  杨恒
//** 修改记录：
//   日期        版本      修改人    修改内容
//
//** 其它说明：
//******************************************************
T_DEVICE_DATA CDBService::DataQueuePopFront()
{
    return m_dataQueue.Take();
}

//******************************************************
//** 函数名:   SetDeviceParam
//** 功能简述: 设置设备参数 
//** 输入参数: 
//   const QString & deviceID:
//   CMessageSetDeviceParam & deviceParam:
//
//** 返回值: 
//   bool:
//
//** 创建日期：2015/08/05
//** 创建人：  唐桥
//** 修改记录：
//   日期        版本      修改人    修改内容
//
//** 其它说明：
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

    // 更新设备参数
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
//** 函数名:   SetDeviceName
//** 功能简述: 设置设备名
//** 输入参数: 
//   const QString & deviceID:设备ID
//   const QString & deviceName:设备名
//
//** 返回值: 
//   bool:
//
//** 创建日期：2015/09/18
//** 创建人：  唐桥
//** 修改记录：
//   日期        版本      修改人    修改内容
//
//** 其它说明：
//******************************************************
bool CDBService::SetDeviceName(const QString & deviceID, const QString & deviceName)
{
    db_con_ptr con = GetDBConnection(DB_CON_READONLY);
    if (!con) {
        return false;
    }

    con->AcquireWrite();

    // 设置设备名
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
//** 函数名:   GetDeviceHistoryDataCount
//** 功能简述: 获取设备历史数据条数
//** 输入参数: 
//   const quint64 & begin:
//   const quint64 & end:
//   QVector<QString> deviceIDs:
//   int & count:[出参]历史数据条数
//
//** 返回值: 
//   bool:
//
//** 创建日期：2016/1/4
//** 创建人：  唐桥
//** 修改记录：
//   日期        版本      修改人    修改内容
//
//** 其它说明：
//******************************************************
bool CDBService::GetDeviceHistoryDataCount(const quint64 & begin, 
                                           const quint64 & end, 
                                           QVector<QString> deviceIDs,
                                           int & count)
{
    QString qstrTableName = QDateTime::fromMSecsSinceEpoch(begin).date().toString("Rig_DeviceData_yyyyMMdd");
    if (!IsExistTable(qstrTableName)) {// 不存在指定日期的数据表，直接返回0
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
//** 函数名:   GetDeviceHistoryData
//** 功能简述: 获取设备历史记录
//** 输入参数: 
//   const quint64 & begin:
//   const quint64 & end:
//   int pageIndex:
//   int itemsInPerPage:
//   QVector<QString> deviceIDs:
//   QList<CMessageDeviceDataItem *> & lst:[出参]获取到的历史数据条数
//
//** 返回值: 
//   bool:
//
//** 创建日期：2016/1/4
//** 创建人：  唐桥
//** 修改记录：
//   日期        版本      修改人    修改内容
//
//** 其它说明：
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
    QString qstrTableName = QDateTime::fromMSecsSinceEpoch(begin).date().toString("Rig_DeviceData_yyyyMMdd");
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
//** 函数名:   run
//** 功能简述: 数据处理列队处理线程
//** 输入参数: 
//
//** 返回值: 
//   void:
//
//** 创建日期：2015/07/26
//** 创建人：  杨恒
//** 修改记录：
//   日期        版本      修改人    修改内容
//
//** 其它说明：
//******************************************************
void CDBHandleTask::run()
{
    QString qstrFormat(QStringLiteral("%1##%2"));
    QString taskID = qstrFormat.arg(QDateTime::currentDateTime().toString("dd#HH_mm_ss__zzz"))
        .arg((int)QThread::currentThreadId());
    LOG_DEBUG() << "CDBHandleTask " << taskID << "start";

    while (!CDBService::GetInstance()->IsStopSingalTriggered())
    {
        static int batchSize = 20;
        while (!CDBService::GetInstance()->m_dataQueue.Empty()) {
            QList<T_DEVICE_DATA> lstBatchData;
            QString batchTableName = (QStringLiteral("Rig_DeviceData_%1")).
                arg(QDateTime::currentDateTime().toString("yyyyMMdd"));

            for (int i = 0; 
                !CDBService::GetInstance()->m_dataQueue.Empty() && i < batchSize;
                i++) {
                    T_DEVICE_DATA deviceData = CDBService::GetInstance()->DataQueueFront();
                    QString todayTableName = QStringLiteral("Rig_DeviceData_%1")
                        .arg(QDateTime::fromMSecsSinceEpoch(deviceData.deviceData.m_struct.timestamp)
                        .toString("yyyyMMdd"));
                    if (todayTableName == batchTableName) {
                        deviceData = CDBService::GetInstance()->DataQueuePopFront();
                        lstBatchData.push_back(deviceData);
                    } else { 
                        break;// 接下来的数据需要换表写
                    }
            }

            if (lstBatchData.length() > 0) {
                bool ret = CDBService::GetInstance()->IsExistTable(batchTableName);
                if (!ret) {
                    ret = CDBService::GetInstance()->CreateDataTable(batchTableName);
                    if (!ret) {
                        break;// 建表失败
                    }
                }

                // 批量插入数据
                ret = CDBService::GetInstance()->InsertDataTable(batchTableName,
                    lstBatchData);
                if (!ret) {// 插入数据失败

                }
            }
        }

        //while (!CDBService::GetInstance()->m_dataQueue.Empty())
        //{
        //    QString tableFormat(QStringLiteral("Rig_DeviceData_%1"));
        //    QString todayTableName = tableFormat.arg(QDateTime::currentDateTime().toString("yyyyMMdd"));
        //    T_DEVICE_DATA deviceData = CDBService::GetInstance()->DataQueuePopFront();
        //    bool ret = CDBService::GetInstance()->IsExistTable(todayTableName);
        //    if(!ret) {
        //        //数据表不存在,先创建当天数据表
        //        ret = CDBService::GetInstance()->CreateDataTable(todayTableName);
        //        if(!ret) {
        //            //创建当天数据表失败
        //        } else {
        //            ret = CDBService::GetInstance()->InsertDataTable(todayTableName,
        //                deviceData.qstrDeviceID, deviceData.deviceData);
        //            if(!ret) {

        //            }
        //        }
        //    } else {
        //        ret = CDBService::GetInstance()->InsertDataTable(todayTableName,
        //            deviceData.qstrDeviceID, deviceData.deviceData);
        //        if(!ret) {

        //        }
        //    }
        //}

        CDBService::GetInstance()->m_dataQueue.Wait(1000);
    }

    LOG_DEBUG() << "CDBHandleTask " << taskID << "exit";
}
