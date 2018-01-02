#include "qexportwidget.h"
#include <QFileDialog>
#include <QMessageBox>
#include <qwt_plot_renderer.h>
#include "qinit_configuration.h"

QExportWidget::QExportWidget(QSqlDatabase db, QWidget *parent)
	: m_db( db), QWidget(parent)
{
	m_progressIsShow = false;
	m_lb_local = new QLabel(QString::fromLocal8Bit("本地数据导出:"), this);
	m_lb_time = new QLabel(QString::fromLocal8Bit("开始时间:"), this );
	
	m_lb_endTime = new QLabel(QString::fromLocal8Bit("结束时间:"), this); 

	m_Begin_time = new QDateTimeEdit( this );
	m_Begin_time->setDisplayFormat("yyyy-MM-dd hh:mm:ss");
	m_Begin_time->setDateTime(QDateTime::currentDateTime());

	m_End_time = new QDateTimeEdit( this );
	m_End_time->setDisplayFormat("yyyy-MM-dd hh:mm:ss");
	m_End_time->setDateTime(QDateTime::currentDateTime());

	m_radio_image = new QCheckBox(QString::fromLocal8Bit("同时导出曲线图"), this );
	m_cbox_ave = new QCheckBox(QString::fromLocal8Bit("平均值导出"), this );
	m_cbox_test = new QCheckBox(QString::fromLocal8Bit("测量值导出"), this );


	m_bt_export = new QPushButton( QString::fromLocal8Bit("数据导出"), this );
	m_lb_aveTm = new QLabel( QString::fromLocal8Bit("平均时间："), this );
	m_lb_units = new QLabel(QString::fromLocal8Bit("分钟"), this );
	m_lne_aveTm = new QLineEdit("10", this );

	//resize( 700, 200);
	QRect rc = this->geometry();
	m_lb_local->setGeometry(10,rc.height()*0.1,150,30 );
	m_lb_time->setGeometry(10, rc.height()*0.2, 100, 30 );
	m_Begin_time->setGeometry(rc.width()*0.1, rc.height()*0.2, 150, 30 );
	m_lb_endTime->setGeometry(rc.width()*0.3,rc.height()*0.2, 100, 30);
	m_End_time->setGeometry(rc.width()*0.4, rc.height()*0.2, 150, 30 );
	m_radio_image->setGeometry( rc.width()*0.4, rc.height()*0.2+50, 120, 30 );
	m_cbox_ave->setGeometry(20, rc.height()*0.2+50, 100, 30 );
	m_cbox_test->setGeometry( rc.width()*0.2, rc.height()*0.2+50, 100, 30);
	m_lb_aveTm->setGeometry(20, rc.height()*0.2+100, 100, 30);
	m_lne_aveTm->setGeometry( rc.width()*0.1, rc.height()*0.2+100, 80, 30);
	m_lb_units->setGeometry( rc.width()*0.25, rc.height()*0.2+100, 50, 30);

	m_lb_aveTm->hide();
	m_lb_units->hide();
	m_lne_aveTm->hide();

	m_bt_export->setGeometry( rc.width()*0.5, rc.height()*0.4, 100, 40);
	m_lb_server = new QLabel(QString::fromLocal8Bit("服务器数据下载:"), this);

	m_lb_loadStart = new QLabel(QString::fromLocal8Bit("开始时间："), this );
	m_lb_loadStop = new QLabel(QString::fromLocal8Bit("结束时间："), this );
	m_load_StartTime = new QDateTimeEdit( this );
	m_load_StartTime->setDateTime(QDateTime::currentDateTime());
	m_load_StartTime->setDisplayFormat("yyyy-MM-dd hh:mm:ss");
	m_load_StopTime = new QDateTimeEdit( this );
	m_load_StopTime->setDisplayFormat("yyyy-MM-dd hh:mm:ss");
	m_load_StopTime->setDateTime(QDateTime::currentDateTime() );
	m_bt_load = new QPushButton(QString::fromLocal8Bit("开始下载"), this );
	m_comb_deviceID = new QComboBox(this);
	//m_comb_deviceID->setCurrentText("");
	m_lb_device = new QLabel(QString::fromLocal8Bit("设备ID:"), this);

	m_lb_loadCount = new QLabel( QString::fromLocal8Bit("数据下载进度："),this );
	m_lb_loadCount->hide();
	m_loadProgress = new QProgressBar( this );
	m_loadProgress->hide();
	m_loadProgress->setMinimum( 0 );

	connect( m_bt_export, SIGNAL(clicked()), this, SLOT(ExportDataSlot()));
	connect( m_bt_load, SIGNAL(clicked()), this, SLOT(LoadServerDataSlot()));
	m_thread = new QDataExportThread(m_db, this );
	connect( m_thread, SIGNAL(QueryDataCompelete()), this, SLOT(GetQueryDataInfo()));
	connect( m_thread, SIGNAL(ExportTestImage()), this, SLOT(ExportTestImageSlot()));
	connect( m_thread, SIGNAL(ExportAveImage()), this, SLOT(ExportAveImageSlot()));
	connect( m_cbox_ave, SIGNAL(stateChanged(int)), this, SLOT(AveCheckBoxStateChange(int)));
	QRegExp regexp("^[1-9][0-9]*$"); 
	m_lne_aveTm->setValidator(new QRegExpValidator(regexp, m_lne_aveTm));
	m_DataCount = 0;
	connect( QCheckRecvLoadDataQueueThread::GetInstance(), SIGNAL(ServerRepData(CMessage*)), this, SLOT(RecvServerLoadData(CMessage*)));
}

