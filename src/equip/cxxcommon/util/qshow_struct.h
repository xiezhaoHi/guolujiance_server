#ifndef _QSHOW_STRUCT_H_
#define _QSHOW_STRUCT_H_
#include "qstatebutton.h"
#include<QPushButton>
#include <qwt_scale_draw.h>
#include <QDateTime>
typedef struct T_BUTTON_INSTRUMENT
{
	T_BUTTON_INSTRUMENT()
	{
		states = NULL;
		btn = NULL;
		value1 = 0.0;	
		value2 = 0.0;
		longitude1 = 0.0;
		longitude2 = 0.0;
		latitude1 = 0.0;
		latitude2 = 0.0;
	}
	QStateButton *states;
	QPushButton *btn;
	QString deviceno;
	QString deviceid;
	QString deviceName;
	QString devType;
	bool flag;
	bool isSelect;
	double value1;
	double value2;
	double longitude1;
	double latitude1;
	double longitude2;
	double latitude2;
	QString types1;
	QString types2;
	QString datetimes1;
	QString datetimes2;
	QString warnLine1st;
	QString warnLine2nd;
	QString maxRange;
	QString units1;
	QString units2;
	QString sample1;
	QString sample2;
	QDateTime recvTime;
}BUTTON_INSTRUMENT;

typedef struct T_XY_VALUE
{
	QVector<double> x_value;
	QVector<double> y_value;
}XY_VALUE;

typedef struct T_DATA_INFO
{
	QString name;
	QString id;
	QString times;
	QString types;
	QString values;
}DATA_INFO;

typedef struct T_PANNEL_DATA
{
	int probe;
	QString deviceid;
	QString types;
	QString times;
	QString units;
	QString value;
	QString battery;
}PANNEL_DATA;

typedef struct T_EXPORT_IMAGE_INFO
{
	QString deviceName;
	QString deviceID;
	QString dates;
	XY_VALUE datas_H2S;
	XY_VALUE datas_SO2;
}EXPORT_IMAGE_INFO;

typedef struct T_DATA_INFO_ST
{
	QVector<DATA_INFO*> datainfo;
} DATA_INFO_ST;
class TimeScaleDraw: public QwtScaleDraw
{
public:  
	TimeScaleDraw( )
	{  
		// 设置坐标轴刻度线的长度
		setTickLength( QwtScaleDiv::MinorTick, 0 );  
		setTickLength( QwtScaleDiv::MediumTick, 0 );  
		setTickLength( QwtScaleDiv::MajorTick, 10 );  

		//不显示刻度线坐落的部件
		enableComponent( QwtScaleDraw::Backbone, false );  
		//标签显示的样式
		setLabelAlignment( Qt::AlignLeft  | Qt::AlignVCenter);  
	}  

	//刻度标签值  
	virtual QwtText label( double value) const  
	{  
		QwtText lbl;  
		lbl = QDateTime::fromMSecsSinceEpoch( value ).toString("hh:mm:ss");
		return lbl;  
	}  
};

#endif