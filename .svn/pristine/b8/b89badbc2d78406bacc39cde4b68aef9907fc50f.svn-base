#include "qdatashowwidget.h"
#include <QtGlobal>
#include <QPainter>
#include <qwt_dial_needle.h>
#include <QDateTime>
#include <QMessageBox>

QDataShowWidget::QDataShowWidget(QVector<DATA_SHOW_INFO> info,QWidget *parent)
    : m_info( info ), QWidget(parent)
{
    Q_ASSERT( m_info.count() == 2 );

    QRect rc = geometry();
    QString msg ;
    m_currentIntrument = new QLabel(QString::fromLocal8Bit("当前设备："), this );
    m_currentIntrument->setGeometry(30, 20, 250, 30 );

    msg = QString::fromLocal8Bit("（瞬时值）：H2S");
    m_lb_datatype = new QLabel( msg, this );
    m_lb_datatype->setGeometry( 30, 50, 200, 30 );

    msg = QString::fromLocal8Bit("（瞬时值）：SO2");
    m_lb_datatype2 = new QLabel( msg, this );
    m_lb_datatype2->setGeometry( 30, rc.height()*0.5+20, 200, 30 );
    
    m_unit1 = new QLabel( this );
    msg = QString::fromLocal8Bit("单位：");
    msg += m_info[0].units;
    m_unit1->setGeometry(rc.width()*0.15 + 30, 80, 100, 30);

    m_unit2 = new QLabel( this );
    msg = QString::fromLocal8Bit("单位：");
    msg += m_info[1].units;
    m_unit2->setGeometry(rc.width()*0.15 + 30,rc.height()*0.5, 100, 30);

    m_lb_aveTm = new QLabel(QString::fromLocal8Bit("平均时间:"), this );
    m_lb_aveTm->setGeometry(rc.width()*0.5+20, 20, 100, 30 );
    m_lne_aveTm = new QLineEdit("10", this );
    m_lne_aveTm->setGeometry(rc.width()*0.6, 20, 100, 30);

    m_lb_minites = new QLabel(QString::fromLocal8Bit("分钟"), this );
    m_lb_minites->setGeometry( rc.width()*0.7+20,20, 50, 20 );

    m_pb_aveTm = new QPushButton(QString::fromLocal8Bit("确定"), this );
    m_pb_aveTm->setGeometry(rc.width()*0.8, 20, 100, 30 );

    m_datatype_ave1 = new QLabel( QString::fromLocal8Bit("（平均值）：H2S"), this );
    m_datatype_ave1->setGeometry(rc.width()*0.5+20, 50, 200, 30 );

    m_datatype_ave2 = new QLabel(QString::fromLocal8Bit("（平均值）：SO2"), this );
    m_datatype_ave2->setGeometry(rc.width()*0.5+20, rc.height()*0.5+20, 200, 30);

    m_unit_ave1 = new QLabel(QString::fromLocal8Bit("单位:"), this );
    m_unit_ave1->setGeometry(rc.width()*0.65+20,50, 200, 30 );

    m_unit_ave2 = new QLabel(QString::fromLocal8Bit("单位:"), this );
    m_unit_ave2->setGeometry(rc.width()*0.65+20, rc.height()*0.5+20, 200, 30);

    m_view1 = new QTableView( this );
    m_model1 = new QCheckDataModel( this );
    m_view1->setModel( m_model1 );
    m_view1->setColumnWidth(0, rc.width()*0.08);
    m_view1->setColumnWidth(1, rc.width()*0.15);
    m_view1->setColumnWidth(2, rc.width()*0.15);
    m_view1->setGeometry(30, 80, rc.width()*0.4, rc.height()*0.3);

    m_view2 = new QTableView( this );
    m_model2 = new QCheckDataModel( this );
    m_view2->setModel( m_model2 );
    m_view2->setColumnWidth(0, 30);
    m_view2->setColumnWidth(1, rc.width()*0.15);
    m_view2->setColumnWidth(2, rc.width()*0.15);
    m_view2->setGeometry(30, rc.height()*0.5+40, rc.width()*0.4, rc.height()*0.3);

    m_view_ave1 = new QTableView( this );
    m_model_ave1 = new QCheckDataModel( this );
    m_view_ave1->setModel( m_model_ave1 );
    m_view_ave1->setColumnWidth(0, 30 );
    m_view_ave1->setColumnWidth( 1, rc.width()*0.15);
    m_view_ave1->setColumnWidth( 2, rc.width()*0.15);
    m_view_ave1->setGeometry( rc.width()*0.5+20, 80, rc.width()*0.4, rc.height()*0.3 );

    m_view_ave2 = new QTableView( this );
    m_model_ave2 = new QCheckDataModel( this );
    m_view_ave2->setModel( m_model_ave2 );
    m_view_ave2->setColumnWidth(0, 30);
    m_view_ave2->setColumnWidth(1, rc.width()*0.15);
    m_view_ave2->setColumnWidth(2, rc.width()*0.15);
    m_view_ave2->setGeometry(rc.width()*0.5+20, rc.height()*0.5+40, rc.width()*0.4, rc.height()*0.3);

    m_pb_Changedevice = new QPushButton(QString::fromLocal8Bit("切换设备"), this );
    m_pb_Changedevice->setGeometry(rc.width()*0.9,rc.height()*0.85, 100, 30 );
    connect( m_pb_Changedevice, SIGNAL(clicked()), this, SLOT(ChangeDeviceSlot()));
    connect( m_pb_aveTm, SIGNAL(clicked()), this, SLOT(AveCalculateSlot()));
    QRegExp regexp("^[1-9]\\d*\\.\\d*|0\\.\\d*[1-9]\\d*$"); 
    m_lne_aveTm->setValidator(new QRegExpValidator(regexp, m_lne_aveTm));
    m_threads = new QQueryDataThread( this );
    connect( m_threads, SIGNAL(ThreadOver()), this, SLOT(GetAveDataSlot()));
    m_aveTime = 10;
    m_isCalculate = false;
}

