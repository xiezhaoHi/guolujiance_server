#include "qstatebutton.h"
#include <QPainter>
QStateButton::QStateButton(QImage mage1,QImage mage2, QWidget *parent)
	: QWidget(parent)
{
	m_image1 = mage1;
	m_image2 = mage2;
}

QStateButton::~QStateButton()
{

}
void QStateButton::setStateEnable(bool flages )
{
	m_isEnable = flages;
	update();
}
void QStateButton::paintEvent(QPaintEvent *event )
{
	QPainter painter(this);
	QBrush qb(QColor::fromRgb(0, 0, 0));
	if( m_isEnable )
	{
		painter.drawImage(QRect(0,0, this->size().width(), this->size().height()), 
			m_image1, QRect(0,0, m_image1.width(), m_image1.height()));	
	}
	else
	{
		painter.drawImage(QRect(0,0, this->size().width(), this->size().height()), 
			m_image2, QRect(0,0, m_image2.width(), m_image2.height()));	
	}
	
}
void QStateButton::mouseReleaseEvent(QMouseEvent *e)
{
	emit clicked();
}
