#include "qquerywarnthread.h"
#include <QSqlQuery>
#include <QVariant>
#include <QDebug>
QQueryWarnThread::QQueryWarnThread(QSqlDatabase db, QObject *parent)
	: QThread(parent)
{
		m_db = db;
		if( !m_db.isOpen() )
		{
			bool iresult = m_db.open();
			if( !iresult )
			{
				qDebug() << "msg database open false";
			}
		}
}

QQueryWarnThread::~QQueryWarnThread()
{

}

void QQueryWarnThread::getQueryMsg(QStringList &strList)
{
	strList = m_strList;
}
void QQueryWarnThread::run()
{
	m_mutex.lock();
	m_strList.clear();
	if( !m_db.isOpen() )
	{
		bool iresult = m_db.open();
		if( !iresult )
		{
			m_mutex.unlock();
			emit SendQueryMsg();
			return;
		}
		
	}
	QSqlQuery *m_pQuery = new QSqlQuery(m_db);
	QString strSql;
	strSql  = QString::fromLocal8Bit("select * from WarnMsg;");
	if( m_pQuery->exec( strSql ))
	{
		
		while (m_pQuery->next())
		{
			m_strList.append( m_pQuery->value(0).toString() );
		}
		emit SendQueryMsg();
	}
	m_mutex.unlock();
}