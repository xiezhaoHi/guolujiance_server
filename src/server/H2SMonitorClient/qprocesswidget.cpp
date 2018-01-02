#include "qprocesswidget.h"

QProcessWidget::QProcessWidget(QWidget *parent)
	: QDialog(parent,Qt::CustomizeWindowHint | Qt::FramelessWindowHint)
{
	m_progress = new QProgressBar( this );
	resize(250, 100);
	m_progress->setMaximum( 100 );

	m_progress->setGeometry(10, 40, 230, 20);
	m_progress->setStyleSheet("QProgressBar{border:none;background:rgb(210, 225, 240);border-radius:3px;text-align:center;}QProgressBar::chunk {background:rgb(60, 140, 220);border-radius:3px;}");
}

QProcessWidget::~QProcessWidget()
{

}
void QProcessWidget::setProgressValue(int v)
{
	m_progress->setValue(v);
}
