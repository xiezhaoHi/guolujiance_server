#ifndef QALLWARNINFOWIDGET_H
#define QALLWARNINFOWIDGET_H

#include <QWidget>
#include <QTableView>
#include "allwarnmsgmodel.h"
#include <QResizeEvent>
#include <QSqlDatabase>
#include "qquerywarnthread.h"
class QAllWarnInfoWidget : public QWidget
{
	Q_OBJECT

public:
	QAllWarnInfoWidget(QSqlDatabase db,QWidget *parent = 0);
	~QAllWarnInfoWidget();
	void BeginQueryMsg();
	void SetBufferMsg(QStringList strlist1,int i);
	public slots:
		void GetQueryMsg();

private:
	AllWarnMsgModel *m_model;
	QTableView *m_view;
	QSqlDatabase m_db;
	void resizeEvent(QResizeEvent *);
	QQueryWarnThread *m_thread;
	QStringList m_strFirstList;
	QStringList m_strSecondList;
signals:
	void QueryMsgOver();
};

#endif // QALLWARNINFOWIDGET_H
