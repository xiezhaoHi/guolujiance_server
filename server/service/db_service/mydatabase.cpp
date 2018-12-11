#include "mydatabase.h"

#include<QTime>
#include<QThread>
#include<Qtsql/QSqlRecord>
#include<QTextCodec>
#include<QCoreApplication>
#include "util/global_util.h"
#include <QDateTime>


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

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
MyDatabase::MyDatabase()
{
	qDebug() << QSqlDatabase::drivers();

	qDebug() << QCoreApplication::libraryPaths();
    m_stop = false;
}

MyDatabase::~MyDatabase()
{

}


 MyDatabase*   MyDatabase::m_singleton = NULL;

 /*destroy服务*/
 void MyDatabase::Destroy()
 {
    NotifyStop();
    if(m_singleton)
    {
        delete m_singleton;
        m_singleton = NULL;
    }
 }


/*开启服务线程*/
void  MyDatabase::Start()
{

}
  MyDatabase* MyDatabase::GetInstance(void)
 {
    if(NULL == m_singleton)
        m_singleton = new MyDatabase();
    return m_singleton;
 }

/*
 * 判断 表是否存在
 */
   bool MyDatabase::TableExists(QString const& strDBName,QString const& strTableName)
   {
	   QString strSql = QString("select * from %1.%2;").arg(strDBName).arg(strTableName);
       if(strSql.isEmpty())
           return false;
       QSqlDatabase db =  ConnectionPool::openConnection();
       QSqlQuery sql_query(db);
       bool retFlag = false;

        sql_query.prepare(strSql);

       if(!sql_query.exec())
       {
            qDebug()<<strSql << "##"<<sql_query.lastError();
       }
       else
       {
           retFlag = true;
       }
       ConnectionPool::closeConnection(db);
       return retFlag;
   }


//给数据库添加数据
bool  MyDatabase::AddData(QString const& strSql)
{
    if(strSql.isEmpty())
        return false;
    QSqlDatabase db =  ConnectionPool::openConnection();
    //qDebug()<< QString("获取数据库连接");
    QSqlQuery sql_query(db);
    bool retFlag = false;

     sql_query.prepare(strSql);

    if(!sql_query.exec())
    {
         qDebug()<<strSql << "##"<<sql_query.lastError();
    }
    else
    {
        retFlag = true;
    }
    ConnectionPool::closeConnection(db);
   // qDebug()<< QString("一次存储完成");
    return retFlag;
}
//删除指定的
bool MyDatabase::DeleteData(QString const& strSql)
{
    QSqlDatabase db =  ConnectionPool::openConnection();

    QSqlQuery sql_query(db);
    bool retFlag = false;
    sql_query.prepare(strSql);

    if(!sql_query.exec())
    {
         qDebug()<<sql_query.lastError();
    }
    else
    {
        retFlag = true;
    }
    ConnectionPool::closeConnection(db);
    return retFlag;
}
//更改数据库 数据
bool MyDatabase::ChangeData(QString const& strSql)
{
    QSqlDatabase db =  ConnectionPool::openConnection();

    QSqlQuery sql_query(db);
    bool retFlag = false;
    sql_query.prepare(strSql);

    if(!sql_query.exec())
    {
         qDebug()<<sql_query.lastError();
    }
    else
    {
        retFlag = true;
    }
    ConnectionPool::closeConnection(db);
    return retFlag;
}

//查询数据库
bool MyDatabase::SelectData(QString const& strSql,QStringList**& pList, int& count,int flag)
{
    QSqlDatabase db;
    db =  ConnectionPool::openConnection();
    QSqlQuery sql_query(db);
    bool retFlag = false;
    sql_query.prepare(strSql);

    if(!sql_query.exec())
    {
         qDebug()<<sql_query.lastError();
    }
    else
    {
        retFlag = true;
        count = sql_query.size();
        if(count >0)
        {
            int columnNum = sql_query.record().count(); //列数
            pList = new QStringList*[count];
			memset(pList, 0, count);
            int index = 0;
            while(sql_query.next())
            {
                    pList[index] = new QStringList;
                    for(int col = 0; col < columnNum; ++col)
                    {
                        pList[index]->append(sql_query.value(col).toString().toLocal8Bit());
                    }
                      ++index;
              }
        }
    }
   
   ConnectionPool::closeConnection(db);

    return retFlag;
}



/*是否停止*/
bool MyDatabase::IsStop()
{
    return m_stop;
}

/*通知停止*/
void MyDatabase::NotifyStop()
{
    m_stop = true;

}
/*
 * 判断数据库 是否有 指定数据库 和 指定数据表
 */