QDataShowWidget::~QDataShowWidget()
{
    if( m_threads->isRunning() )
    {
        m_threads->wait();
    }
    qDebug() << "QDataShowWidget release";
}
void QDataShowWidget::paintEvent(QPaintEvent *e )
{
    QWidget::paintEvent(e);
    /*QPainter painter( this );
    QBrush brush(QColor(255,255,255));
    painter.setBackground( brush );

    painter.drawRect( 400, 30, 350, 150 );
    painter.drawRect( 400, 290, 350, 150 );*/
    
}
void QDataShowWidget::resizeEvent(QResizeEvent *e)
{
    QRect rc = geometry();
    m_view1->setGeometry(30, 80, rc.width()*0.4, rc.height()*0.35);
    m_view2->setGeometry(30, rc.height()*0.5+40, rc.width()*0.4, rc.height()*0.35);

    m_view_ave1->setGeometry( rc.width()*0.5+20, 80, rc.width()*0.4, rc.height()*0.35 );
    m_view_ave2->setGeometry(rc.width()*0.5+20, rc.height()*0.5+40, rc.width()*0.4, rc.height()*0.35);

    m_pb_Changedevice->setGeometry(rc.width()*0.85,rc.height()*0.93, 100, 30 );
    m_unit2->setGeometry(rc.width()*0.25 + 30,rc.height()*0.5+10, 100, 30);
    m_unit1->setGeometry(rc.width()*0.25 + 30, 50, 100, 30);
    m_lb_datatype->setGeometry( 30, 50, 200, 30 );
    m_lb_datatype2->setGeometry( 30, rc.height()*0.5+10, 200, 30 );

    m_datatype_ave1->setGeometry(rc.width()*0.5+20, 50, 200, 30 );

    m_datatype_ave2->setGeometry(rc.width()*0.5+20, rc.height()*0.5+10, 200, 30);

    m_unit_ave1->setGeometry(rc.width()*0.75+20,50, 200, 30 );

    m_unit_ave2->setGeometry(rc.width()*0.75+20, rc.height()*0.5+10, 200, 30);

    m_view1->setColumnWidth(0, rc.width()*0.08);
    m_view1->setColumnWidth(1, rc.width()*0.23-10);
    m_view1->setColumnWidth(2, rc.width()*0.1-20);

    m_view2->setColumnWidth(0, rc.width()*0.08);
    m_view2->setColumnWidth(1, rc.width()*0.23-10);
    m_view2->setColumnWidth(2, rc.width()*0.1-20);

    m_view_ave1->setColumnWidth(0, rc.width()*0.08);
    m_view_ave1->setColumnWidth(1, rc.width()*0.23-10);
    m_view_ave1->setColumnWidth(2, rc.width()*0.1-20);

    m_view_ave2->setColumnWidth(0, rc.width()*0.08);
    m_view_ave2->setColumnWidth(1, rc.width()*0.23-10);
    m_view_ave2->setColumnWidth(2, rc.width()*0.1-20);

    m_lb_aveTm->setGeometry(rc.width()*0.5+20, 20, 100, 20 );
    m_lne_aveTm->setGeometry(rc.width()*0.6, 20, 100, 20);
    m_pb_aveTm->setGeometry(rc.width()*0.8, 20, 60, 20 );
    m_lb_minites->setGeometry( rc.width()*0.7+40,20, 50, 20 );
}
void QDataShowWidget::ChangeDeviceSlot()
{
    emit clicked();
}

