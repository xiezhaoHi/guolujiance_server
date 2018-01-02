#include "qmessagethread.h"
#include "message/message_get_devices_info_req.h"
#include "message/message_device_id.h"
#include "message/message_broadcast_device_realtime_data.h"
#include "message/message_device_realtime_data.h"
#include "message/message_get_device_list_rep.h"
#include "qconnectstate.h"

//单键对象方法
//static QMessageThread *_sharedObject = nullptr;
/*
QMessageThread* QMessageThread::getInstance()
{
    if (! _sharedObject)
    {
        _sharedObject = new QMessageThread(new QObject());
        _sharedObject->init();
    }

    return _sharedObject;
}*/

//初始化
bool QMessageThread::init()
{
    //读取配置文件
    /*CGlobalUtil::getInstance()->ReadIniFile();
    serverIP=CGlobalUtil::getInstance()->GetServerIP();
    serverPort=CGlobalUtil::getInstance()->GetServerPort();*/
    return true;
}
QMessageThread::QMessageThread(QObject * parent ):QThread( parent )

{
    
}
//构造
QMessageThread::QMessageThread(void)
{
    m_icount = 0;
    m_isSendMsg = false;
    isInit = true;
    m_isquit = false;
    m_isConnect=false;
    m_ioserviceThread = NULL;
    //读取配置文件
    CGlobalUtil::getInstance()->ReadIniFile();
    serverIP=CGlobalUtil::getInstance()->GetServerIP();
    serverPort=CGlobalUtil::getInstance()->GetServerPort();	
    m_connectTimer = new QTimer( this );

    m_connectTimer->setInterval(5000);
    m_connectTimer->start();
    connect( m_connectTimer, SIGNAL(timeout()), this, SLOT(ConnectSlot()));
	//connect( QConnectState::getInstance(), SIGNAL(disConnect()), this, SLOT(ConnectSlot()));
}

//析构
QMessageThread::~QMessageThread(void)
{
if( m_connectTimer->isActive() )
    m_connectTimer->stop();
}

