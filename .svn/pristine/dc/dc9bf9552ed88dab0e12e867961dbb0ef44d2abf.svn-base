#include "qwaitwidget.h"
#include <QLabel>
#include <QMovie>

QWaitWidget::QWaitWidget(QWidget *parent)
	: QDialog(parent,Qt::CustomizeWindowHint | Qt::FramelessWindowHint)
{
	resize( 120, 120 );
	setAttribute(Qt::WA_TranslucentBackground, true );  //ÉèÖÃÊôÐÔÍ¸Ã÷
	QLabel *lb_move = new QLabel( this );
	m_movie = new QMovie(":/login/images/login/loading3.gif");
	lb_move->setMovie( m_movie );
	m_movie->start();
	lb_move->setGeometry(20, 20, 86, 86 );
}

QWaitWidget::~QWaitWidget()
{
	if( m_movie )
	{
		delete m_movie;
		m_movie = NULL;
	}
}
