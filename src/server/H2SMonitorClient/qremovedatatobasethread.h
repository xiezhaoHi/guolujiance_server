#ifndef QREMOVEDATATOBASETHREAD_H
#define QREMOVEDATATOBASETHREAD_H

#include <QThread>
#include <QSqlDatabase>
#include <QVector>
#include <QMutex>
#include "qshow_struct.h"
#include "util\sync_cond_queue.h"
typedef CSyncCondQueue<DATA_INFO_ST> QueueDataInfo;

class QRemoveDataToBaseThread : public QThread
{
	Q_OBJECT

public:
	QRemoveDataToBaseThread(QSqlDatabase db, QObject *parent);
	~QRemoveDataToBaseThread();
	void setDataInfo(QVector<DATA_INFO*> datas , bool isLoad = false);
	void setQuitFlag();
	//QueueDataInfo GetQueueDataInfo();

private:
	void run();
	QSqlDatabase m_db;
	QVector<DATA_INFO*> m_data;	
	QueueDataInfo m_queue;
	QueueDataInfo m_loadqueue;
	bool m_isavedata;
	QMutex m_mutex;
	static int m_irecord;
	bool m_iscontiue;
signals:
	void SaveCommpelete();
	void SaveLoadDataCount(int);
};

#endif // QREMOVEDATATOBASETHREAD_H
