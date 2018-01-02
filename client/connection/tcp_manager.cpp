/*****************************************************
**  �� �� ����tcp_logic.cpp
**  �� Ȩ��XXX ��Ȩ����
**  ��    ����Ver.1.0
**  ���ݼ�����ͨѶ�߼���ʵ���ļ�
**
**  �������ڣ�2015/07/22 
**  �����ˣ����
**  �޸ļ�¼��
    ����        �汾      �޸���    �޸�����   
*****************************************************/

#include "tcp_manager.h"

//�������󷽷�
static CTcpManager *_sharedObject = nullptr;

CTcpManager* CTcpManager::getInstance()
{
	if (! _sharedObject)
	{
		_sharedObject = new CTcpManager(new QObject());
		_sharedObject->init();
	}

	return _sharedObject;
}

//��ʼ��
bool CTcpManager::init()
{
	//��ȡ�����ļ�
	CGlobalUtil::getInstance()->ReadIniFile();
	serverIP=CGlobalUtil::getInstance()->GetServerIP();
	serverPort=CGlobalUtil::getInstance()->GetServerPort();
	return true;
}
CTcpManager::CTcpManager(QObject * parent ):QObject( parent )

{

	

}
//����
CTcpManager::CTcpManager(void)
{
}

//����
CTcpManager::~CTcpManager(void)
{
	if ( m_nTimerId != 0 )

		killTimer(m_nTimerId);
}
void CTcpManager::timerEvent( QTimerEvent *event )

{
	while (!m_pTcpClient->get_rec_queue().empty())
	{
		CMessage * pMessage=m_pTcpClient->get_rec_queue().front();
		if(pMessage->m_header.m_struct.cmd == PC_SERVER_CMD_LOGIN_REP
			||pMessage->m_header.m_struct.cmd == PC_SERVER_CMD_GET_DEVICE_LIST_REP
			||pMessage->m_header.m_struct.cmd == PC_SERVER_CMD_GET_DEVICE_INFO_REP)
		{
			//ϵͳ�ظ���Ϣ
			sys_msgs_queue.push_back(pMessage);
		}
		else if(pMessage->m_header.m_struct.cmd == PC_SERVER_CMD_BROADCAST_REALTIME_DATA)
		{
			//���ݻظ���Ϣ
			sys_msgs_queue.push_back(pMessage);
		}
		else
		{
			qDebug() << "error msg:" <<  pMessage->ToString();
		}
		/*pMessage->m_header.m_struct.messageNo = 1;
		pMessage->m_header.m_struct.cmdResult = 0;*/

		m_pTcpClient->get_rec_queue().pop_front();
		//delete pMessage;
		pMessage=NULL;
	}
	qDebug( "timer event, id %d",event->timerId() );

}

//��ʼTcp����
void CTcpManager::StartTcpServer()
{
	//��Tcp���ӣ���ʼͨѶ
	boost::asio::io_service io_service;

	tcp::resolver resolver(io_service);

	tcp::resolver::query query("127.0.0.1", "7024");
	auto endpoint_iterator = resolver.resolve(query);
	CTcpClient c(io_service, endpoint_iterator);
	m_pTcpClient=&c;
	std::thread t([&io_service](){ io_service.run(); });
	//m_nTimerId = startTimer(1000);
}

//��ʼTcp����
void CTcpManager::StopTcpServer()
{

}
bool CTcpManager::SendMessage(CMessage *message)
{
	m_pTcpClient->write(message);
	return true;
}

