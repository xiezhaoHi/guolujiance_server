#ifndef QRUNSTATEINFO_WIDGET_H
#define QRUNSTATEINFO_WIDGET_H

#include <QWidget>
#include <QTableView>
#include <qstateinfomodel.h>

class QRunStateInfo_Widget : public QWidget
{
	Q_OBJECT

public:
	QRunStateInfo_Widget(QWidget *parent = 0);
	~QRunStateInfo_Widget();
	void setMsg(QString msg);

private:
	QTableView *m_view;
	QStateInfoModel *m_model;

};

#endif // QRUNSTATEINFO_WIDGET_H