bool    MyDatabase::FindDBTableAndCreate(int flag,QString const& strDBName,QString const& strTableName)
{

    QString strDB = QString("Create Database If Not Exists %1%2 Character Set UTF8")
            .arg(m_strDBName)
            .arg(strDBName);
    bool retFalg = AddData(strDB);
    if(!retFalg)
        return false;

    //表不存在
    if(!TableExists(m_strDBName+strDBName
                                             ,m_strTableName[flag]+strTableName))
    {
        //创建表
       retFalg = CreateFlagTable(flag,m_strDBName+strDBName,m_strTableName[flag]+strTableName);
        if(!retFalg)
            return false;
        //初始化sq buffer
	}
    return true;
}
/*
 * 创建指定的表
 */
bool    MyDatabase::CreateFlagTable(int flag,QString const& strDBName, QString const& strTableName)
{
    QString strSql;
    switch (flag) {
    case 1:
            strSql = QString("CREATE TABLE  If Not Exists  `%1`.`%2` (\
                             `structure_stress_id` varchar(10) NOT NULL,\
                            `structure_stress_date_id` varchar(45) DEFAULT NULL,\
                             `structure_stress_date` varchar(45) DEFAULT NULL,\
                             `structure_stress_time` varchar(45) DEFAULT NULL,\
                             `structure_stress_ms` varchar(45) DEFAULT NULL,\
                             `structure_stress_data` varchar(45) DEFAULT NULL,\
                             `save_date` varchar(45) DEFAULT NULL,\
                             `save_time` varchar(45) DEFAULT NULL\
                           ) ENGINE=MyISAM DEFAULT CHARSET=utf8;")
            .arg(strDBName)
            .arg(strTableName)
           ;
        break;
    default:
        break;
    }
    return AddData(strSql);
}

/*
 * 对外 提供 借口 检查 数据库 和 表
 */
void  MyDatabase::Examine_DB_Table()
{
    QDateTime current_date_time = QDateTime::currentDateTime();
//    QString strDate = current_date_time.toString("yyyyMMdd");
//    QString current_time = current_date_time.toString("hh:mm:ss.zzz");

}

/*
1.初始化设备关联的锅炉信息
2.初始化字段参数表
*/
bool MyDatabase::InitDevConfigMap(QString const& strDeviceID)
{
	//1.初始化设备关联的锅炉信息
	QString qstrSQL = QStringLiteral("SELECT boiler_id FROM boiler_device WHERE id ='%1';").arg(strDeviceID);
	QStringList** pDataList = nullptr;
	int count = 0;
	SelectData(qstrSQL, pDataList, count);

	//查询到设备的信息 数据,默认一个设备号对应一个信息数据
	if (pDataList != nullptr && count > 0)
	{
		m_deviceMsgMap[strDeviceID].m_boilerId		= (*pDataList[0])[0];
	}

	//资源释放
	for (int index = 0; index < count; ++index)
	{
		if (pDataList != nullptr && pDataList[index] != nullptr)
		{
			delete pDataList[index];
		}
	}
	delete pDataList;
	return true;
}

bool MyDatabase::InitParamConfigMap()
{
	//1.初始化设备关联的锅炉信息
	QString qstrSQL = QStringLiteral("SELECT param_key FROM boiler_params;");
	QStringList** pDataList = nullptr;
	int count = 0;
	SelectData(qstrSQL, pDataList, count);

	//查询到设备的信息 数据,默认一个设备号对应一个信息数据
	if (pDataList != nullptr && count > 0)
	{
		m_paramsList.clear();
		QString strParam = "";
		for (int index = 0; index < count; ++index)
		{
			strParam = (*pDataList[index])[0];
			m_paramsList.insert(index,strParam);
		}
	}

	//资源释放
	for (int index = 0; index < count; ++index)
	{
		if (pDataList != nullptr && pDataList[index] != nullptr)
		{
			delete pDataList[index];
		}
	}
	delete pDataList;
	return true;
}

