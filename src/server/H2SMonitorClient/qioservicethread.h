#ifndef QIOSERVICETHREAD_H
#define QIOSERVICETHREAD_H

#include <QThread>
#include <boost/asio.hpp>
#include "tcp_client.h"
#include "protocol/protocol_cmd.h"
#include "message/message.h"
#include "message/message_login.h"
#include "message_queue.h"
#include "util/global_util.h"

class QIOServiceThread : public QThread
{
	Q_OBJECT

public:
	QIOServiceThread(QObject *parent= 0 );
	~QIOServiceThread();
	void run();
	
	static QIOServiceThread *getInstance();
	boost::asio::io_service m_io_service;
	std::shared_ptr<boost::asio::io_service::work> m_work;  
	void StopThread();

private:
	static QIOServiceThread* m_instance;
	
};

#endif // QIOSERVICETHREAD_H
