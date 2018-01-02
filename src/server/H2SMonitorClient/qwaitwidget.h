#ifndef QWAITWIDGET_H
#define QWAITWIDGET_H

#include <QDialog>


class QWaitWidget : public QDialog
{
	Q_OBJECT

public:
	QWaitWidget(QWidget *parent = 0);
	~QWaitWidget();
	QMovie *m_movie ;
};

#endif // QWAITWIDGET_H
