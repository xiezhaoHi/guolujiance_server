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
#include <QString>
#include <QQueue>
#include <QSettings>
#include <QFile>
#include <QtWidgets/QApplication>
#include "message/message.h"
#include <QMutex>
#include "util\sync_cond_queue.h"
typedef CSyncCondQueue<CMessage*> session_message_queue;
class CMessageQueue
{
	
public:
	static CMessageQueue* getInstance();
	bool init();
	CMessageQueue(void);
	~CMessageQueue(void);
	//session_message_queue GetRecMessageQueue();
	bool isEmpty();
	bool RecQueuePushBack(CMessage * pMessage);
	bool wait(unsigned long timeout);
	bool GetConnectState();
	void setConnectState(bool states);
	CMessage*  RecQueuePopFront();

	session_message_queue       rec_msgs_queue;

protected:
	QMutex m_mutex;
private:
	bool isConnect;
	session_message_queue       sys_msgs_queue;
	session_message_queue       data_msgs_queue;
	static int m_count;

};