bool MyDatabase::InsertDataTable(const QString & tableName, T_DEVICE_DATA&  lst)
{
	if (lst == nullptr) {
		return true;
	}
	Q_ASSERT(lst != nullptr);

	double concentration;
	char sz[12] = { 0 };
	UINT16 maskOfData = 0;
	bool ret = false;
	bool  selectflag = false; //表示查询表无错
	LOG_DEBUG() << QStringLiteral("开始解析数据>>>>>>>>>>>>>>>>>>>>>");

	//说明  虽然参数是List 实际 只有一个值, 是为了不改原来的代码
	T_DEVICE_DATA& data = lst;


		

		QString strDeviceID = data->qstrDeviceID;
		

		if (m_deviceMsgMap.contains(strDeviceID))
		{
			LOG_DEBUG() << QStringLiteral("设备[%1]信息已缓存,直接使用!").arg(strDeviceID);
		}
		else
		{
			InitDevConfigMap(strDeviceID);
			InitParamConfigMap();
			LOG_DEBUG() << QStringLiteral("设备第一次登陆,成功获取设备信息并缓存[devId=%1]!").arg(strDeviceID);
		}
    
		//该设备有效.
		Q_ASSERT(m_deviceMsgMap.contains(strDeviceID));
	
	//数据库 插入语句
	QString qstrSQLFormat;
	qstrSQLFormat = QString("INSERT INTO `boiler_data`(");
	int listSize = m_paramsList.size();
	for (int index = 0 ; index < listSize;++index)
	{
		qstrSQLFormat +=  m_paramsList[index] + "," ;
	}
	qstrSQLFormat += "`device_id`,`boiler_id`,`create_time`) value(";

	int flag; //标识 0 旧版 1新版
	flag = data->m_flag;
	QMap<QString, float>   dataMap; //存储 不同字段 不同类型的数据
	if (OLD_TYPE == flag)
	{
		QVariantList listDeviceData;
		QString strData; //保存每组数据生成的json字符串
						 //遍历每组数据 中的数据表

		double doubleO2 = -1;
		for (message_device_realtime_data_ptr realData : data->m_vectorData) {
			if (realData->m_struct.channelNo != MAMOS_ID)
			{
				U8 buf[8];
				U8 * pData = (U8 *)&(realData->m_struct.data_);
				memcpy(buf, pData, 7);

				concentration = Convert7ByteDataToD64(buf);
				//不会小于0，仅用于防错
				if (concentration < 0)
				{
					concentration = 0;
				}

				dataMap[gMamosKey[realData->m_struct.channelNo]] = concentration;
			}
		}
	}
	else if (NEW_TYPE == flag)
	{
		QVariantList listDeviceData;
		QString strData; //保存每组数据生成的json字符串
						 //遍历每组数据 中的数据表
		for (std::shared_ptr<NEW_DEVICE_DATA> realData : data->m_vectorDataNew)
		{
			dataMap[realData->m_strFlag] = realData->m_data;
		}
	}
	QString dataTemp; //字段对应的中间数据
	int coutData = 0; //有效数据个数,当个数不大于0,就不处理
	for (int index = 0; index < listSize; ++index)
	{
		dataTemp = "";
		if (dataMap.contains(m_paramsList[index]))
		{
			dataTemp = QString::number(dataMap[m_paramsList[index]], 'f', 4) ;
			++coutData;
		}
		qstrSQLFormat += "'" + dataTemp +"',";
	}
	qstrSQLFormat += QString("'%1','%2','%3');").arg(strDeviceID, m_deviceMsgMap[strDeviceID].m_boilerId, QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));

	if (coutData > 0)
	{
		//插入数据库
		AddData(qstrSQLFormat);
	}
}


bool MyDatabase::GetDeviceDataByDeviceCode(const QString & deviceCode
	, DeviceInfo & deviceInfo)
{
	Q_ASSERT(deviceCode.length() > 0);
	bool ret = false;

	//QString qstrSQL = QStringLiteral("SELECT ID FROM Rig_DeviceInfo WHERE Code = ?");
	QString qstrSQL = QStringLiteral("SELECT id,create_date,longitude,latitude FROM boiler_device where number='%1';").arg(deviceCode);

	QStringList** pDataList = nullptr;
	int count = 0;
	SelectData(qstrSQL, pDataList, count);

	//查询到设备的信息 数据,默认一个设备号对应一个信息数据
	if (pDataList != nullptr && count > 0)
	{
		deviceInfo.m_strID = (*pDataList[0])[0];
		QDateTime timeT = QDateTime::fromString((*pDataList[0])[1], QString("yyyy-MM-dd hh:mm:ss"));
		deviceInfo.m_dateTime = timeT.toSecsSinceEpoch();
		deviceInfo.m_strJD = (*pDataList[0])[2].toFloat();
		deviceInfo.m_strWD = (*pDataList[0])[3].toFloat();
		ret = true;
	}

	//资源释放
	for (int index = 0; index < count; ++index)
	{
		if (pDataList != nullptr && pDataList[index] != nullptr)
		{
			delete pDataList[index];
			pDataList[index] = nullptr;
		}
	}
	delete pDataList;
	pDataList = nullptr;

	return ret;
}
