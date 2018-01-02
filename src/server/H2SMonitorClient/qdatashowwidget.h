#ifndef QDATASHOWWIDGET_H
#define QDATASHOWWIDGET_H
#include <QDateTime>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QPaintEvent>
#include <qdialwidget.h>
#include <QTableView>
#include "qcheckdatamodel.h"
#include <QResizeEvent>
#include "qshow_struct.h"
#include <QLineEdit>
#include "qquerydatathread.h"
#include <QSqlDatabase>

typedef struct T_DATA_SHOW_INFO
{
	double value;
	QString datetime;
	QString types;
	QString latitude;
	QString longitude;
	QString units;
}DATA_SHOW_INFO;

class QDataShowWidget : public QWidget
{
	Q_OBJECT

public:
	QDataShowWidget(QVector<DATA_SHOW_INFO> info, QWidget *parent = 0);
	~QDataShowWidget();
	void setDatabase(QSqlDatabase db);
	void setInfo(QVector<DATA_SHOW_INFO> &info ,XY_VALUE &st1, XY_VALUE &st2, XY_VALUE &ave1, XY_VALUE &ave2);
	void setCurrentDevice( QString deivce);
	void setDialMaxRange(double range1 = 10, double range2 = 10);
	void UpdateInfo(int index, double dates, double data, bool isave=false);
private:
	QPushButton *m_pb_Changedevice, *m_pb_aveTm;
	QLabel *m_deviceName;
	QVector<DATA_SHOW_INFO> m_info;
	QLabel *m_unit1, *m_unit2, *m_unit_ave1, *m_unit_ave2, *m_lb_aveTm, *m_lb_minites;
//	QwtDial  *m_dialOne, *m_dialTwo;
	QLabel *m_currentIntrument;
//	QLabel *m_lb_upData, *m_lb_downData;
//	QLabel *m_lb_times2, *m_lb_times;
	QLabel *m_lb_datatype, *m_lb_datatype2, *m_datatype_ave1, *m_datatype_ave2;
//	QLabel *m_lb_longitudes, *m_lb_longitudes2;
//	QLabel *m_lb_latitudes , *m_lb_latitudes2;
	QTableView *m_view1, *m_view2, *m_view_ave1, *m_view_ave2;
	QCheckDataModel *m_model1, *m_model2, *m_model_ave1, *m_model_ave2;
	QLineEdit *m_lne_aveTm;
	XY_VALUE  m_dataprobe1;
	XY_VALUE  m_dataprobe2;
	QSqlDatabase m_db;
	int m_sample1, m_sample2;
	int m_icount1, m_icount2;
	int m_aveTime;

	bool m_isCalculate;
	QQueryDataThread *m_threads;
	XY_VALUE m_aveCalculate;     //计算平均值的容器；
	void paintEvent(QPaintEvent *);
	void resizeEvent(QResizeEvent *);
	void UpdateOneModel( QCheckDataModel *model, double value, double times);
	void AveCalculateRecv(XY_VALUE &st, QCheckDataModel *model);
	private slots:
		void ChangeDeviceSlot();
		void AveCalculateSlot();
		void GetAveDataSlot();
signals:
		void clicked();
		void BeginQueryData();
		void QueryDataOver();
};

#endif // QDATASHOWWIDGET_H