void QDataShowWidget::setCurrentDevice( QString deivce /*, QString sample1 , QString sample2*/)
{
    QString msg = QString::fromLocal8Bit("当前设备:");
    msg += deivce;
    m_currentIntrument->setText( msg );
    //m_sample1 = sample1.toInt();
    //m_sample2 = sample2.toInt();

}

void QDataShowWidget::setInfo(QVector<DATA_SHOW_INFO> &info ,XY_VALUE &st1, XY_VALUE &st2, XY_VALUE &ave1_st, XY_VALUE &ave2_st)
{
    m_isCalculate = false;
    if( info.count() != 2 ) 
    {
        return;
    }
    m_dataprobe1.x_value.clear();
    m_dataprobe1.y_value.clear();
    m_dataprobe2.x_value.clear();
    m_dataprobe2.y_value.clear();

    //m_dataprobe1 = st1;
    //m_dataprobe2 = st2;
    m_info = info;
    QString msg;
    msg = QString::fromLocal8Bit("(瞬时值)：H2S");
    m_lb_datatype->setText( msg ); 
    msg = QString::fromLocal8Bit("(平均值)：H2S");
    m_datatype_ave1->setText( msg ); 

    msg = QString::fromLocal8Bit("(瞬时值)：SO2");
    m_lb_datatype2->setText( msg );

    msg = QString::fromLocal8Bit("(平均值)：SO2");
    m_datatype_ave2->setText( msg );

    msg = QString::fromLocal8Bit("单位：");
    msg += m_info[0].units;
    m_unit1->setText( msg );
    m_unit_ave1->setText( msg );
    msg = QString::fromLocal8Bit("单位：");
    msg += m_info[1].units;
    m_unit2->setText( msg );
    m_unit_ave2->setText( msg );

    m_model1->cleanData();
    m_model2->cleanData();
    m_model_ave1->cleanData();
    m_model_ave2->cleanData();
    for( int i = 0; i < st1.x_value.count(); ++i )
    {
        UpdateOneModel( m_model1, st1.y_value[i], st1.x_value[i]);
    }
    for( int i = 0; i < st2.x_value.count(); ++i )
    {
        UpdateOneModel( m_model2, st2.y_value[i], st2.x_value[i]);
    }

    QStringList strlist = m_currentIntrument->text().split("_");

    if( strlist.count() == 2 )
    {
        m_threads->setQueryFactor( strlist[1],m_lne_aveTm->text().toInt(), m_db);
        m_threads->start();
    }
    emit BeginQueryData();
}