void QMessageThread::run()
{
    //读取配置文件
    CGlobalUtil::getInstance()->ReadIniFile();
    serverIP=CGlobalUtil::getInstance()->GetServerIP();
    serverPort=CGlobalUtil::getInstance()->GetServerPort();	
    //打开Tcp链接，开始通讯

/*	boost::asio::io_service io_service;
    tcp::resolver resolver(io_service);
    QByteArray baip = serverIP.toLatin1();
    QByteArray baport = serverPort.toLatin1();
    tcp::resolver::query query(baip.data(), baport.data());
    //tcp::resolver::query query("192.168.1.104", "17024");
    auto endpoint_iterator = resolver.resolve(query);
    //CTcpClient c(io_service, endpoint_iterator);
    
//	if( m_isConnect )
    //{
        m_pTcpClient= new CTcpClient(io_service, endpoint_iterator);
        std::thread t = std::thread([&io_service](){ io_service.run();qDebug() << "quit"; });*/

    //}
    
    ConnectSlot();
    //	Sleep( 500 );
    //	m_isConnect = CMessageQueue::getInstance()->GetConnectState();
    while( !m_isquit )
    {
        static int sysi = 0;
        static int recvi = 0;
        //std::thread t2;
        m_isConnect = CMessageQueue::getInstance()->GetConnectState();
        if( m_isConnect )
        {
            if (!CMessageQueue::getInstance()->isEmpty() )
            {
                CMessage * pMessage=CMessageQueue::getInstance()->RecQueuePopFront();
            
                qDebug() << pMessage->ToString();
                if(pMessage->m_header.m_struct.cmd == PC_SERVER_CMD_LOGIN_REP
                    ||pMessage->m_header.m_struct.cmd == PC_SERVER_CMD_GET_DEVICE_LIST_REP
                    ||pMessage->m_header.m_struct.cmd ==  PC_SERVER_CMD_GET_DEVICES_INFO_REP
                    ||pMessage->m_header.m_struct.cmd ==  PC_SERVER_CMD_GET_DEVICE_STATUS_REP
                    ||pMessage->m_header.m_struct.cmd ==  PC_SERVER_CMD_SET_DEVICE_PARAM_REP
					|| pMessage->m_header.m_struct.cmd == PC_SERVER_CMD_SEND_HEARTBEAT_REP
                    )
                {
                
            
                    if( pMessage->m_header.m_struct.cmd == PC_SERVER_CMD_LOGIN_REP )
                    {
                        if( pMessage->m_header.m_struct.cmdResult == 0 )
                        {
                        //	emit ServerRepInfo(1);
                            //系统回复消息
                            
                            m_mutex.lock();
                            m_isSendMsg = true;
                            sys_msgs_queue.Push(pMessage);
                            m_mutex.unlock();
                        }
                        else
                        {
                            emit ServerRepMsg( pMessage->ToString() );
                        }
                    
                    }
                    else if( pMessage->m_header.m_struct.cmd == PC_SERVER_CMD_GET_DEVICE_LIST_REP)
                    {
                        if( pMessage->m_header.m_struct.cmdResult == 0 )
                        {
                    
                                CMessageGetDeviceListRep* devicerep = static_cast<CMessageGetDeviceListRep*>(pMessage->m_pBody);
                                QList<CMessageDevicesListItem *> listinfo = devicerep->m_lstDevices;

                                qDebug() <<"\r\n" << listinfo.count();
                    
                            //系统回复消息
                            m_mutex.lock();
                            sys_msgs_queue.Push(pMessage);
                            m_mutex.unlock();
                        }
                        else
                        {
                            emit ServerRepMsg(pMessage->ToString() );
                        }
                                                             
                    }
                    else if( pMessage->m_header.m_struct.cmd == PC_SERVER_CMD_SET_DEVICE_PARAM_REP 
						|| pMessage->m_header.m_struct.cmd == PC_SERVER_CMD_SEND_HEARTBEAT_REP )
                    {
                        if( pMessage->m_header.m_struct.cmdResult == 0  )
                        {
                            m_mutex.lock();
                            sys_msgs_queue.Push(pMessage);
                            m_mutex.unlock();
                        }
                        else
                        {
                            emit ServerRepMsg( pMessage->ToString() );
                        }
                    
                    }
					
                    sysi++;
                qDebug() <<"MesssageThread total sys count::" << sysi;
                }
                else if(pMessage->m_header.m_struct.cmd == PC_SERVER_CMD_BROADCAST_REALTIME_DATA&& pMessage->m_header.m_struct.cmdResult == 0)
                {
                    
                    //数据回复消息
                    m_mutex.lock();									
                    data_msgs_queue.Push(pMessage);
                    recvi++;
                    qDebug() <<"MesssageThread DataQueue current count:" <<data_msgs_queue.count() <<" total data count:" << recvi << "total sys count:" << sysi;
                    m_mutex.unlock();
                //	emit ServerRepData();
                }
				else if( pMessage->m_header.m_struct.cmd == PC_SERVER_CMD_GET_HISTORY_DEVICE_DATA_COUNT_REP )
				{
					emit ServerRepDeviceDataCount(pMessage);
				}
				else if( pMessage->m_header.m_struct.cmd == PC_SERVER_CMD_GET_HISTORY_DEVICE_DATA_REP)
				{
					load_data_queue.Push(pMessage);
				}
                else
                {
                    qDebug() << "error msg:" <<  pMessage->ToString();
                }
                pMessage=NULL;
            }
            else
            {
                CMessageQueue::getInstance()->wait( 1000 );
            }
        }
        else
        {
        CMessageQueue::getInstance()->wait( 2000 );
        }
    }
}

//开始Tcp服务
void QMessageThread::StartTcpServer()
{
    //打开Tcp链接，开始通讯
    boost::asio::io_service io_service;

    tcp::resolver resolver(io_service);

    tcp::resolver::query query("127.0.0.1", "7024");// WTF? tangqiao 2015/12/22
    auto endpoint_iterator = resolver.resolve(query);
    CTcpClient c(io_service, endpoint_iterator);
    m_pTcpClient= &c;
    std::thread t([&io_service](){ io_service.run(); });
    //m_nTimerId = startTimer(1000);
    m_isConnect = true;
}

