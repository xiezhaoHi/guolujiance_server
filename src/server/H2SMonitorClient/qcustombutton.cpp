
/*****************************************************
** 文件名：qcustombutton.cpp
** 版 权：XXX 版权所有
** 版 本：
** 内容简述：按钮界面实现文件
** 创建日期：2015年5月
** 创建人：荣永
** 修改记录：
日期        版本      修改人    修改内容   
*****************************************************/ 
#include "qcustombutton.h"
#include  <QPainter>
#include <QLabel>

QCustomButton::QCustomButton(QImage imgState ,QWidget *parent)
	: QPushButton(parent)
{
	m_isNormal = false;
	m_isWarning = false;
	this->m_bkImage = imgState;
	m_normal.load("./Resources/images/warning/normal.png");
	
	resize( 40, 40 );
}


QCustomButton::~QCustomButton()
{
	
}


/******************************************************
** 函数名：paintEvent
** 功能简述:重载重绘函数
** 输入参数: 
** bool bCheck状态
** 返回值: 
**  创建日期：2015年7月
** 创建人：荣永
** 修改记录：
日期        版本      修改人    修改内容
** 其它说明：
******************************************************/
void QCustomButton::paintEvent(QPaintEvent *pEvent)
{
	QPushButton::paintEvent(pEvent);
	QPainter painter(this);
	QBrush qb(QColor::fromRgb(0, 0, 0));
	if( !m_isNormal && !m_isWarning )
	{
		painter.drawImage(QRect(0,0, this->size().width(), this->size().height()), 
			m_bkImage, QRect(0,0, m_bkImage.width(), m_bkImage.height()));	
	}
	else if( m_isWarning )
	{
		painter.drawImage(QRect(0,0, this->size().width(), this->size().height()), 
			m_bkImage, QRect(0,0, m_bkImage.width(), m_bkImage.height()));	
	}
	else if( m_isNormal )
	{
		painter.drawImage(QRect(0,0, this->size().width(), this->size().height()), 
			m_normal, QRect(0,0, m_normal.width(), m_normal.height()));	
	}
	
}
void QCustomButton::setWaraning()
{

	m_isWarning = true;
	m_isNormal = false;
	update();
}
void QCustomButton::setNormal()
{
	m_isNormal = true;
	m_isWarning = false;
	update();
}