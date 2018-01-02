/*****************************************************
**  文 件 名：tcp_logic.h
**  版 权：XXX 版权所有
**  版    本：Ver.1.0
**  内容简述：通讯逻辑类头文件
**
**  创建日期：2015/07/22 
**  创建人：杨恒
**  修改记录：
    日期        版本      修改人    修改内容   
*****************************************************/

#pragma once
#include <thread>
#include <boost/asio.hpp>
#include "tcp_client.h"
#include "protocol/protocol_cmd.h"
#include "message/message.h"
#include "message/message_login.h"
#include "util/global_util.h"
#include <QString>
#include <QQueue>
#include <QSettings>
#include <QFile>
#include <QtWidgets/QApplication>

class CTcpManager:public QObject
{
	Q_OBJECT
public:
	static CTcpManager* getInstance();
	bool init();
	CTcpManager( QObject * parent = 0 );
	CTcpManager(void);
	~CTcpManager(void);
	void StartTcpServer();
	void StopTcpServer();
	bool SendMessage(CMessage *message);
protected:

	void timerEvent( QTimerEvent *event );

	int m_nTimerId;

private:
	CTcpClient *m_pTcpClient;
	QString serverIP;
	QString serverPort;
	session_message_queue       sys_msgs_queue;
	session_message_queue       data_msgs_queue;
};

