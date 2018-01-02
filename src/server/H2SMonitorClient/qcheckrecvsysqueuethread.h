#ifndef QCHECKRECVSYSQUEUETHREAD_H
#define QCHECKRECVSYSQUEUETHREAD_H

#include <QThread>
#include "qmessagethread.h"

class QCheckRecvQueueThread : public QThread
{
	Q_OBJECT

public:
	QCheckRecvQueueThread(QObject *parent=0);
	~QCheckRecvQueueThread();
	void run();
	static QCheckRecvQueueThread* GetInstance();
	void SetMessageThread(QMessageThread *threads);
	void QuitThread();
	void setRecvData( bool isrecv );
private:
	static QCheckRecvQueueThread *m_instance;
	QMessageThread *m_messageThread;
	bool m_flag;
	bool m_isRecv;
signals:
	void ServerRepInfo(CMessage* );
	//void ServerRepData(CMessage* );
};

#endif // QCHECKRECVQUEUETHREAD_H
