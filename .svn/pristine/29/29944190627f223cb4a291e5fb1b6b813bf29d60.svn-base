#include "qallwarninfowidget.h"
#include <QDateTime>
#include <QHeaderView>

QAllWarnInfoWidget::QAllWarnInfoWidget(QSqlDatabase db, QWidget *parent)
	: m_db( db),QWidget(parent)
{
	m_view = new QTableView( this );
	m_model = new AllWarnMsgModel( this );
	m_view->setModel( m_model );
	m_view->verticalHeader()->setVisible(true );
	m_view->verticalHeader()->setMinimumSectionSize(300);
	QRect rc = geometry();
	m_view->setGeometry(10, 10, rc.width() - 10, rc.height()-10);
	m_view->setColumnWidth(0, 30);
	m_view->setColumnWidth(1, 150);
	m_view->setColumnWidth(5, 50);
	m_view->setColumnWidth( 6, 50);
	m_view->setColumnWidth( 7, 60 );
	m_thread = new QQueryWarnThread( m_db, this );
	connect( m_thread, SIGNAL(SendQueryMsg()), this, SLOT(GetQueryMsg()));
}

QAllWarnInfoWidget::~QAllWarnInfoWidget()
{
	if( m_thread->isRunning() )
	{	
		m_thread->wait();
	}
}
void QAllWarnInfoWidget::resizeEvent(QResizeEvent *e)
{
	QRect rc = geometry();
	m_view->setGeometry(10, 10, rc.width() - 10, rc.height()-10);
	QWidget::resizeEvent( e );
	m_view->setColumnWidth(0, rc.width()*0.05);
	m_view->setColumnWidth(1, rc.width()*0.2);
	m_view->setColumnWidth(2, rc.width()*0.15);
	m_view->setColumnWidth(3, rc.width()*0.15);
	m_view->setColumnWidth(4, rc.width()*0.15);
	m_view->setColumnWidth(5, rc.width()*0.08);
	m_view->setColumnWidth(6, rc.width()*0.08);
	m_view->setColumnWidth(7, rc.width()*0.08);

}
void QAllWarnInfoWidget::BeginQueryMsg()
{
	
	if( m_thread->isRunning() )
	{
		m_thread->wait();
	}
	m_thread->start();
}
void QAllWarnInfoWidget::GetQueryMsg()
{
	m_model->cleanData();
	m_model->submit();
	QStringList strList;
	m_thread->getQueryMsg(strList);
	unsigned long ilen = 0;
	for( unsigned long i = 0; i < strList.count(); ++i )
	{
		QStringList strInfo = strList[i].split("_");
		Q_ASSERT(strInfo.count() == 7);
		QString strTime = QDateTime::fromMSecsSinceEpoch(strInfo[6].toULongLong()).toString("yyyy-MM-dd hh:mm:ss");
		A_Warn_Info *info = new A_Warn_Info(QString("%1").arg(i), strInfo[0], strInfo[1],strInfo[2],strTime, strInfo[3], strInfo[4],strInfo[5],this );
		QModelIndex index = m_model->index(0, 0, QModelIndex());
		m_model->setData(index, QVariant::fromValue(info), Qt::UserRole);
		ilen++;
	}
	for( unsigned long i = 0; i < m_strFirstList.count(); ++i )
	{
		QStringList strInfo = m_strFirstList[i].split("_");
		Q_ASSERT(strInfo.count() == 7);
		QString strTime = QDateTime::fromMSecsSinceEpoch(strInfo[6].toULongLong()).toString("yyyy-MM-dd hh:mm:ss");
		A_Warn_Info *info = new A_Warn_Info(QString("%1").arg(ilen), strInfo[0], strInfo[1],strInfo[2],strTime, strInfo[3], strInfo[4],strInfo[5],this );
		QModelIndex index = m_model->index(0, 0, QModelIndex());
		m_model->setData(index, QVariant::fromValue(info), Qt::UserRole);
		ilen++;
	}
	for( unsigned long i = 0; i < m_strSecondList.count(); ++i )
	{
		QStringList strInfo = m_strSecondList[i].split("_");
		Q_ASSERT(strInfo.count() == 7);
		QString strTime = QDateTime::fromMSecsSinceEpoch(strInfo[6].toULongLong()).toString("yyyy-MM-dd hh:mm:ss");
		A_Warn_Info *info = new A_Warn_Info(QString("%1").arg(ilen), strInfo[0], strInfo[1],strInfo[2],strTime, strInfo[3], strInfo[4],strInfo[5],this );
		QModelIndex index = m_model->index(0, 0, QModelIndex());
		m_model->setData(index, QVariant::fromValue(info), Qt::UserRole);
		ilen++;
	}
	emit QueryMsgOver();
}
void QAllWarnInfoWidget::SetBufferMsg(QStringList strlist1,int i )
{
	if( i == 1 )
		m_strFirstList = strlist1;
	else if( i == 2 )
		m_strSecondList = strlist1;
}