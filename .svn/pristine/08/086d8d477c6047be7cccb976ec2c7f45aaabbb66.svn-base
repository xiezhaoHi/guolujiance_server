#ifndef QPASSWD_DLG_H
#define QPASSWD_DLG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>

class QPasswd_dlg : public QDialog
{
	Q_OBJECT

public:
	QPasswd_dlg(QWidget *parent = 0);
	~QPasswd_dlg();

private:
	QPushButton *m_bt_sure, *m_bt_cancel;
	QLineEdit   *m_lne_user, *m_lne_pwd;
	static int m_irecord;
	private slots:
		void CancelSlot();
		void SureSlot();

};

#endif // QPASSWD_DLG_H
