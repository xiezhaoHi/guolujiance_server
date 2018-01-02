/*****************************************************
**  文 件 名：tcp_logic.cpp
**  版 权：XXX 版权所有
**  版    本：Ver.1.0
**  内容简述：通讯逻辑类实现文件
**
**  创建日期：2015/07/22 
**  创建人：杨恒
**  修改记录：
    日期        版本      修改人    修改内容   
*****************************************************/

#include "tcp_manager.h"

//单键对象方法
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

//初始化
bool CTcpManager::init()
{
	//读取配置文件
	CGlobalUtil::getInstance()->ReadIniFile();
	serverIP=CGlobalUtil::getInstance()->GetServerIP();
	serverPort=CGlobalUtil::getInstance()->GetServerPort();
	return true;
}
CTcpManager::CTcpManager(QObject * parent ):QObject( parent )

{

	

}
//构造
CTcpManager::CTcpManager(void)
{
}

//析构
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
			//系统回复消息
			sys_msgs_queue.push_back(pMessage);
		}
		else if(pMessage->m_header.m_struct.cmd == PC_SERVER_CMD_BROADCAST_REALTIME_DATA)
		{
			//数据回复消息
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

//开始Tcp服务
void CTcpManager::StartTcpServer()
{
	//打开Tcp链接，开始通讯
	boost::asio::io_service io_service;

	tcp::resolver resolver(io_service);

	tcp::resolver::query query("127.0.0.1", "7024");
	auto endpoint_iterator = resolver.resolve(query);
	CTcpClient c(io_service, endpoint_iterator);
	m_pTcpClient=&c;
	std::thread t([&io_service](){ io_service.run(); });
	//m_nTimerId = startTimer(1000);
}

//开始Tcp服务
void CTcpManager::StopTcpServer()
{

}
bool CTcpManager::SendMessage(CMessage *message)
{
	m_pTcpClient->write(message);
	return true;
}

