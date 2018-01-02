#include "qdataexportthread.h"
#include <QDebug>
#include <QSqlTableModel>
#include <QFileDialog>
#include <QSqlRecord>
#include <QAxWidget>
#include <QSqlQuery>
#include <QSqlError>
#include <qwt_plot_renderer.h>
#include "util/global_util.h"

QDataExportThread::QDataExportThread(QSqlDatabase db, QObject *parent)
    : QThread(parent)
{
    m_qstrExportDir = "./export";
    m_db = db;
    if( !m_db.isOpen() )
    {
        if(!m_db.open())
        {
            qDebug() <<"data base opne false" ;
        }
    }
    m_isImage = false;
    m_isave = false;
    m_istest = false;
}

QDataExportThread::~QDataExportThread()
{

}

void QDataExportThread::setIsExportInfo(bool isbool, bool isave, bool istest )
{
    m_isImage = isbool;
    m_isave = isave;
    m_istest = istest;
}
void QDataExportThread::setDate(QDateTime Startdatetime,QDateTime Stoptime, int AveTm )
{
    m_Starttime = Startdatetime;
	m_Stoptime = Stoptime;
    m_aveTime = AveTm;
}
void QDataExportThread::run()
{
   // if(QDate::currentDate().daysTo( m_date ) > 2 )
     //   return;
    m_mutex.lock();
    if( !m_db.isOpen() )
    {
        if(!m_db.open())
        {
            qDebug() <<"data base opne false" ;
            m_mutex.unlock();
            emit QueryDataCompelete();
            return;
        }
    }
    QString strerror;
    QSqlError errorid;
 
    QSqlQuery *m_pdQuery = new QSqlQuery(m_db);
    bool isExists = false;
    m_dataInfo.clear();
    QString qstrSelectSQL = QString::fromLocal8Bit("select * from dataInfo where 时间>='%1' and 时间<'%2' order by 设备ID ASC, 时间 ASC;").arg(m_Starttime.toString("yyyy-MM-dd hh:mm:ss")).arg( m_Stoptime.toString("yyyy-MM-dd hh:mm:ss") );   

    if( !m_pdQuery->exec( qstrSelectSQL) )
    {
        errorid = m_pdQuery->lastError();
        strerror = errorid.text();
        qDebug() <<strerror;
        emit QueryDataCompelete();
        m_mutex.unlock();
        return;
    }
    while (m_pdQuery->next())
    {
        QStringList st;
        st.append( m_pdQuery->value(0).toString() );
        st.append( m_pdQuery->value(1).toString() );
        st.append( m_pdQuery->value(2).toString());
        st.append( m_pdQuery->value(3).toString() );
        st.append( m_pdQuery->value(4).toString() );
        m_dataInfo.push_back( st );
    }
	//将查询到的信息分为H2S和SO2保存。
	H2SAndSO2();
    if( m_istest )// 导出测量值
    {
        m_test_imageInfo.clear();

        QString sheetName = "Sheet1";
        QSqlDatabase db = QSqlDatabase::addDatabase("QODBC","excelexport");
        if( !db.isValid())
        {
            emit QueryDataCompelete();
            m_mutex.unlock();
            return;   //! type error
        }
		
        QString dsnFormat("DRIVER={Microsoft Excel Driver (*.xls)};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"*.xls\";DBQ=%1/%2_%3.xls");
        QString qstrDsn = dsnFormat.arg(m_qstrExportDir).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss")).arg(QStringLiteral("测量值"));
        db.setDatabaseName(qstrDsn);

        // open connection
        if( !db.open())
        {
            qDebug()<<"error";
            emit QueryDataCompelete();
            m_mutex.unlock();
            return;  //! db error
        }
        QSqlQuery query(db);
        QString sSql;
        bool state;
        // drop the table if it's already exists
        sSql = QString("DROP TABLE [%1]").arg(sheetName);
        query.exec( sSql);
        //create the table (sheet in Excel file)
        sSql = QString::fromLocal8Bit("CREATE TABLE [%1] (").arg(sheetName);
        sSql += QString::fromLocal8Bit("[设备名称] varchar,[设备ID] varchar,[检测时间] varchar, [H2S] numeric ,[SO2] numeric )");
        state = query.prepare( sSql);
        if( !query.exec()) {
            errorid =  query.lastError();
            qDebug() << errorid.text();
            db.close();
            emit QueryDataCompelete();
            m_mutex.unlock();
            return;
        }
        //insert a record
        sSql = QString("INSERT INTO [%1] ").arg( sheetName);
        sSql += QString::fromLocal8Bit("(设备名称, 设备ID, 检测时间, H2S,SO2) VALUES(:data1, :data2, :data3, :data4, :data5)");
        state = query.prepare( sSql);
        //circle
		QString strType;
        for(int i = 0; i < m_h2sAndSo2Info.count(); i++) 
		{			
            for (int j = 0; j < 5; j++)
			{
				if( j < 3 )
				 query.bindValue(QString(":data%1").arg(j+1), m_h2sAndSo2Info[i][j]);	
				else if( j == 3 )
				{					
					query.bindValue(QString(":data%1").arg(j+1),QString::number( m_h2sAndSo2Info[i][j].toFloat(),'f',1));
				}
				else if( j == 4 )
				{
					query.bindValue(QString(":data%1").arg(j+1),QString::number( m_h2sAndSo2Info[i][j].toFloat(),'f',2));
				}
            }
            if( !query.exec())
            {
                errorid = query.lastError();
                qDebug() << errorid.text();
                emit QueryDataCompelete();
                m_mutex.unlock();
                return;
            }
        }
        db.close();
    }

    //for( int i = 0; i < m_dataInfo.count(); ++i )
	for( int i = 0; i < m_h2sAndSo2Info.count(); ++i)
    {
        isExists = false;     
		if( m_h2sAndSo2Info[i].count() < 5 )
            continue;
        for( int j = 0; j < m_test_imageInfo.count(); ++j )
        {
            QString deviceid = m_h2sAndSo2Info[i][1];
            QString deviceid_image = m_test_imageInfo[j].deviceID;
            if( deviceid == deviceid_image )
            {
				
                m_test_imageInfo[j].datas_H2S.x_value.push_back( QDateTime::fromString(m_h2sAndSo2Info[i][2],"yyyy-MM-dd hh:mm:ss").toMSecsSinceEpoch() );
                m_test_imageInfo[j].datas_H2S.y_value.push_back( m_h2sAndSo2Info[i][3].toDouble() );
                m_test_imageInfo[j].datas_SO2.x_value.push_back( QDateTime::fromString(m_h2sAndSo2Info[i][2],"yyyy-MM-dd hh:mm:ss").toMSecsSinceEpoch() );
                m_test_imageInfo[j].datas_SO2.y_value.push_back( m_h2sAndSo2Info[i][4].toDouble() );				
                isExists = true;
                break;// tangqiao 2015/12/22
            }
        }
        if( isExists )
            continue;
        EXPORT_IMAGE_INFO st;
        st.deviceName = m_h2sAndSo2Info[i][0];
        st.deviceID = m_h2sAndSo2Info[i][1];
        st.dates = m_Starttime.toString("yyyy-MM-dd");

	
        st.datas_H2S.x_value.push_back(QDateTime::fromString(m_h2sAndSo2Info[i][2],"yyyy-MM-dd hh:mm:ss").toMSecsSinceEpoch() );
        st.datas_H2S.y_value.push_back( m_h2sAndSo2Info[i][3].toDouble() );
        st.datas_SO2.x_value.push_back( QDateTime::fromString(m_h2sAndSo2Info[i][2],"yyyy-MM-dd hh:mm:ss").toMSecsSinceEpoch() );
        st.datas_SO2.y_value.push_back( m_h2sAndSo2Info[i][4].toDouble() );

        m_test_imageInfo.push_back( st );
    }
    if( m_istest && m_isImage )
        emit ExportTestImage();

    isExists = false;
    if( m_isave )
    {
        //m_aveDataInfo.clear();
        m_ave_imageInfo.clear();
        AveExportCalculator();
        QString sheetName = "Sheet1";
        QSqlDatabase db = QSqlDatabase::addDatabase("QODBC","excelexport");
        if( !db.isValid())
        {
            emit QueryDataCompelete();
            m_mutex.unlock();
            return;   //! type error
        }

        // 导出平均值xls
        QString qstrDBNameFormat("DRIVER={Microsoft Excel Driver (*.xls)};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"*.xls\";DBQ=%1/%2_%3.xls");
        QString qstrDBName = qstrDBNameFormat.arg(m_qstrExportDir).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss")).arg(QStringLiteral("平均值"));
        db.setDatabaseName(qstrDBName);

        // open connection
        if( !db.open())
        {
            qDebug()<<"error";
            emit QueryDataCompelete();
            m_mutex.unlock();
            return;  //! db error
        }
        QSqlQuery query(db);
        QString sSql;
        bool state;
        // drop the table if it's already exists
        sSql = QString("DROP TABLE [%1]").arg(sheetName);
        query.exec( sSql);
        //create the table (sheet in Excel file)
        sSql = QString::fromLocal8Bit("CREATE TABLE [%1] (").arg(sheetName);
        sSql += QString::fromLocal8Bit("[设备名称] varchar,[设备ID] varchar,[检测时间] varchar, [H2S] numeric , [SO2] numeric )");
        state = query.prepare( sSql);
        if( !query.exec()) {
            errorid =  query.lastError();
            qDebug() << errorid.text();
            db.close();
            emit QueryDataCompelete();
            m_mutex.unlock();
            return;
        }
        if( m_ave_imageInfo.count() != 0 )
        {
            sSql = QString("INSERT INTO [%1] ").arg( sheetName);
            sSql += QString::fromLocal8Bit("(设备名称, 设备ID, 检测时间,H2S,SO2) VALUES(:data1, :data2, :data3, :data4, :data5)");
            state = query.prepare( sSql);
            for(int i = 0; i < m_ave_imageInfo.count(); i++)
            {
                query.bindValue(QString(":data1") ,m_ave_imageInfo[i].deviceName);
                query.bindValue(QString(":data2") ,m_ave_imageInfo[i].deviceID);
				for( int j = 0; j < m_ave_imageInfo[i].datas_H2S.x_value.count();++j )
				{
					QString strTime = QDateTime::fromMSecsSinceEpoch( m_ave_imageInfo[i].datas_H2S.x_value[j] ).toString("yyyy-MM-dd hh:mm:ss");
					query.bindValue(QString(":data3") , strTime);
					query.bindValue(QString(":data4") , QString::number(m_ave_imageInfo[i].datas_H2S.y_value[j],'f',1));					
					query.bindValue(QString(":data5") ,QString::number(m_ave_imageInfo[i].datas_SO2.y_value[j],'f',2));		
					
					if( !query.exec())
					{
						errorid = query.lastError();
						qDebug() << errorid.text();
						emit QueryDataCompelete();
						db.close();
						m_mutex.unlock();
						return;
					}
				}
				
				}
            }      
        db.close();
        if( m_isImage )
        {		
            emit ExportAveImage();
        }		
    }
    m_mutex.unlock();
    emit QueryDataCompelete();
}
void QDataExportThread::GetDataInfo(QVector<EXPORT_IMAGE_INFO> &vt, bool isave)
{
    m_mutex.lock();
    if( isave )
    {
        vt = m_ave_imageInfo;
    }
    else
    {
        vt = m_test_imageInfo;
    }
    m_mutex.unlock();
}

