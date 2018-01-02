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

