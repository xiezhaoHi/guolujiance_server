#ifndef QDIALWIDGET_H
#define QDIALWIDGET_H

#include <QWidget>
#include <qwt_dial.h>
#include <QLabel>
#include <QLCDNumber>
#include "qshow_struct.h"



class QDialWidget : public QWidget
{
	Q_OBJECT

public:
	QDialWidget( QWidget *parent = 0);
	~QDialWidget();
	//void setInfo(PANNEL_DATA st );
	void setDialParam(double max=10 );
	void UpdateValue(double f);
	private slots:
		void setNum(double);

private:
	QwtDial *m_dial;
	QLabel  *m_lb_ID;;
	QLabel  *m_lb_name;
	QLabel  *m_lb_probe;
	QLCDNumber *m_lcd_value;
};

#endif // QDIALWIDGET_H
