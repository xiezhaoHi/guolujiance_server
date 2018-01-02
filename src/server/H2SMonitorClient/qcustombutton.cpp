
/*****************************************************
** �ļ�����qcustombutton.cpp
** �� Ȩ��XXX ��Ȩ����
** �� ����
** ���ݼ�������ť����ʵ���ļ�
** �������ڣ�2015��5��
** �����ˣ�����
** �޸ļ�¼��
����        �汾      �޸���    �޸�����   
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
** ��������paintEvent
** ���ܼ���:�����ػ溯��
** �������: 
** bool bCheck״̬
** ����ֵ: 
**  �������ڣ�2015��7��
** �����ˣ�����
** �޸ļ�¼��
����        �汾      �޸���    �޸�����
** ����˵����
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