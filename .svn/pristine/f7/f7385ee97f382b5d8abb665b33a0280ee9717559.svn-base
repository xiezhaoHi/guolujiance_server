#include "qquerydatathread.h"
#include <QSqlQuery>
#include <QSqlError>

QQueryDataThread::QQueryDataThread(QObject *parent)
	:QThread(parent)
{

}

QQueryDataThread::~QQueryDataThread()
{

}

void QQueryDataThread::setQueryFactor(QString strid, int avetim,QSqlDatabase db)
{
	m_id = strid;
	m_db = db;
	m_aveTime = avetim;
}

void QQueryDataThread::GetQueryResult(XY_VALUE &st1, XY_VALUE &st2 )
{
	QMutexLocker locker( &m_mutex );
	st1 = m_aveprobe1;
	st2 = m_aveprobe2;
}
void QQueryDataThread::run()
{

	//QMutexLocker locker( &m_mutex );
	m_mutex.lock();
	if( !m_db.isOpen() )
	{
		if( !m_db.open() )
		{
			qDebug() << "QQueryDataThread open database false!" ;
			m_mutex.unlock();
			return;
		}
	}
	QSqlQuery *m_Query = new QSqlQuery(m_db);
	m_probe1.x_value.clear();
	m_probe1.y_value.clear();
	m_probe2.x_value.clear();
	m_probe2.y_value.clear();
	m_aveprobe1.x_value.clear();
	m_aveprobe1.y_value.clear();
	m_aveprobe2.x_value.clear();
	m_aveprobe2.y_value.clear();

	QString strtime = QDate::currentDate().toString("yyyy-MM-dd");
	strtime += " 00:00:00";
	QSqlError errorid;
	QString strerror;
	QString strsql = QString::fromLocal8Bit("select * from dataInfo where 设备ID='%1' and 时间>='%2' order by 类型 ASC, 时间 ASC;").arg( m_id ).arg(strtime);
	if( !m_Query->exec( strsql) )
	{
		errorid = m_Query->lastError();
		strerror = errorid.text();
		qDebug() <<strerror;
		m_mutex.unlock();
		return;
	}
	m_mutex.unlock();
	while (m_Query->next())
	{
		QString strtype = m_Query->value(3).toString();
		strtype = strtype.left(6);
		qint64 itime = QDateTime::fromString(m_Query->value(2).toString(), "yyyy-MM-dd hh:mm:ss").toMSecsSinceEpoch();
		double ivalue = m_Query->value( 4 ).toDouble();
		if( strtype == "probe1" )
		{
			m_probe1.x_value.push_back(itime);
			m_probe1.y_value.push_back( ivalue );
		}
		else if( strtype == "probe2" )
		{
			m_probe2.x_value.push_back(itime);
			m_probe2.y_value.push_back( ivalue );
		}		
	}
	//m_probe1.x_value +=m_probeLast1.x_value ;
//	m_probe1.y_value += m_probeLast1.y_value;
	//m_probe2.x_value += m_probeLast2.x_value;
//	m_probe2.y_value += m_probeLast2.y_value;

	AveCalculator( m_probe1, m_aveTime, m_aveprobe1);
	AveCalculator( m_probe2, m_aveTime, m_aveprobe2 );
	emit ThreadOver();
}
/*
void QQueryDataThread::AveCalculator(XY_VALUE ProbeValue, int aveTime, XY_VALUE &out)
{
	QVector<double> vt_tm;
	QVector<double> vt_val;
	while( ProbeValue.x_value.count() > 0 )
	{
		double startTm = ProbeValue.x_value[0];
		double endTm = startTm + m_aveTime*60*1000;
		int icount = 0;
		vt_tm.clear();
		vt_val.clear();
		int k = 0;
		while( ProbeValue.x_value.count() > k )
		{
			if( ProbeValue.x_value[k] <= endTm )
			{
				vt_tm.push_back( ProbeValue.x_value[k] );
				vt_val.push_back(ProbeValue.y_value[k]);
				++k;
			}
			else
			{
				//最少有一个数据
				ProbeValue.x_value.remove(0, k);
				ProbeValue.y_value.remove(0, k);
				out.x_value.push_back( AveCalculatefromQVector(vt_tm ));
				out.y_value.push_back( AveCalculatefromQVector(vt_val ));
				vt_tm.clear();
				vt_val.clear();
				k = 0;
				if( ProbeValue.x_value.count()!= 0 )
				{
					startTm = ProbeValue.x_value[0];
					endTm = startTm + m_aveTime*60*1000;
				}

			}
		}
		ProbeValue.x_value.clear();
		ProbeValue.y_value.clear();
		out.x_value.push_back( AveCalculatefromQVector(vt_tm ));
		out.y_value.push_back( AveCalculatefromQVector(vt_val ));

	}	
}
	for( int i = 0; i < m_probe1.x_value.count(); ++i )
	{
		double startTim = m_probe1.x_value[i];
		double 
		if( m_probe1.x_value.count() >= m_icount1)
		{
			QVector<double> st_v = m_probe1.x_value.mid(0, m_icount1);
			double ave = AveCalculatefromQVector(st_v);
			m_aveprobe1.x_value.push_back( ave );
			m_probe1.x_value.remove(0, m_icount1);
		}
		else
		{
			QVector<double> st_v = m_probe1.x_value;
			double ave = AveCalculatefromQVector(st_v);
			m_aveprobe1.x_value.push_back( ave );
			m_probe1.x_value.clear();
		}
		if( m_probe1.y_value.count() >= m_icount1 )
		{
			QVector<double> st_v = m_probe1.y_value.mid(0, m_icount1);
			double ave = AveCalculatefromQVector(st_v);
			m_aveprobe1.y_value.push_back( ave );
			m_probe1.y_value.remove(0, m_icount1);
		}
		else
		{
			QVector<double> st_v = m_probe1.y_value;
			double ave = AveCalculatefromQVector(st_v);
			m_aveprobe1.y_value.push_back( ave );
			m_probe1.y_value.clear();
		}
	}
	for( int i = 0; i < m_probe2.x_value.count(); ++i )
	{
		if( m_probe2.x_value.count() >= m_icount2)
		{
			QVector<double> st_v = m_probe2.x_value.mid(0, m_icount2);
			double ave = AveCalculatefromQVector(st_v);
			m_aveprobe2.x_value.push_back( ave );
			m_probe2.x_value.remove(0, m_icount2);
		}
		else
		{
			QVector<double> st_v = m_probe2.x_value;
			double ave = AveCalculatefromQVector(st_v);
			m_aveprobe2.x_value.push_back( ave );
			m_probe2.x_value.clear();
		}
		if( m_probe2.y_value.count() >= m_icount2 )
		{
			QVector<double> st_v = m_probe2.y_value.mid(0, m_icount2);
			double ave = AveCalculatefromQVector(st_v);
			m_aveprobe2.y_value.push_back( ave );
			m_probe2.y_value.remove(0, m_icount2);
		}
		else
		{
			QVector<double> st_v = m_probe2.y_value;
			double ave = AveCalculatefromQVector(st_v);
			m_aveprobe2.y_value.push_back( ave );
			m_probe2.y_value.clear();
		}
	}
}*/