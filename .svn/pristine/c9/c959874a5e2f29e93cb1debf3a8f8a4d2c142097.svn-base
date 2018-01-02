#ifndef QINNERWIDGET_H
#define QINNERWIDGET_H

#include <QWidget>

class QInnerWidget : public QWidget
{
	Q_OBJECT

public:
	QInnerWidget(QWidget *parent = 0);
	~QInnerWidget();
	void mousePressEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *);
signals:
	void mouseDown(QPoint mosuePos);
	void mouseUp(QPoint mosuePos);
	void mouseMove(QPoint mosuePos);
	
};

#endif // QINNERWIDGET_H
