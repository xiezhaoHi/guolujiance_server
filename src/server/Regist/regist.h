#ifndef REGIST_H
#define REGIST_H

#include <QtWidgets/QMainWindow>
#include <QLineEdit>
#include <QPushButton >
#include "yxyDES2.h"


class Regist : public QMainWindow
{
	Q_OBJECT

public:
	Regist(QWidget *parent = 0);
	~Regist();

private:
	QLineEdit *m_LocalHost, *m_RegistCode;
	QPushButton *m_btn_regist;

	private slots:
	void RegistSlot();
};

#endif // REGIST_H
