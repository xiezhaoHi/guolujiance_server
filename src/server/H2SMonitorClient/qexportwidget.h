#ifndef QEXPORTWIDGET_H
#define QEXPORTWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QDateEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QResizeEvent>
#include <QComboBox>
#include "qdataexportthread.h"
#include <QSqlDatabase>
#include "qprocesswidget.h"
#include <QProgressBar>
#include <QCheckBox>
#include "qwt_plot_curve.h"
#include "qwt_scale_map.h"
#include "qwt_plot.h"
#include <qwt_scale_draw.h>
#include <QLineEdit>
#include "qmessagethread.h"
#include "qcheckrecvloaddataqueuethread.h"
#include "qremovedatatobasethread.h"
#include "message/message_device_data_item.h"
#include "message/message_get_device_data_count_rep.h"
#include "message/message_get_device_data_count_req.h"
#include "message/message_get_device_data_rep.h"
#include "message/message_get_device_data_req.h"
#include "message/message_get_device_data_count_req.h"
#include "protocol/protocol_data_desc.h"
#include <QProgressBar>
#include "qwaitwidget.h"

class QExportWidget : public QWidget
{
	Q_OBJECT

public:
	QExportWidget(QSqlDatabase db, QWidget *parent = 0);
	~QExportWidget();
	void resizeEvent(QResizeEvent *);
	void setDeviceInfo( const QVector<BUTTON_INSTRUMENT> btn_instrument);
	void setMessageThread(QMessageThread *messageThread, QRemoveDataToBaseThread *removeThread);
private:
	QDateTimeEdit *m_Begin_time;
	QDateTimeEdit *m_End_time;
	QDateTimeEdit *m_load_StartTime, *m_load_StopTime;
	QLabel *m_lb_loadStart, *m_lb_loadStop;
	QPushButton *m_bt_export, *m_bt_load;
	QCheckBox *m_radio_image;	
	QLabel *m_lb_time, *m_lb_aveTm, *m_lb_units,*m_lb_endTime;
	QLabel *m_lb_local, *m_lb_server, *m_lb_device;
	QLineEdit *m_lne_aveTm;
	QComboBox *m_comb_deviceID;
	QDataExportThread *m_thread;
	QSqlDatabase m_db;
	QVector<DATA_INFO> m_dataInfo;
	QCheckBox *m_cbox_ave, *m_cbox_test;
	QVector<EXPORT_IMAGE_INFO> m_vt_testInfo;
	QVector<EXPORT_IMAGE_INFO> m_vt_aveInfo;
	QMessageThread *m_messageThread;
	QRemoveDataToBaseThread *m_removeThread;
	unsigned long m_DataCount;
	QVector<unsigned long > m_vt_count;
	int m_record;
	QDateTime m_dateStartTime;
	QDateTime m_dateEndTime;
	QVector<DATA_INFO*> m_vtbasedata;
	QVector<BUTTON_INSTRUMENT> m_btn_instrument;
	QString m_loadDeviceID;
	QLabel *m_lb_loadCount;
	QProgressBar *m_loadProgress;
	bool m_progressIsShow;
	QWaitWidget *m_waitDlg;

	QwtPlot *InitQwtPlot();
	void ExportToImage(QVector<EXPORT_IMAGE_INFO> &vt, bool isave = false);
	private slots:
		void ExportDataSlot();
		void GetQueryDataInfo();
		void ExportTestImageSlot();
		void ExportAveImageSlot();
		void AveCheckBoxStateChange(int);
		void LoadServerDataSlot();
		void ServerRepDataCount(CMessage*);
		void RecvServerLoadData(CMessage*);
		void SaveLoadDataCountSlot(int);
signals:
		void BeginExport(QString );
		void ExportCompelete(int i);
		void BeginLoadData(QString);

};

#endif // QEXPORTWIDGET_H
