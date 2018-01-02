/*****************************************************
** �ļ�����qcustombutton.h
** �� Ȩ��XXX ��Ȩ����
** �� ����
** ���ݼ�������ť����ͷ�ļ�
** �������ڣ�2015��7��
** �����ˣ�����
** �޸ļ�¼��
����        �汾      �޸���    �޸�����   
*****************************************************/ 
#ifndef QCUSTOMBUTTON_H
#define QCUSTOMBUTTON_H

#include <QWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QMovie>

class QCustomButton : public QPushButton
{
	Q_OBJECT

public:
	QCustomButton(QImage imgState1, QWidget *parent = 0);
	~QCustomButton();
	void setWaraning();
	void setNormal();

private:
	QImage m_bkImage;
	QImage m_normal;
	void paintEvent(QPaintEvent *pEvent);
	bool m_isNormal;
	bool m_isWarning;
	

};

#endif // QCUSTOMBUTTON_H