//开始Tcp服务
void QMessageThread::SetThreadOver()
{
    m_isquit = true;
}
bool QMessageThread::SendMessage(CMessage *message)
{
    if(m_pTcpClient) 
        m_pTcpClient->write(message);
    return true;
}
CMessage* QMessageThread::SysQueuePopFront()
{
    m_mutex.lock();
    CMessage *ret = NULL;
    if( !sys_msgs_queue.isEmpty() )
         ret = sys_msgs_queue.Take();
    m_mutex.unlock();
    return ret;
}
CMessage* QMessageThread::DataQueuePopFront()
{
    m_mutex.lock();
    CMessage *ret = NULL;
    if( !data_msgs_queue.isEmpty() )
        ret = data_msgs_queue.Take();
    m_mutex.unlock();
    return ret;
}
CMessage *QMessageThread::LoadDataQueuePopFront()
{
	m_mutex.lock();
	CMessage *ret = NULL;
	if( !load_data_queue.isEmpty() )
		ret = load_data_queue.Take();
	m_mutex.unlock();
	return ret;
}
bool QMessageThread::DataQueueIsEmpty()
{
    m_mutex.lock();
    bool isempty = data_msgs_queue.Empty();
    m_mutex.unlock();
    return isempty;
}
bool QMessageThread::SysQueueIsEmpty()
{
    m_mutex.lock();
    bool isempty = sys_msgs_queue.Empty();
    m_mutex.unlock();
    return isempty;
}
bool QMessageThread::LoadQueueIsEmpty()
{
	//m_mutex.lock();
	bool isempty = load_data_queue.Empty();
	//m_mutex.unlock();
	return isempty;
}
bool QMessageThread::SysWait(unsigned long time /* = ULONG_MAX */)
{
    return sys_msgs_queue.Wait( time );
}
bool QMessageThread::DataWait(unsigned long timeout)
{
    return data_msgs_queue.Wait( timeout );
}
bool QMessageThread::LoadDataWait(unsigned long timeout)
{
	return load_data_queue.Wait(timeout);
}

void QMessageThread::ConnectSlot()
{
    bool isconnect = CMessageQueue::getInstance()->GetConnectState();
    
    if( !isconnect )
        ReconnectServer();		
    
}
void QMessageThread::setReConnectInfo(QString name, QString pwd )
{
    isInit = false;
    struser = name;
    strPwd = pwd;
}

void QMessageThread::ReconnectServer()
{
    if( m_ioserviceThread )
    {
        if( m_ioserviceThread->isRunning() )
        {
            m_ioserviceThread->StopThread();
            m_ioserviceThread->wait();
        }
        delete m_ioserviceThread;
        m_ioserviceThread = NULL;
    }
    m_mutex.lock();
    if( m_isSendMsg )
        emit ServerDisconnect();
    m_isSendMsg = false;

    CGlobalUtil::getInstance()->ReadIniFile();
    QString serverIP=CGlobalUtil::getInstance()->GetServerIP();
    QString serverPort=CGlobalUtil::getInstance()->GetServerPort();	
    //打开Tcp链接，开始通讯


    QByteArray baip = serverIP.toLatin1();
    QByteArray baport = serverPort.toLatin1();
    //boost::asio::io_service io_service1;
    QIOServiceThread *ioThread = new QIOServiceThread( this );

    tcp::resolver resolver(ioThread->m_io_service);
    tcp::resolver::query query(baip.data(), baport.data());
    auto endpoint_iterator = resolver.resolve(query);
    m_pTcpClient = new CTcpClient( ioThread->m_io_service, endpoint_iterator );
    ioThread->start();
    Sleep( 1000 );
    //m_t = std::thread ([&io_service1](){ io_service1.run(); qDebug() << "quit"; });	
    m_isConnect = CMessageQueue::getInstance()->GetConnectState();

    if( !m_isConnect )
    {
        ioThread->StopThread();
        ioThread->wait();
        delete ioThread;
	
    }
    else
    {
        m_ioserviceThread = ioThread;
		emit ServierConnect();
		
    }
    if( isInit )
    {
        //
    }
    else
    {
        if( CMessageQueue::getInstance()->GetConnectState() )
        {
            m_isSendMsg = true;
            CMessageLogin *login = new CMessageLogin();
            login->m_qstrAccount = struser;
            login->m_qstrPassword = strPwd;

            CMessage * message = new CMessage();
            message->m_header.m_struct.cmd = PC_CLIENT_CMD_LOGIN;
            message->m_header.m_struct.messageNo = 1;
            message->m_header.m_struct.cmdResult = 1;
            message->SetMessageBody(login);
            SendMessage(message);
        }	
    }
    m_mutex.unlock();
}
