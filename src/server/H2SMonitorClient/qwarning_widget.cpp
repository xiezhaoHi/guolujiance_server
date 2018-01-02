#include "qwarning_widget.h"
#include <QPainter>
#include <QLabel>
#include <QDebug>

QWarning_Widget::QWarning_Widget(WARN_STATE states, QWidget *parent)
	:m_state( states),QWidget(parent)
{
	
	m_mv_warn1st = new QMovie(":/warning/images/warning/warn_1st.gif");
	m_mv_warn2nd = new QMovie(":/warning/images/warning/warn_2nd.gif");
	m_lb_mv = new QLabel( this );
	if( m_state == WARNING_2ND_STATE)
	{
		m_lb_mv->setMovie( m_mv_warn2nd );
	}
	else if( m_state == WARNING_1ST_STATE )
	{
		m_lb_mv->setMovie( m_mv_warn1st );
	}	
	m_lb_mv->setGeometry(12, 0, 40, 40 );
	m_lb_mv->hide();
}

QWarning_Widget::~QWarning_Widget()
{
	qDebug() << "QWarning_Widget release";
}
void QWarning_Widget::set1stWarning()
{
	m_isSendSignal = true;
	m_lb_mv->show();
	m_mv_warn1st->start();
	
}
void QWarning_Widget::set2ndWarning()
{
	m_isSendSignal = true;
	m_lb_mv->show();
	m_mv_warn2nd->start();
	
}
void QWarning_Widget::setNormal()
{
	m_isSendSignal = false;
	m_lb_mv->hide();
	if( m_state == WARNING_1ST_STATE )
		m_mv_warn1st->stop();
	else if(m_state == WARNING_2ND_STATE )
		m_mv_warn2nd->stop();	
}
void QWarning_Widget::mouseReleaseEvent(QMouseEvent *e)
{
	if( m_isSendSignal )
		emit clicked();
}