QExportWidget::~QExportWidget()
{
	if( QCheckRecvLoadDataQueueThread::GetInstance()->isRunning() )
	{
		QCheckRecvLoadDataQueueThread::GetInstance()->QuitThread();
		QCheckRecvLoadDataQueueThread::GetInstance()->wait();
	}
}
void QExportWidget::resizeEvent(QResizeEvent *e)
{
	QRect rc = this->geometry();
	m_lb_local->setGeometry(10,rc.height()*0.1,150,30 );
	m_lb_time->setGeometry(10, rc.height()*0.2, 100, 30 );
	m_Begin_time->setGeometry(rc.width()*0.1, rc.height()*0.2, 150, 30 );
	m_lb_endTime->setGeometry(rc.width()*0.3,rc.height()*0.2, 100, 30);
	m_End_time->setGeometry(rc.width()*0.4, rc.height()*0.2, 150, 30 );

	m_radio_image->setGeometry( rc.width()*0.4, rc.height()*0.2+50, 120, 30 );
	m_bt_export->setGeometry( rc.width()*0.5, rc.height()*0.4, 100, 40);
	m_cbox_ave->setGeometry(20, rc.height()*0.2+50, 100, 30 );
	m_cbox_test->setGeometry( rc.width()*0.2, rc.height()*0.2+50, 100, 30);
	m_lb_aveTm->setGeometry(20, rc.height()*0.2+100, 100, 30);
	m_lne_aveTm->setGeometry( rc.width()*0.1, rc.height()*0.2+100, 80, 30);
	m_lb_units->setGeometry( rc.width()*0.25, rc.height()*0.2+100, 50, 30);
	m_lb_server->setGeometry( 10, rc.height()*0.5, 250, 30);


	m_lb_loadStart->setGeometry( 10, rc.height()*0.6, 120, 30);
	m_load_StartTime->setGeometry( rc.width()*0.1, rc.height()*0.6, 150, 30);
	m_lb_loadStop->setGeometry( rc.width()*0.3, rc.height()*0.6, 120,30);
	m_load_StopTime->setGeometry( rc.width()*0.4, rc.height()*0.6, 150,30);
	m_bt_load->setGeometry( rc.width()*0.5,rc.height()*0.7, 100,40);

	

	m_lb_device->setGeometry(10, rc.height()*0.7, 100, 30);
	m_comb_deviceID->setGeometry(rc.width()*0.1, rc.height()*0.7, 150, 30);
	m_lb_loadCount->setGeometry(10, rc.height()*0.8, 150, 30 );
	m_loadProgress->setGeometry( rc.width()*0.15, rc.height()*0.8, 250, 30);
}
void QExportWidget::ExportDataSlot()
{
	if( !m_cbox_ave->isChecked() && !m_cbox_test->isChecked() )
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请选择导出数据的类型！"));
		return;
	}

    QString qstrExportDir = QFileDialog::getExistingDirectory(this, QStringLiteral("导出文件存放路径"), 
        QInitConfiguration::GetInitInstance()->GetLastExportPath(), 
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (qstrExportDir.trimmed().isEmpty())
    {
        return;
    }
    else
    {
        m_thread->SetExportDir(qstrExportDir);
        QInitConfiguration::GetInitInstance()->SetLastExportPath(qstrExportDir);
    }
	m_thread->setIsExportInfo(m_radio_image->isChecked(), m_cbox_ave->isChecked(), m_cbox_test->isChecked() );
	QDateTime exportdate = m_Begin_time->dateTime();
	m_thread->setDate( exportdate,  m_End_time->dateTime(),m_lne_aveTm->text().toInt());
	if( m_thread->isRunning())
	{
		m_thread->wait();
	}
	m_thread->start();
	emit BeginExport(QString::fromLocal8Bit("开始导出数据"));
}
void QExportWidget::GetQueryDataInfo()
{
	if( m_vt_testInfo.count() >0 )
		ExportToImage(m_vt_testInfo);
	if( m_vt_aveInfo.count() > 0 )
		ExportToImage( m_vt_aveInfo , true);
	m_vt_aveInfo.clear();
	m_vt_testInfo.clear();

	emit ExportCompelete(1);
}