void QDataShowWidget::setDialMaxRange(double range1 /* = 10 */, double range2 /* = 10 */)
{
    /*m_dialOne->setScale(0,range1 );
    //角度
    m_dialOne->setScaleArc(0, 270.0 );
    //将100分为10格
    m_dialOne->setScaleMaxMajor(10);
    //再将第一小格分为2小格
    m_dialOne->setScaleMaxMinor( 2 );

    m_dialTwo->setScale(0, range2 );
    //角度
    m_dialTwo->setScaleArc(0, 270.0 );
    //将100分为10格
    m_dialTwo->setScaleMaxMajor(10);
    //再将第一小格分为2小格
    m_dialTwo->setScaleMaxMinor( 2 );*/
}
void QDataShowWidget::UpdateInfo(int index, double dates, double data, bool isave)
{
    int icount = 0;
    //double idate = dates.toDouble();
    QString strdate = QDateTime::fromMSecsSinceEpoch(dates).toString("yyyy-MM-dd hh:mm:ss");
    QString strdata = QString("%1").arg( data );
    if( index == 1 )
    {
        if( !isave )
        {
            UpdateOneModel( m_model1, data, dates );
            m_dataprobe1.x_value.push_back( dates );
            m_dataprobe1.y_value.push_back( data );
            /*icount = m_model1->rowCount();
            Data_Info *info = new Data_Info(QString("%1").arg(icount), strdate, strdata, this );
            QModelIndex index = m_model1->index(0, 0, QModelIndex());
            m_model1->setData(index, QVariant::fromValue(info), Qt::UserRole);*/
        }		
        else 
        {
            //UpdateOneModel( m_model_ave1, data, dates );
            /*icount = m_model_ave1->rowCount();
            Data_Info *info = new Data_Info(QString("%1").arg(icount), strdate, strdata, this );
            QModelIndex index = m_model_ave1->index(0, 0, QModelIndex());
            m_model_ave1->setData(index, QVariant::fromValue( info), Qt::UserRole );*/
        }
    }
    else
    {
        if( !isave )
        {
            UpdateOneModel( m_model2, data, dates );
            m_dataprobe2.x_value.push_back( dates );
            m_dataprobe2.y_value.push_back( data );
            /*icount = m_model2->rowCount();
                Data_Info *info = new Data_Info(QString("%1").arg(icount), strdate, strdata, this );
            QModelIndex index = m_model2->index(0, 0, QModelIndex());
            m_model2->setData(index, QVariant::fromValue(info), Qt::UserRole);*/
        }		
        else 
        {
            //UpdateOneModel( m_model_ave2, data, dates );
            /*icount = m_model_ave2->rowCount();
            Data_Info *info = new Data_Info(QString("%1").arg(icount), strdate, strdata, this );
            QModelIndex index = m_model_ave2->index(0, 0, QModelIndex());
            m_model_ave2->setData(index, QVariant::fromValue( info), Qt::UserRole );*/
        }
        
    }
    if( m_isCalculate )
    {
        AveCalculateRecv( m_dataprobe1, m_model_ave1);
        AveCalculateRecv( m_dataprobe2, m_model_ave2);
    }
}

void QDataShowWidget::AveCalculateSlot()
{
	if( m_lne_aveTm->text().toDouble() <1 )
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("平均时间不能小于1分钟！请重新输入平均时间"));
		return;
	}
    m_isCalculate = false;
    m_model_ave1->cleanData();
    m_model_ave2->cleanData();
    
    if( m_threads->isRunning() )
    {
        m_threads->wait();
    }
    QStringList strlist = m_currentIntrument->text().split("_");
    //m_icount1 =*60 / m_sample1;
    //m_icount2 = m_lne_aveTm->text().toInt()*60/m_sample2;
    m_aveTime = m_lne_aveTm->text().toInt();
    if( strlist.count() == 2 )
    {
        m_threads->setQueryFactor( strlist[1], m_lne_aveTm->text().toInt(), m_db);
        m_threads->start();
    }
    emit BeginQueryData();
}