void QDataExportThread::AveExportCalculator()
{
    QVector<EXPORT_IMAGE_INFO> vt_info = m_test_imageInfo;
    QVector<double> vt_tm;
    QVector<double> vt_val;
    EXPORT_IMAGE_INFO st;
    for( int i = 0; i < vt_info.count(); ++i )
    {
        
        st.datas_H2S.x_value.clear();
        st.datas_H2S.y_value.clear();
        st.datas_SO2.x_value.clear();
        st.datas_SO2.y_value.clear();
        st.deviceName = vt_info[i].deviceName;
        st.deviceID = vt_info[i].deviceID;
        st.dates = vt_info[i].dates;
        AveCalculator(vt_info[i].datas_H2S, m_aveTime, st.datas_H2S );
        AveCalculator( vt_info[i].datas_SO2, m_aveTime, st.datas_SO2 );

        m_ave_imageInfo.push_back( st );
    }
    
}

void QDataExportThread::SetExportDir(const QString & exportDir)
{
    m_qstrExportDir = exportDir;
}

QString QDataExportThread::GetExportDir()
{
    return m_qstrExportDir;
}
void QDataExportThread::H2SAndSO2()
{
	m_H2SdataInfo.clear();
	m_SO2aveDataInfo.clear();
	for( int i = 0; i < m_dataInfo.count();++i )
	{
		if( m_dataInfo[i].at(3).compare("probe1:H2S") == 0 )
		{
			m_H2SdataInfo.push_back( m_dataInfo[i] );
		}
		else if( m_dataInfo[i].at( 3).compare("probe2:SO2") == 0 )
		{
			m_SO2aveDataInfo.push_back( m_dataInfo[i] );
		}
	}
	QVector<QStringList> tmpVt = m_SO2aveDataInfo;
	for( int j = 0; j < m_H2SdataInfo.count() ;++j )
	{
		QStringList tmpList = m_H2SdataInfo[j];
		tmpList.removeAt(3);
		QString tmpTime = m_H2SdataInfo[j].at(2);
		for( int i = 0; i < tmpVt.count();++i )
		{
			if( tmpTime.compare(tmpVt[i].at(2)) == 0 )
			{
				tmpList.push_back( tmpVt[i].at(4));
				tmpVt.remove( i);
				m_h2sAndSo2Info.push_back( tmpList);
				break;
			}
		}
		
	}
}
