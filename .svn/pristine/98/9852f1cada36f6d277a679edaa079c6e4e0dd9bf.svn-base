#include "qcheckrecvqueuethread.h"
#include "message/message_get_device_list_rep.h"
QCheckRecvQueueThread * QCheckRecvQueueThread::m_instance=NULL;
QCheckRecvQueueThread::QCheckRecvQueueThread(QObject *parent)
	: QThread(parent)
{
	m_isRecv = false;
	m_flag = true;
	m_messageThread = NULL;
}

QCheckRecvQueueThread::~QCheckRecvQueueThread()
{

}
QCheckRecvQueueThread* QCheckRecvQueueThread::GetInstance()
{
	if( m_instance == NULL )
	{
		m_instance = new QCheckRecvQueueThread();
	}
	return m_instance;
}
void QCheckRecvQueueThread::QuitThread()
{
	m_flag = false;
}
void QCheckRecvQueueThread::run()
{
	while( m_flag )
	{
		if( m_messageThread->GetSysMsgQueueCount() > 0 )
		{
			CMessage *msg = m_messageThread->SysQueuePopFront();
			if( msg->m_header.m_struct.cmd == PC_SERVER_CMD_GET_DEVICE_LIST_REP && msg->m_header.m_struct.cmdResult == 0 )
			{
				CMessage *devicesinfo = msg ;//m_messageThread->SysQueuePopFront();
				CMessageGetDeviceListRep* devicerep = static_cast<CMessageGetDeviceListRep*>(devicesinfo->m_pBody);
				QList<CMessageDevicesListItem *> listinfo = devicerep->m_lstDevices;
				
				qDebug() <<"\r\n" << listinfo.count();
			}
			emit ServerRepInfo(msg);			
		}
		else if( m_messageThread->GetDataMsgQueueCount() > 0 )
		{
			if( m_isRecv )
				emit ServerRepData(m_messageThread->DataQueuePopFront());
		}
	}
}
void QCheckRecvQueueThread::SetMessageThread(QMessageThread *threads)
{
	if( m_messageThread == NULL )
		m_messageThread = threads;
}
void QCheckRecvQueueThread::setRecvData( bool isrecv )
{
	m_isRecv = isrecv;
}