void QExportWidget::ExportTestImageSlot()
{
	m_vt_testInfo.clear();
	m_thread->GetDataInfo(m_vt_testInfo);	
}
void QExportWidget::ExportAveImageSlot()
{
	m_vt_aveInfo.clear();
	m_thread->GetDataInfo(m_vt_aveInfo, true );
}
QwtPlot* QExportWidget::InitQwtPlot()
{
	QwtPlot* m_plot = new QwtPlot( this );
	m_plot->setCanvasBackground(Qt::white);

	QwtPlotGrid *grid = new QwtPlotGrid();
	grid->enableXMin( true );
	grid->enableYMin( true );
	grid->setMinorPen(Qt::gray, 1, Qt::DashLine );
	grid->attach( m_plot );

	QwtLegend *legend = new QwtLegend;
	//设置可以点击
	//legend->setDefaultItemMode( QwtLegendData::Checkable );
	m_plot->insertLegend( legend, QwtPlot::LeftLegend );
	m_plot->setAxisScaleDraw(QwtPlot::xBottom,new TimeScaleDraw( ) );
	m_plot->setAxisLabelRotation( QwtPlot::xBottom, -50.0 );
	m_plot->setAxisLabelAlignment( QwtPlot::xBottom, Qt::AlignLeft | Qt::AlignBottom );
	QRect rc = geometry();
	m_plot->setGeometry(10, 10, rc.width()-50, rc.height()-50);
	return m_plot;
}
void QExportWidget::ExportToImage(QVector<EXPORT_IMAGE_INFO> &vt, bool isave)
{
	QwtPlotRenderer renderer;
	
	QwtPlot *plot_h2s  = InitQwtPlot();
//	QwtPlot *plot_so2  = InitQwtPlot();
	for( int i = 0; i < vt.count(); ++i )
	{
		QString strPath = "./export/";
		strPath += vt[i].deviceName;
		strPath += "-";
		//strPath += vt[i].dates;
		strPath += QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss");
		if( isave )
			strPath += "-ave";
		double x_min = 0.0, x_max = 0.0;
		if( vt[i].datas_H2S.x_value.count() !=0 )
		{
			
			QwtPlotCurve *d_curves = new QwtPlotCurve();
			d_curves->setPen( Qt::red );
			d_curves->setStyle( QwtPlotCurve::Lines );
			d_curves->setTitle("H2S");
			d_curves->attach( plot_h2s );
			x_min = vt[i].datas_H2S.x_value[0];
			x_max = vt[i].datas_H2S.x_value.last();
			plot_h2s->setAxisScale( QwtPlot::xBottom,x_min, x_max);
			d_curves->setSamples(vt[i].datas_H2S.x_value, vt[i].datas_H2S.y_value );

			QwtPlotCurve *d_so2 = new QwtPlotCurve();
			d_so2->setPen( Qt::blue );
			d_so2->setStyle( QwtPlotCurve::Lines );
			d_so2->setTitle("SO2");
			d_so2->attach( plot_h2s );
			d_so2->setSamples(vt[i].datas_H2S.x_value,vt[i].datas_SO2.y_value);
			plot_h2s->replot();
			QString strH2s = strPath;
			strH2s += ".jpg";
			renderer.renderDocument(plot_h2s, strH2s, QSizeF(600, 200), 85);
			
			delete d_curves;
		}
	/*	if( vt[i].datas_SO2.x_value.count() != 0 )
		{
			
			QwtPlotCurve *curves = new QwtPlotCurve();
			curves->setPen( Qt::red );
			curves->setStyle( QwtPlotCurve::Lines );
			curves->setTitle("SO2");
			curves->attach( plot_so2 );
			x_min = vt[i].datas_SO2.x_value[0];
			x_max = vt[i].datas_SO2.x_value.last();
			plot_so2->setAxisScale( QwtPlot::xBottom,x_min, x_max);
			curves->setSamples(vt[i].datas_SO2.x_value, vt[i].datas_SO2.y_value );
			QString strSO2 = strPath;
			strSO2 += "-SO2.jpg";
			renderer.renderDocument(plot_so2, strSO2, QSizeF(600, 200), 85);
			
			delete curves;
		}	*/	
	}
	delete plot_h2s;
	//delete plot_so2;
}

