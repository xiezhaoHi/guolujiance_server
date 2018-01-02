#ifndef QPHOTOSHOWWIDGET_H
#define QPHOTOSHOWWIDGET_H

#include <QWidget>
#include "qwt_plot_curve.h"
#include "qwt_scale_map.h"
#include "qwt_plot.h"
#include <qwt_scale_draw.h>
#include <QPushButton>
#include <QLabel>

#include "qshow_struct.h"
#include <QDateTime>
#include "qshow_struct.h"
#define POINT_COUNT 180
//const int Size = 180;
//const int CurvCnt = 6;




class QPhotoShowWidget : public QWidget
{
	Q_OBJECT

public:
	QPhotoShowWidget(QWidget *parent = 0);
	~QPhotoShowWidget();
	void resizeEvent(QResizeEvent *e);
	void setCurvcnt(QString str, int icount ,QVector<BUTTON_INSTRUMENT> &vt_btinfo );
	void setHideChangeDevice();
	void UpdateCurvcnt(double yvalue, int index,double times);
	void InitICurves( int icount );
	void ShowSpline(QString str, QVector<XY_VALUE> vt_info );
private:
	QPushButton *m_bt_changeDevice;
	QLabel *m_currentDevice;
	QwtPlotCurve *d_curves;
	QwtPlot *m_plot;
	int m_iCurvcnt;
	QVector<double > m_xvalue;
	QVector<QVector<double> > m_yvalue;
	QStringList m_vxList;
	QVector<XY_VALUE> m_data;
	double m_xmax, m_xmin;
	private slots:
		void ChangeDeviceSlot();
signals:
	void clicked();
};

#endif // QPHOTOSHOWWIDGET_H
