#ifndef QQUERYWARNTHREAD_H
#define QQUERYWARNTHREAD_H

#include <QThread>
#include <QSqlDatabase>
#include <QMutex>
#include <QStringList>
class QQueryWarnThread : public QThread
{
	Q_OBJECT

public:
	QQueryWarnThread(QSqlDatabase db, QObject *parent);
	~QQueryWarnThread();
	
	void getQueryMsg(QStringList &strList);

private:
	void run();
	QSqlDatabase m_db;
	QMutex m_mutex;
	QStringList m_strList;
signals:
	void SendQueryMsg( );
};

#endif // QQUERYWARNTHREAD_H
