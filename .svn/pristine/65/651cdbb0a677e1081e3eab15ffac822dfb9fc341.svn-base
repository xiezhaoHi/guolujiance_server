#include "qremovedatatobasethread.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

int QRemoveDataToBaseThread::m_irecord = 0;
QRemoveDataToBaseThread::QRemoveDataToBaseThread(QSqlDatabase db, QObject *parent)
	: m_db( db ), QThread(parent)
{
	m_irecord++;
	qDebug() << "QRemoveDataToBaseThread count:---------------------------------------------" << m_irecord;
}

QRemoveDataToBaseThread::~QRemoveDataToBaseThread()
{
	m_irecord--;
	qDebug() << "QRemoveDataToBaseThread count:---------------------------------------------" << m_irecord;
}


void QRemoveDataToBaseThread::setDataInfo(QVector<DATA_INFO*> datas, bool isLoad )
{
	//QMutexLocker locker(&m_mutex);
	/*m_data.clear();
	m_data = datas;
	m_isavedata = isave;*/
	if( isLoad == false )
	{
		DATA_INFO_ST st;
		st.datainfo = datas;
		m_queue.Push( st );
	}
	else
	{
		DATA_INFO_ST st;
		st.datainfo = datas;
		m_loadqueue.Push( st );
	}
}
void QRemoveDataToBaseThread::run()
{
	while( m_iscontiue )
	{
		int icount = 0;
		bool isload = false;
		if( !m_queue.Empty() || !m_loadqueue.Empty() )
		{
			if( !m_queue.Empty() )
			{
				m_data = m_queue.Take().datainfo;	
			}
			else if( !m_loadqueue.Empty() )
			{
				m_data = m_loadqueue.Take().datainfo;
				icount = m_data.count();
				isload = true;
			}
				
			QString strsql;
			m_mutex.lock();
			QSqlQuery *m_pdQuery = new QSqlQuery(m_db);
			strsql = "begin transaction";
			if(!m_pdQuery->exec( strsql ) )
			{
				QSqlError str = m_pdQuery->lastError();
				QString strerror = str.text();
				qDebug() << strerror;
				m_mutex.unlock();
				break;
			}
			for( int i = 0; i < m_data.count(); ++i )
			{
				QString strtimes = QDateTime::fromMSecsSinceEpoch( m_data[i]->times.toDouble()).toString("yyyy-MM-dd hh:mm:ss");
                strsql = QString("replace into dataInfo values('%1','%2','%3','%4','%5')").arg(m_data[i]->name)
                    .arg(m_data[i]->id).arg(strtimes).arg(m_data[i]->types).arg( m_data[i]->values);

                if(!m_pdQuery->exec( strsql ) )
                {
                    QSqlError str = m_pdQuery->lastError();
                    QString strerror = str.text();
                    qDebug() << strerror;
                }
				delete m_data[i];
			}	
			strsql = "commit transaction";
			if(!m_pdQuery->exec( strsql ) )
			{
				QSqlError str = m_pdQuery->lastError();
				QString strerror = str.text();
				qDebug() << strerror;
				m_mutex.unlock();
				break;
			}	
			if( isload )
			{
				emit SaveLoadDataCount( icount );
			}
				
			delete m_pdQuery;
			m_mutex.unlock();
		}
		else
		{
			m_queue.Wait(1000);
		}
	}	
}
void QRemoveDataToBaseThread::setQuitFlag()
{
	m_iscontiue = false;
}
/*
QueueDataInfo QRemoveDataToBaseThread::GetQueueDataInfo()
{
	//QMutexLocker locker(&m_mutex);
	QueueDataInfo ret = m_queue;
	return ret;
}*/