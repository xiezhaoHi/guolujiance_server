/*****************************************************
** 文件名：qcustombutton.h
** 版 权：XXX 版权所有
** 版 本：
** 内容简述：按钮界面头文件
** 创建日期：2015年7月
** 创建人：荣永
** 修改记录：
日期        版本      修改人    修改内容   
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
