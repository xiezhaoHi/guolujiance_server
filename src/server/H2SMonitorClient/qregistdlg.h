#ifndef QREGISTDLG_H
#define QREGISTDLG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton >
#include "yxyDES2.h"

class QRegistDlg : public QDialog
{
	Q_OBJECT

public:
	QRegistDlg(QString diskid, QWidget *parent = 0);
	~QRegistDlg();
	
	QString m_diskid;
	QString m_code;
private:
	static int m_createNum;
	QLineEdit *m_LocalHost, *m_RegistCode;
	QPushButton *m_btn_regist;


	private slots:
		void RegistSlot();
};

#endif // QREGISTDLG_H
