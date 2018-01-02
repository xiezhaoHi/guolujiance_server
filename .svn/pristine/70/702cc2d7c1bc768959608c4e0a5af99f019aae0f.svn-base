#include "qcheckrecvloaddataqueuethread.h"

QCheckRecvLoadDataQueueThread *QCheckRecvLoadDataQueueThread::m_instance = NULL;
QCheckRecvLoadDataQueueThread::QCheckRecvLoadDataQueueThread(QObject *parent)
	: QThread(parent)
{
	m_flag = true;
	m_messageThread = NULL;
}

QCheckRecvLoadDataQueueThread::~QCheckRecvLoadDataQueueThread()
{

}
void QCheckRecvLoadDataQueueThread::run()
{
	while( m_flag )
	{
		if( !m_messageThread->LoadQueueIsEmpty() )
		{
			CMessage *msg = m_messageThread->LoadDataQueuePopFront();		
			emit ServerRepData(msg);			
		}
		else
		{
			m_messageThread->LoadDataWait( 2000 );
		}
	}
}
QCheckRecvLoadDataQueueThread* QCheckRecvLoadDataQueueThread::GetInstance()
{
	if( m_instance == NULL )
	{
		m_instance = new QCheckRecvLoadDataQueueThread();
	}
	return m_instance;
}
void QCheckRecvLoadDataQueueThread::SetMessageThread(QMessageThread *threads)
{
	if( m_messageThread == NULL )
		m_messageThread = threads;
}
void QCheckRecvLoadDataQueueThread::QuitThread()
{
	m_flag = false;
}
