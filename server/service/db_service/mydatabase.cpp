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

 /*destroy����*/
 void MyDatabase::Destroy()
 {
    NotifyStop();
    if(m_singleton)
    {
        delete m_singleton;
        m_singleton = NULL;
    }
 }


/*���������߳�*/
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
 * �ж� ���Ƿ����
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


//�����ݿ��������
bool  MyDatabase::AddData(QString const& strSql)
{
    if(strSql.isEmpty())
        return false;
    QSqlDatabase db =  ConnectionPool::openConnection();
    //qDebug()<< QString("��ȡ���ݿ�����");
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
   // qDebug()<< QString("һ�δ洢���");
    return retFlag;
}
//ɾ��ָ����
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
//�������ݿ� ����
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

//��ѯ���ݿ�
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
            int columnNum = sql_query.record().count(); //����
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



/*�Ƿ�ֹͣ*/
bool MyDatabase::IsStop()
{
    return m_stop;
}

/*ֹ֪ͨͣ*/
void MyDatabase::NotifyStop()
{
    m_stop = true;

}
/*
 * �ж����ݿ� �Ƿ��� ָ�����ݿ� �� ָ�����ݱ�
 */
bool    MyDatabase::FindDBTableAndCreate(int flag,QString const& strDBName,QString const& strTableName)
{

    QString strDB = QString("Create Database If Not Exists %1%2 Character Set UTF8")
            .arg(m_strDBName)
            .arg(strDBName);
    bool retFalg = AddData(strDB);
    if(!retFalg)
        return false;

    //������
    if(!TableExists(m_strDBName+strDBName
                                             ,m_strTableName[flag]+strTableName))
    {
        //������
       retFalg = CreateFlagTable(flag,m_strDBName+strDBName,m_strTableName[flag]+strTableName);
        if(!retFalg)
            return false;
        //��ʼ��sq buffer
	}
    return true;
}
/*
 * ����ָ���ı�
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
 * ���� �ṩ ��� ��� ���ݿ� �� ��
 */
void  MyDatabase::Examine_DB_Table()
{
    QDateTime current_date_time = QDateTime::currentDateTime();
//    QString strDate = current_date_time.toString("yyyyMMdd");
//    QString current_time = current_date_time.toString("hh:mm:ss.zzz");

}

/*
1.��ʼ���豸�����Ĺ�¯��Ϣ
2.��ʼ���ֶβ�����
*/
bool MyDatabase::InitDevConfigMap(QString const& strDeviceID)
{
	//1.��ʼ���豸�����Ĺ�¯��Ϣ
	QString qstrSQL = QStringLiteral("SELECT boiler_id FROM boiler_device WHERE id ='%1';").arg(strDeviceID);
	QStringList** pDataList = nullptr;
	int count = 0;
	SelectData(qstrSQL, pDataList, count);

	//��ѯ���豸����Ϣ ����,Ĭ��һ���豸�Ŷ�Ӧһ����Ϣ����
	if (pDataList != nullptr && count > 0)
	{
		m_deviceMsgMap[strDeviceID].m_boilerId		= (*pDataList[0])[0];
	}

	//��Դ�ͷ�
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
	//1.��ʼ���豸�����Ĺ�¯��Ϣ
	QString qstrSQL = QStringLiteral("SELECT param_key FROM boiler_params;");
	QStringList** pDataList = nullptr;
	int count = 0;
	SelectData(qstrSQL, pDataList, count);

	//��ѯ���豸����Ϣ ����,Ĭ��һ���豸�Ŷ�Ӧһ����Ϣ����
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

	//��Դ�ͷ�
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
	bool  selectflag = false; //��ʾ��ѯ���޴�
	LOG_DEBUG() << QStringLiteral("��ʼ��������>>>>>>>>>>>>>>>>>>>>>");

	//˵��  ��Ȼ������List ʵ�� ֻ��һ��ֵ, ��Ϊ�˲���ԭ���Ĵ���
	T_DEVICE_DATA& data = lst;


		

		QString strDeviceID = data->qstrDeviceID;
		

		if (m_deviceMsgMap.contains(strDeviceID))
		{
			LOG_DEBUG() << QStringLiteral("�豸[%1]��Ϣ�ѻ���,ֱ��ʹ��!").arg(strDeviceID);
		}
		else
		{
			InitDevConfigMap(strDeviceID);
			InitParamConfigMap();
			LOG_DEBUG() << QStringLiteral("�豸��һ�ε�½,�ɹ���ȡ�豸��Ϣ������[devId=%1]!").arg(strDeviceID);
		}
    
		//���豸��Ч.
		Q_ASSERT(m_deviceMsgMap.contains(strDeviceID));
	
	//���ݿ� �������
	QString qstrSQLFormat;
	qstrSQLFormat = QString("INSERT INTO `boiler_data`(");
	int listSize = m_paramsList.size();
	for (int index = 0 ; index < listSize;++index)
	{
		qstrSQLFormat +=  m_paramsList[index] + "," ;
	}
	qstrSQLFormat += "`device_id`,`boiler_id`,`create_time`) value(";

	int flag; //��ʶ 0 �ɰ� 1�°�
	flag = data->m_flag;
	QMap<QString, float>   dataMap; //�洢 ��ͬ�ֶ� ��ͬ���͵�����
	if (OLD_TYPE == flag)
	{
		QVariantList listDeviceData;
		QString strData; //����ÿ���������ɵ�json�ַ���
						 //����ÿ������ �е����ݱ�

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

				dataMap[gMamosKey[realData->m_struct.channelNo]] = concentration;
			}
		}
	}
	else if (NEW_TYPE == flag)
	{
		QVariantList listDeviceData;
		QString strData; //����ÿ���������ɵ�json�ַ���
						 //����ÿ������ �е����ݱ�
		for (std::shared_ptr<NEW_DEVICE_DATA> realData : data->m_vectorDataNew)
		{
			dataMap[realData->m_strFlag] = realData->m_data;
		}
	}
	QString dataTemp; //�ֶζ�Ӧ���м�����
	int coutData = 0; //��Ч���ݸ���,������������0,�Ͳ�����
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
		//�������ݿ�
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

	//��ѯ���豸����Ϣ ����,Ĭ��һ���豸�Ŷ�Ӧһ����Ϣ����
	if (pDataList != nullptr && count > 0)
	{
		deviceInfo.m_strID = (*pDataList[0])[0];
		QDateTime timeT = QDateTime::fromString((*pDataList[0])[1], QString("yyyy-MM-dd hh:mm:ss"));
		deviceInfo.m_dateTime = timeT.toSecsSinceEpoch();
		deviceInfo.m_strJD = (*pDataList[0])[2].toFloat();
		deviceInfo.m_strWD = (*pDataList[0])[3].toFloat();
		ret = true;
	}

	//��Դ�ͷ�
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
