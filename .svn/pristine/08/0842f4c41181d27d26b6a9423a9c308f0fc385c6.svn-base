#ifndef QDEVICE_COLLECT_DLG_H
#define QDEVICE_COLLECT_DLG_H

#include <QDialog>
#include <QLabel>
#include "qcustombutton.h"
#include <QLineEdit>
#include <QtSerialPort/QSerialPort>
#include <QPaintEvent>

class QDevice_Collect_dlg : public QDialog
{
	Q_OBJECT

public:
	QDevice_Collect_dlg(QWidget *parent = 0);
	~QDevice_Collect_dlg();
	void setCommId(QString id);
	private slots:
		void H2StoZeroSlot();
		void H2StoFullSlot();
		void SO2toZeroSlot();
		void SO2toFullSlot();
		void ReadCommInfo();

private:
	static int m_irecord;
	QString m_commid;
	QLabel *m_lb_id;
	QCustomButton *m_bt_Zero_h2s, *m_bt_Zero_so2, *m_bt_full_h2s, *m_bt_full_so2, *m_bt_close;
	QSerialPort *m_port;
	void setZeroCollect();
	void setFullCollect();
	void paintEvent(QPaintEvent *);
};

#endif // QDEVICE_COLLECT_DLG_H
