#include "qioservicethread.h"
#include <QDebug>

QIOServiceThread* QIOServiceThread::m_instance = NULL;
QIOServiceThread::QIOServiceThread(QObject *parent)
	: QThread(parent)
{
	
}

QIOServiceThread::~QIOServiceThread()
{

}
QIOServiceThread* QIOServiceThread::getInstance()
{
	if( m_instance == NULL )
	{
		m_instance = new QIOServiceThread();
	}
	return m_instance;
}

void QIOServiceThread::run()
{
	
//	m_work->reset( boost::asio::io_service::work(m_io_service));
	boost::asio::io_service::work work(m_io_service);
	m_io_service.run();
	QString str = QString("ioservice over!");
	qDebug() << str;
	
}
void QIOServiceThread::StopThread()
{
	m_io_service.stop();
}