void QExportWidget::AveCheckBoxStateChange(int state)
{
	if( state == 0 )
	{
		m_lb_aveTm->hide();
		m_lb_units->hide();
		m_lne_aveTm->hide();
	}
	else if( state == 2)
	{
		m_lb_aveTm->show();
		m_lb_units->show();
		m_lne_aveTm->show();
	}
}
void QExportWidget::LoadServerDataSlot()
{
	m_vt_count.clear();
	QString strName = m_comb_deviceID->currentText();
	if( strName.compare("所有设备") != 0)
	{
		for( int j = 0; j < m_btn_instrument.count(); ++j )
		{
			if( m_btn_instrument[j].deviceName.compare( strName) == 0 )
			{
				m_loadDeviceID= m_btn_instrument[j].deviceid ;
				break;
			}
		}
		
	}
	else
	{
		m_loadDeviceID = "";
	}
	
	if( !QCheckRecvLoadDataQueueThread::GetInstance()->isRunning() )
	{
		QCheckRecvLoadDataQueueThread::GetInstance()->start();
	}
	m_DataCount = 0;
	m_record = 0;
	QDateTime beginTime = m_load_StartTime->dateTime();
	QTime bTime = beginTime.time();
	bTime.setHMS(bTime.hour(),bTime.minute(),bTime.second(),0);
	beginTime.setTime(bTime);
	QString str = beginTime.toString("yyyy-MM-dd hh:mm:ss.zzz");
	m_dateStartTime = beginTime;
	m_dateEndTime = m_load_StopTime->dateTime();
    bTime = m_dateEndTime.time();
    bTime.setHMS(bTime.hour(),bTime.minute(),bTime.second(),999);
    m_dateEndTime.setTime(bTime);

	int idays = beginTime.daysTo( m_dateEndTime );
	
	for( int i = 0; i <= idays ;i++ )
	{
		QDateTime tmpTime = QDateTime::fromString( beginTime.date().toString("yyyy-MM-dd") + " 23:59:59.999", "yyyy-MM-dd hh:mm:ss.zzz");
		if(  i == idays)
			tmpTime = m_dateEndTime;
		QString strtmp = beginTime.toString("yyyy-MM-dd hh:mm:ss.zzz");
		strtmp = tmpTime.toString("yyyy-MM-dd hh:mm:ss.zzz");
		CMessage *message = new CMessage();
		message->m_header.m_struct.cmd = PC_CLIENT_CMD_GET_HISTORY_DEVICE_DATA_COUNT;
		message->m_header.m_struct.messageNo = i;
		message->m_header.m_struct.cmdResult = 1;// 非0表示 需要获取错误信息
		CMessageGetDeviceDataCountReq * pGetDeviceDataCountReq = new CMessageGetDeviceDataCountReq();
		pGetDeviceDataCountReq->m_struct.begin = beginTime.toMSecsSinceEpoch();
		pGetDeviceDataCountReq->m_struct.end =tmpTime.toMSecsSinceEpoch();
		if( m_loadDeviceID.compare("") != 0 )
		{
			pGetDeviceDataCountReq->SetDeviceID( m_loadDeviceID );
		}
		message->SetMessageBody(pGetDeviceDataCountReq);
	
		m_messageThread->SendMessage(message);
		beginTime = QDateTime::fromString(beginTime.date().addDays(1).toString("yyyy-MM-dd ") + "00:00:00.000", "yyyy-MM-dd hh:mm:ss.zzz");
		m_record++;
		m_vt_count.push_back(0);
	}
}
void QExportWidget::setMessageThread(QMessageThread *messageThread, QRemoveDataToBaseThread *removeThread)
{
	m_messageThread = messageThread;
	QCheckRecvLoadDataQueueThread::GetInstance()->SetMessageThread(m_messageThread);
	connect( m_messageThread, SIGNAL(ServerRepDeviceDataCount(CMessage*)), this, SLOT(ServerRepDataCount(CMessage*)));
	m_removeThread = removeThread;
	connect( m_removeThread, SIGNAL(SaveLoadDataCount(int)), this, SLOT(SaveLoadDataCountSlot(int)));
}
void QExportWidget::ServerRepDataCount(CMessage*msg)
{
	if( msg->m_header.m_struct.cmd == PC_SERVER_CMD_GET_HISTORY_DEVICE_DATA_COUNT_REP && msg->m_header.m_struct.cmdResult == 0 )
	{
		CMessageGetDeviceDataCountRep* rep = static_cast<CMessageGetDeviceDataCountRep*>(msg->m_pBody);
		m_DataCount += rep->m_struct.deivceDataCount;
		int index = msg->m_header.m_struct.messageNo;
		m_vt_count[index] =  rep->m_struct.deivceDataCount ;
		m_record--;
	}
	if( m_record == 0 )
	{
		if( m_progressIsShow == false)
		{
			m_loadProgress->setValue( 0 );
			m_loadProgress->setMaximum( m_DataCount );
		}
		QDateTime beginTime = m_dateStartTime;
		int idays = beginTime.daysTo( m_dateEndTime );
		Q_ASSERT(idays+1 == m_vt_count.count());
		for( int i = 0; i <= idays; i++)
		{
			QDateTime tmpTime = QDateTime::fromString( beginTime.date().toString("yyyy-MM-dd") + " 23:59:59", "yyyy-MM-dd hh:mm:ss");
			if(  i == idays)
				tmpTime = m_dateEndTime;
			if( m_vt_count[i] != 0 )
			{
				int itimes = m_vt_count[i] / 100;
				int ireCount = m_vt_count[i] %100;
				int i =0;
				for( ; i <= itimes; ++i )
				{
					CMessage *message = new CMessage();
					message->m_header.m_struct.cmd = PC_CLIENT_CMD_GET_HISTORY_DEVICE_DATA;
					message->m_header.m_struct.messageNo = 5;
					message->m_header.m_struct.cmdResult = 1;// 非0表示 需要获取错误信息
					QString strtmp = beginTime.toString("yyyy-MM-dd hh:mm:ss");
					strtmp = tmpTime.toString("yyyy-MM-dd hh:mm:ss");
					CMessageGetDeviceDataReq * pGetDeviceDataReq = new CMessageGetDeviceDataReq();
					pGetDeviceDataReq->m_struct.begin = beginTime.toMSecsSinceEpoch();
					pGetDeviceDataReq->m_struct.end =tmpTime.toMSecsSinceEpoch();
					pGetDeviceDataReq->m_struct.pageIndex = i+1;
					pGetDeviceDataReq->m_struct.rowsOfPerPage = 100;
					if( m_loadDeviceID.compare("") != 0 )
					{
						pGetDeviceDataReq->SetDeviceID( m_loadDeviceID );
					}
					message->SetMessageBody(pGetDeviceDataReq);	
					m_messageThread->SendMessage(message);
				}								
			}
			beginTime = QDateTime::fromString(beginTime.date().addDays(1).toString("yyyy-MM-dd ") + "00:00:00", "yyyy-MM-dd hh:mm:ss");
			m_record++;
		}
		emit BeginLoadData( QString::fromLocal8Bit("开始下载数据"));
	}
}

