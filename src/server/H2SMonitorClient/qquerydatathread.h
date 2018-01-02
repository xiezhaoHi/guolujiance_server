#ifndef QQUERYDATATHREAD_H
#define QQUERYDATATHREAD_H

#include <QThread>
#include <QSqlDatabase>
#include "qshow_struct.h"
#include "util/global_util.h"



class QQueryDataThread : public QThread
{
	Q_OBJECT

public:
	QQueryDataThread(QObject *parent);
	~QQueryDataThread();
	void setQueryFactor(QString strid, int avetime,QSqlDatabase db);
	void GetQueryResult(XY_VALUE &st1, XY_VALUE &st2 );
//	void AveCalculator(XY_VALUE ProbeValue, int aveTime, XY_VALUE &out);

private:
	void run();
	QSqlDatabase m_db;
	XY_VALUE m_probe1;
	XY_VALUE m_probe2;
	XY_VALUE m_aveprobe1;
	XY_VALUE m_aveprobe2;
	QMutex m_mutex;
	QString m_id;
	int m_aveTime;
signals:
	void ThreadOver();
};

#endif // QQUERYDATATHREAD_H
