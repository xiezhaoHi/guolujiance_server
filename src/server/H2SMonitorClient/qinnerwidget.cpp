#include "qinnerwidget.h"
#include <QMouseEvent>
#include <QDebug>
QInnerWidget::QInnerWidget(QWidget *parent)
	: QWidget(parent)
{
	
}

QInnerWidget::~QInnerWidget()
{
	qDebug() << "QInnerWidget release";
}
void QInnerWidget::mouseMoveEvent(QMouseEvent *e)
{
	emit mouseMove(e->globalPos());
}
void QInnerWidget::mousePressEvent(QMouseEvent *e)
{
	emit mouseDown(e->globalPos());

}
void QInnerWidget::mouseReleaseEvent(QMouseEvent *e)
{
	emit mouseUp(e->globalPos());
}