void QExportWidget::RecvServerLoadData(CMessage*msg)
{
	
	if( msg->m_header.m_struct.messageNo == 5 && msg->m_header.m_struct.cmd == PC_SERVER_CMD_GET_HISTORY_DEVICE_DATA_REP && msg->m_header.m_struct.cmdResult == 0)
	{	
		CMessageGetDeviceDataRep* rep = static_cast<CMessageGetDeviceDataRep*>(msg->m_pBody);
		QList<CMessageDeviceDataItem*> listItem = rep->m_lstDeviceData;
		for( int i = 0; i < listItem.count(); ++i )
		{
			DATA_INFO* st = new DATA_INFO();
			QString strid = listItem[i]->GetDeviceID();
			for( int j = 0; j < m_btn_instrument.count(); ++j )
			{
				if( strid.compare( m_btn_instrument[j].deviceid) == 0 )
				{
					st->name = m_btn_instrument[j].deviceName;
					st->id = m_btn_instrument[j].deviceno;
					break;
				}
			}
			st->values = QString("%1").arg(listItem[i]->m_struct.data);
			CMessageDeviceRealtimeData data = listItem[i]->m_deviceData;
			st->times =QString("%1").arg( data.m_struct.timestamp );
			st->types = QString("probe%1:%2").arg( data.m_struct.channelNo).arg(GetGasDisplayNameByCode(data.m_struct.datatype) );
			m_vtbasedata.push_back( st );
		}
		m_removeThread->setDataInfo( m_vtbasedata , true);
		m_vtbasedata.clear();
		m_record--;
	}
	else if(msg->m_header.m_struct.cmdResult != 0)
	{
		//QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit(rep->))
	}
	
}
void QExportWidget::setDeviceInfo( const QVector<BUTTON_INSTRUMENT> btn_instrument)
{
	m_btn_instrument = btn_instrument;
	m_comb_deviceID->addItem(QString::fromLocal8Bit("所有设备"));
	for( int i = 0; i < m_btn_instrument.count();++i)
	{
		m_comb_deviceID->addItem(m_btn_instrument[i].deviceName);
	}
	m_comb_deviceID->setCurrentText(QString::fromLocal8Bit("所有设备"));
	
}
void QExportWidget::SaveLoadDataCountSlot(int icount)
{
	if( m_progressIsShow == false )
	{
		m_progressIsShow = true;
		m_lb_loadCount->show();
		m_loadProgress->show();
	}
	int iallCount = m_loadProgress->value();
	iallCount += icount;
	m_loadProgress->setValue( iallCount );
	//QString str = m_lb_loadCount->text();
	//str =QString("%1 / %2").arg( iallCount).arg( m_loadProgress->maximum());
	//m_lb_loadCount->setText( str );
	if( iallCount == m_loadProgress->maximum() )
	{
		emit ExportCompelete(2);
		m_loadProgress->setValue( 0 );
		m_loadProgress->hide();
		m_lb_loadCount->hide();
		m_progressIsShow = false;
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("数据下载完成！"));
	}
}