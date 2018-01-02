/*****************************************************
**  �� �� ����tcp_logic.h
**  �� Ȩ��XXX ��Ȩ����
**  ��    ����Ver.1.0
**  ���ݼ�����ͨѶ�߼���ͷ�ļ�
**
**  �������ڣ�2015/07/22 
**  �����ˣ����
**  �޸ļ�¼��
    ����        �汾      �޸���    �޸�����   
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

