#ifndef QCHECKRECVLOADDATAQUEUETHREAD_H
#define QCHECKRECVLOADDATAQUEUETHREAD_H

#include <QThread>
#include "qmessagethread.h"

class QCheckRecvLoadDataQueueThread : public QThread
{
	Q_OBJECT

public:
	QCheckRecvLoadDataQueueThread(QObject *parent=0);
	~QCheckRecvLoadDataQueueThread();

	void run();
	static QCheckRecvLoadDataQueueThread* GetInstance();
	void SetMessageThread(QMessageThread *threads);
	void QuitThread();
private:
	static QCheckRecvLoadDataQueueThread *m_instance;
	QMessageThread *m_messageThread;
	bool m_flag;

signals:
	void ServerRepData(CMessage* );
	
};

#endif // QCHECKRECVLOADDATAQUEUETHREAD_H
