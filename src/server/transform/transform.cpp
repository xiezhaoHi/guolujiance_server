#include "transform.h"
#include "QPushButton"
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDateTime>
#include <QThread>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QMap>
transform::transform(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	QWidget* pCenter = new QWidget;

	pCenter->setFixedSize(200, 200);
	
	QPushButton* buttonOld = new QPushButton(QStringLiteral("改名"));
	connect(buttonOld, &QPushButton::clicked, this, &transform::on_old_clicked);

	QPushButton* button = new QPushButton(QStringLiteral("转换"));
	connect(button, &QPushButton::clicked, this, &transform::on_clicked);

	QHBoxLayout* mainLayout = new QHBoxLayout;
	mainLayout->addWidget(buttonOld);
	mainLayout->addWidget(button);
	pCenter->setLayout(mainLayout);

	setCentralWidget(pCenter);
}
//   182.131.2.216,1335\\SQL2005
#define  LOCAL
#ifdef LOCAL
static QString        QSTR_DB_CON_STR = QStringLiteral("DRIVER={SQL SERVER};SERVER=127.0.0.1;DATABASE=GasMonitor");
static QString        QSTR_DB_PASSWORD = QStringLiteral("123456");
static QString        QSTR_DB_HOSTNAME = QStringLiteral("127.0.0.1");
static QString        QSTR_DB_USER = QStringLiteral("sa");
static int            N_DB_PORT = 1433;
#else
static QString        QSTR_DB_CON_STR = QStringLiteral("DRIVER={SQL SERVER};SERVER=182.131.2.216,1433;DATABASE=GasMonitor");
static QString        QSTR_DB_PASSWORD = QStringLiteral("rtmc@@508001");
static QString        QSTR_DB_HOSTNAME = QStringLiteral("182.131.2.216");
static QString        QSTR_DB_USER = QStringLiteral("sa");
static int            N_DB_PORT = 1433;
#endif

QSqlDatabase CreateDBConnection()
{
	// 软件授权验证
	QMessageBox box;

	QString qstrConnectionName = QDateTime::currentDateTime().toLocalTime()
		.toString(QStringLiteral("con_yyyyMMdd_HHmmss_zzz"));
	QSqlDatabase db = QSqlDatabase::addDatabase("QODBC", qstrConnectionName);
	db.setDatabaseName(QSTR_DB_CON_STR);
	//db.setDatabaseName(QString::fromLocal8Bit("QTDSN"));
	db.setHostName(QSTR_DB_HOSTNAME);
	db.setUserName(QSTR_DB_USER);
	db.setPassword(QSTR_DB_PASSWORD);
	db.setPort(N_DB_PORT);

	if (!db.open()) {
		box.setText(QString("error_%1").arg(db.lastError().text()));
		box.exec();
	}
	else
	{
		box.setText("success");
		box.exec();
	}
		
	return QSqlDatabase(db);
}

//QString qstrSQLFormat(QStringLiteral("INSERT INTO %1 ([DeviceId],[CompanyName],[Address],[Longitude],[Latitude],[CurrentTime],[BoilerId],[DeviceData]) VALUES (?,?,?,?,?,?,?,?)"));
QString strCreate = QStringLiteral("CREATE TABLE  %1 (\
										 DeviceId uniqueidentifier NOT NULL,\
										 CompanyName nvarchar(50) NULL,\
										 Address nvarchar(255) NULL,\
										 Longitude DECIMAL(9,6) NOT NULL,\
										 Latitude DECIMAL(9,6) NOT NULL,\
										 Data nvarchar(MAX) NOT NULL,\
										 CurrentTime datetime NOT NULL,\
										 BoilerId uniqueidentifier NULL)");
//.toString("yyyyMMdd")
//转换

