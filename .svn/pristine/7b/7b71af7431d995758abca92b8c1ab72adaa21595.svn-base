#ifndef QPARAMSETWIDGET_H
#define QPARAMSETWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QRadioButton>
#include <QPaintEvent>
#include "qinit_configuration.h"

typedef struct T_PARAM_SET
{
	QString code;
	QString name;
	QString types;
	QString warn_one;
	QString warn_two;
	QString maxs;
	QString sample;
}PARAM_SET;
typedef enum E_WARN_TYPE
{
	AVE_VALUE_WARN,
	TEST_VALUE_WARN,
}WARN_TYPE;

class QParamSetWidget : public QWidget
{
	Q_OBJECT

public:
	QParamSetWidget(QWidget *parent = 0);
	~QParamSetWidget();
	void InitInfo(PARAM_SET st);

private:
	void InitWidget();
	void CleanParam();
	void paintEvent(QPaintEvent *);
	QPushButton *m_pb_nextDevice, *m_pb_saveParam, *m_pb_warnParam;
	QLabel *m_lb_code, *m_lb_name, *m_lb_type;
	QLineEdit *m_probe1_one_warn, *m_probe1_two_warn, *m_probe1_max_value;
	QLineEdit *m_probe2_one_warn, *m_probe2_two_warn, *m_probe2_max_value;
	QLineEdit *m_lne_Name_set, *m_lne_sample1, *m_lne_sampl2,*m_lne_ave_time;
	QRadioButton *m_radio_ave, *m_radio_test;
	QLabel *m_lb_sample1, *m_lb_sample2;
	QLabel *m_lb_ave_time;
	QLabel *m_lb_ave_unit;
	private slots:
		void NextDeviceSlot();
		void SaveParamSlot();
		void SaveWarnSlot();
		void AveWarnClickSlot(bool);
signals:
		void clicked();
		void SaveParamSignal(QString);
		void SaveWarnParam( int , int );
};

#endif // QPARAMSETWIDGET_H
