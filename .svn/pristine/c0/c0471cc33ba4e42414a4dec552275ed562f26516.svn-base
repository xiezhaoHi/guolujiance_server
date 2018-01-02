#ifndef QWT_DIAL_WIDGET_H
#define QWT_DIAL_WIDGET_H

#include <QWidget>
#include <qwt_dial.h>
#include <QLabel>
#include <QLCDNumber>
#include "qwt_dial_needle.h"
typedef struct T_PANNEL_DATA2
{
	double value;
	int probe;
	QString strName;
	QString strNo;
}PANNEL_DATA2;

class Qwt_Dial_Widget : public QWidget
{
	Q_OBJECT

public:
	Qwt_Dial_Widget(QWidget *parent = 0);
	~Qwt_Dial_Widget();
	void setInfo(PANNEL_DATA2 st );
	void setDialParam(double max=10 );
	void UpdateValue(double f);
	void deleteDial();
	private slots:
		void setNum(double);

private:
	QwtDial *m_dial;
	QLabel  *m_lb_ID;;
	QLabel  *m_lb_name;
	QLabel  *m_lb_probe;
	QLCDNumber *m_lcd_value;
	QwtCompassMagnetNeedle  *m_needle;
};

#endif // QWT_DIAL_WIDGET_H