//   二氧化硫      一氧化氮       一氧化碳        二氧化氮   氧气       二氧化碳      时间           锅炉ID
//,[SulfurDioxide],[NitricOxide],[CarbonMonoxide],[Hydrogen],[Oxygen],[CarbonDioxide],[CurrentTime],[BoilerId]) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?)"));
enum mamos_ch_index
{

	V_PER_O2 = 0x00,    //O2含量        %
	V_PER_CO2,          //CO2含量       %


	V_PPM_CO ,			//CO含量        ppm
	V_PPM_NO,           //NO含量        ppm
	V_PPM_NO2,          //NO2含量       ppm
	V_PPM_NOx,          //NOx含量       ppm
	V_PPM_SO2,          //SO2含量       ppm

	V_TMP_YAN = 0x200,  //烟温          ℃
	V_TMP_SHI,          //室温          ℃

	V_mbar_DP1 = 0x300, //△P1          mbar
	V_mbar_DP2,         //△P2          mbar
	V_mbar_DP,          //△P           mbar

	V_mPs_L = 0x400,    //气流速度      m/s
	V_mPs_B,            //泵气流速度    m/s

	V_longitude = 0x500,//经度          °
	V_dimension,        //维度          °
	V_Unix32,           //UNIX 32位时间

	
};

char*  gMamosKey[] = { "V_PER_O2","V_PPM_CO", "V_PPM_NO", "V_PPM_SO2"
, "V_PPM_NO2", "V_PER_CO2", nullptr };


void transform::on_clicked(bool checked)
{

	QMap<QString, QVariantList> maplstDeviceID;
	QMap<QString, QVariantList> maplstCompanyName;
	QMap<QString, QVariantList> maplstAddress;
	QMap<QString, QVariantList> maplstLongitude;
	QMap<QString, QVariantList> maplstLatitude;
	QMap<QString, QVariantList> maplstTimestamp;
	QMap<QString, QVariantList> maplstBoilerId;
	QMap<QString, QVariantList> maplstData;

	QVariantList lstDeviceID;
	QVariantList lstCompanyName;
	QVariantList lstAddress;
	QVariantList lstLongitude;
	QVariantList lstLatitude;
	QVariantList lstTimestamp;
	QVariantList lstBoilerId;
	QVariantList lstData;
	QSqlDatabase db = CreateDBConnection();

	QSqlQuery queryDef(db);

	//QString qstrSQL = QStringLiteral("SELECT ID FROM Rig_DeviceInfo WHERE Code = ?");
	QString qstrSQL = QStringLiteral("SELECT * FROM %1");
	QString strInsert = QStringLiteral("insert into %1 values(?,?,?,?,?,?,?,?)");

	QString strOldestName = "DeviceData_20160101_old"; //最老的名字
	QString batchTableNameOld = (QStringLiteral("DeviceData_%1_old"));
	QString batchTableNameNew = (QStringLiteral("DeviceData_%1"));
	QDateTime curTime = QDateTime::currentDateTime();
	QString strOldName = batchTableNameOld.arg(curTime.toString("yyyyMMdd"));
	QString strNewName = batchTableNameNew.arg(curTime.toString("yyyyMMdd"));
	QString strSelect = qstrSQL.arg(strOldName);
	QString strDelete = "drop table %1;";
	
	bool ret;
	QStringList dataList;
	
	while (strOldName != strOldestName)
	{
		ret = queryDef.exec(strSelect);
		
		if (!ret) {
			curTime = curTime.addDays(-1);
			strOldName = batchTableNameOld.arg(curTime.toString("yyyyMMdd"));
			strNewName = batchTableNameNew.arg(curTime.toString("yyyyMMdd"));
			strSelect = qstrSQL.arg(strOldName);
			continue;
		}
		ret = queryDef.next();
		while (ret) {
			dataList.clear();
			lstDeviceID << queryDef.value(0);
			lstCompanyName << queryDef.value(1);
			lstAddress << queryDef.value(2);
			lstLongitude << queryDef.value(3);
			lstLatitude << queryDef.value(4);
			dataList.append(queryDef.value(9).toString());
			dataList.append(queryDef.value(7).toString());
			dataList.append(queryDef.value(6).toString());
			dataList.append(queryDef.value(5).toString());
			dataList.append(queryDef.value(8).toString());
			dataList.append(queryDef.value(10).toString());
			
			lstBoilerId << queryDef.value(12);
			lstTimestamp << queryDef.value(11);

			QJsonObject json;
			for (int index =0; index < dataList.size() && gMamosKey[index] != nullptr; ++index)
			{
				json.insert(gMamosKey[index], dataList[index]);
			}
			QJsonDocument document;
			document.setObject(json);
			QByteArray byte_array = document.toJson(QJsonDocument::Compact);
			QString json_str(byte_array);
			lstData << json_str;
			ret = queryDef.next();
		}
		queryDef.exec(strDelete.arg(strNewName));
		//创建一个新表 //组装数据 插入新表
		if (queryDef.exec(strCreate.arg(strNewName)))
		{

			if (queryDef.prepare(strInsert.arg(strNewName)))
			{
				queryDef.addBindValue(lstDeviceID);
				queryDef.addBindValue(lstCompanyName);
				queryDef.addBindValue(lstAddress);
				queryDef.addBindValue(lstLongitude);
				queryDef.addBindValue(lstLatitude);
				queryDef.addBindValue(lstData);
				queryDef.addBindValue(lstTimestamp);
				queryDef.addBindValue(lstBoilerId);
				queryDef.execBatch(); 
					
			}
			
		}
		
		QString strErr = QString::fromStdString(queryDef.lastError().text().toStdString());
		lstDeviceID.clear();
		lstCompanyName.clear();
		lstAddress.clear();
		lstLongitude.clear();
		lstLatitude.clear();
		lstTimestamp.clear();
		lstBoilerId.clear();
		lstData.clear();
		curTime = curTime.addDays(-1);
		strOldName = batchTableNameOld.arg(curTime.toString("yyyyMMdd"));
		strNewName = batchTableNameNew.arg(curTime.toString("yyyyMMdd"));
		strSelect = qstrSQL.arg(strOldName);
	}

	//[DeviceId], [CompanyName], [Address], [Longitude], [Latitude], [CurrentTime], [BoilerId]
	
	db.close();

	QMessageBox box;
	box.setText("ok");
	box.exec();
}

