#ifndef QCHECKRECVDATAQUEUETHREAD_H
#define QCHECKRECVDATAQUEUETHREAD_H

#include <QThread>
#include "qmessagethread.h"

class QCheckRecvDataQueueThread : public QThread
{
	Q_OBJECT

public:
	QCheckRecvDataQueueThread(QObject *parent = 0);
	~QCheckRecvDataQueueThread();
	void run();
	static QCheckRecvDataQueueThread* GetInstance();
	void SetMessageThread(QMessageThread *threads);
	void QuitThread();
	void setRecvData( bool isrecv );
private:
	static QCheckRecvDataQueueThread *m_instance;
	QMessageThread *m_messageThread;
	bool m_flag;
	bool m_isRecv;
	int sendCount;
signals:
	
	void ServerRepData( const CMessage *);
	
};

#endif // QCHECKRECVDATAQUEUETHREAD_H
