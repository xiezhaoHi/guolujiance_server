#include "qcheckrecvdataqueuethread.h"

QCheckRecvDataQueueThread * QCheckRecvDataQueueThread::m_instance=NULL;
QCheckRecvDataQueueThread::QCheckRecvDataQueueThread(QObject *parent)
	: QThread(parent)
{
	sendCount = 0;
	m_isRecv = false;
	m_flag = true;
	m_messageThread = NULL;
}

QCheckRecvDataQueueThread::~QCheckRecvDataQueueThread()
{

}

QCheckRecvDataQueueThread* QCheckRecvDataQueueThread::GetInstance()
{
	if( m_instance == NULL )
	{
		m_instance = new QCheckRecvDataQueueThread();
	}
	return m_instance;
}
void QCheckRecvDataQueueThread::QuitThread()
{
	
	m_flag = false;
}
void QCheckRecvDataQueueThread::run()
{
	while( m_flag )
	{
		
	   if( !m_messageThread->DataQueueIsEmpty() )
		{
			if( m_isRecv )
			{
				emit ServerRepData(m_messageThread->DataQueuePopFront());
				sendCount++;
				qDebug() << "SendData count:" << sendCount;				
			}
		}
	   else
	   {
		   m_messageThread->DataWait( 1000 );
	   }
	}
}
void QCheckRecvDataQueueThread::SetMessageThread(QMessageThread *threads)
{
	if( m_messageThread == NULL )
		m_messageThread = threads;
}
void QCheckRecvDataQueueThread::setRecvData( bool isrecv )
{
	m_isRecv = isrecv;
}