//改名
void transform::on_old_clicked(bool)
{
	QSqlDatabase db = CreateDBConnection();

	QSqlQuery queryDef(db);

	QString strOldestName = "DeviceData_20160101"; //最老的名字
	QString batchTableNameOld = (QStringLiteral("DeviceData_%1"));
	QString batchTableNameNew = (QStringLiteral("DeviceData_%1_old"));
	QDateTime curTime = QDateTime::currentDateTime();
	QString strOldName = batchTableNameOld.arg(curTime.toString("yyyyMMdd"));
	QString strNewName = batchTableNameNew.arg(curTime.toString("yyyyMMdd"));
	QString strSql,strSqlCount;
	
	while (strOldName != strOldestName)
	{
		//查看表是否已存在
		strSqlCount = QString("select count(*) from %1").arg(strNewName);
		if (!queryDef.exec(strSqlCount)) //无效 表不存在
		{
			strSql = QString("sp_rename '%1','%2'").arg(strOldName).arg(strNewName);
			queryDef.exec(strSql);
		}
		else
		{
			strSql = QString("DROP TABLE %1").arg(strOldName);
			queryDef.exec(strSql);
		}
		curTime = curTime.addDays(-1);
		strOldName = batchTableNameOld.arg(curTime.toString("yyyyMMdd"));
		strNewName = batchTableNameNew.arg(curTime.toString("yyyyMMdd"));
	}

	db.close();
	QMessageBox box;
	box.setText(QStringLiteral("完成"));
	box.exec();
}