void QDataShowWidget::setDatabase(QSqlDatabase db)
{
    m_db = db;
}
void QDataShowWidget::GetAveDataSlot()
{
    XY_VALUE st1;
    XY_VALUE st2;
    m_threads->GetQueryResult(st1, st2);
    int icount = 0;
    for( int i = 0; i < st1.x_value.count(); ++i )
    {
        UpdateOneModel( m_model_ave1, st1.y_value[i], st1.x_value[i]);
        /*QString strdate = QDateTime::fromMSecsSinceEpoch(st1.x_value[i]).toString("yyyy-MM-dd hh:mm:ss");
        QString strdata = QString("%1").arg( st1.y_value[i] );
        icount = m_model_ave1->rowCount();
        Data_Info *info = new Data_Info(QString("%1").arg(icount), strdate, strdata, this );
        QModelIndex index = m_model_ave1->index(0, 0, QModelIndex());
        m_model_ave1->setData(index, QVariant::fromValue(info), Qt::UserRole);	*/	
    }
    for( int i = 0; i < st2.x_value.count(); ++i )
    {
        UpdateOneModel( m_model_ave2, st2.y_value[i], st2.x_value[i]);
    }

    //AveCalculateRecv( m_dataprobe1, m_model_ave1);
    //AveCalculateRecv( m_dataprobe2, m_model_ave2);
    m_isCalculate = true;
    emit QueryDataOver();
}

void QDataShowWidget::UpdateOneModel( QCheckDataModel *model, double value, double times)
{
	QString strdata;
	if( model == m_model1 || model == m_model_ave1 )
	{
		strdata = QString("%1").arg(value, 0, 'f', 1);
		/*strdata = QString::number(value,'g',2);
		if( strdata.count() - strdata.indexOf(".") == 3 )
		{
			strdata = strdata.left( strdata.count() -1 );
		}
		else if( strdata.indexOf(".") == -1)
		{
			strdata += ".0";
		}*/
	
	}
	else if( model == m_model2 || model == m_model_ave2 )
	{
		strdata = QString("%1").arg(value, 0, 'f', 2);
		/*strdata = QString::number(value, 'g',2);
		if( strdata.indexOf(".") == -1)
		{
			strdata += ".00";
		}
		else if( strdata.count()-strdata.indexOf(".") == 2)
		{
			strdata += "0";
		}*/
	}
    QString strdate = QDateTime::fromMSecsSinceEpoch(times).toString("yyyy-MM-dd hh:mm:ss");
     
    int icount = model->rowCount();
    Data_Info *info = new Data_Info(QString("%1").arg(icount), strdate, strdata, this );
    QModelIndex index = m_model_ave2->index(0, 0, QModelIndex());
    model->setData(index, QVariant::fromValue(info), Qt::UserRole);		
}
void QDataShowWidget::AveCalculateRecv(XY_VALUE &st, QCheckDataModel *model)
{
    while( st.x_value.count() > 0 )
    {
        bool isCal = false;
        double startTim = st.x_value[0];
        double endTim = startTim + m_aveTime*60*1000;
        int icount = 0; 
        for( int i = 0; i < st.x_value.count(); ++i )
        {
            if( st.x_value[i] >= endTim )
            {
                isCal = true;
                icount = i;
                break;
            }
        }
        if( !isCal )
        {
            break;
        }

        XY_VALUE st_in;
        st_in.x_value = st.x_value.mid(0, icount);
        st_in.y_value = st.y_value.mid(0, icount );
        XY_VALUE st_out;
        AveCalculator(st_in, m_aveTime, st_out);
        for( int i = 0; i < st_out.x_value.count(); ++i )
        {
            UpdateOneModel(model, st_out.y_value[i], st_out.x_value[i]);
        }

        st.x_value.remove(0, icount);
        st.y_value.remove(0, icount);

    }
}