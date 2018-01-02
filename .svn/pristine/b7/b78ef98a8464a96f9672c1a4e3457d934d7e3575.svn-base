#include "h2smonitorclient.h"
#include <QWidget>
#include <QGridLayout>
#include <QDebug>
#include <QFile>
#include <QLabel>
#include <QDateTime>
#include <QDesktopWidget>
#include <QApplication>
#include "qdialwidget.h"
#include <QTimer>
#include <QTextCodec>
#include "message/message_get_devices_info_req.h"
#include "message/message_device_id.h"
#include "message/message_login_rep.h"
#include "message/message_broadcast_device_realtime_data.h"
#include "message/message_device_realtime_data.h"
#include "message/message_get_devices_info_rep.h"
#include "message/message_set_device_param.h"
#include "protocol/protocol_data_desc.h"
#include <QGroupBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QSqlQuery>
#include <QSqlError>
#include "logging.h"
#include "message/message_heartbeat.h"
#include "qconnectstate.h"
#include <QProcess>
#include <QFileInfo>


H2SMonitorClient::H2SMonitorClient(QWidget *parent)
    : QMainWindow(parent )
{
	m_isLogin = false;
    setWindowTitle( QString::fromLocal8Bit("环境监测系统客户端"));
    QIcon image(":/images/images/main.png");
    setWindowIcon(image);
    m_1stcount = 0;
    m_2ndcount = 0;
    m_currentDeviceNo = 0;
//	this->setWindowFlags(Qt::Dialog | Qt::WindowMinimizeButtonHint |Qt::WindowCloseButtonHint);
    m_wait_dlg = new QWaitWidget( this );
    m_timer = new QTimer( this );	
    m_timer->setSingleShot( true );	
    m_tm_UpdateTimer = new QTimer( this );
	m_tm_HeartBeat = new QTimer( this );
    m_tm_HeartBeat->setInterval( 60000 );
    //m_userName = QString("");
    //setFixedSize(1024, 760);
    setMinimumHeight(760);
    setMinimumWidth( 1024 );

    QString strName = "./database/msg.mdb";

    InitDir();
    InitDatabase();
    m_saveDataThread = new QRemoveDataToBaseThread(m_datadb, this );
    m_saveDataThread->start();
    BuildUI();
    m_warnType = TEST_VALUE_WARN;
    m_aveTime = QInitConfiguration::GetInitInstance()->m_aveTime.toInt();
    int itype = QInitConfiguration::GetInitInstance()->m_warnType.toInt();
    if( itype == 1)
    {
        m_warnType = AVE_VALUE_WARN;
    }
    else if( itype == 2)
    {
        m_warnType = TEST_VALUE_WARN;
    }
    
    m_calculateCount = 0;
    m_connectCount = 100;
    connect( m_timer, SIGNAL(timeout()), this, SLOT(TimeoutSlot()));
    connect( m_tm_UpdateTimer, SIGNAL(timeout()), this, SLOT(UpdateButtonState()));
	connect( m_tm_HeartBeat, SIGNAL(timeout()), this, SLOT(HeartBeatSlot()));
    connect( this, SIGNAL(UpdateWarnSignal()), this, SLOT(UpdateWarnState()));
	connect( QConnectState::getInstance(), SIGNAL(disConnect()), this, SLOT(ServerDisconnectSlot()));
	connect( QConnectState::getInstance(), SIGNAL(connected()), this, SLOT(ServerConnectSlot()));
    CLogging::GetInstance()->Log(LOG_INFO,"client", QString::fromLocal8Bit("客户端启动成功"));
	m_WarnPlayer = new QMediaPlayer( this );
	m_mediaList = new QMediaPlaylist(this );
	bool isok = m_mediaList->addMedia(QMediaContent(QUrl("./Resources/warn.mp3")));
	isok = m_mediaList->addMedia(QMediaContent(QUrl("./Resources/Fire.mp3")));
	m_mediaList->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
	m_WarnPlayer->setPlaylist( m_mediaList );
	m_WarnPlayer->setVolume( 200 );
	m_isFisrtPlayer = false;
	m_isSecondPlayer = false;
	
}

H2SMonitorClient::~H2SMonitorClient()
{
    if( m_tm_UpdateTimer->isActive() )
    {
        m_tm_UpdateTimer->stop();
    }
	if( m_tm_HeartBeat->isActive() )
	{
		m_tm_HeartBeat->stop();
	}
    if( m_messageThread->isRunning() )
    {
        m_messageThread->SetThreadOver(   );
        m_messageThread->wait();

    }
    if( m_saveDataThread->isRunning() )
    {
        m_saveDataThread->setQuitFlag();
        m_saveDataThread->wait();
    }
    m_datadb.close();
    m_msgdb.close();
    qDebug() << "H2SMonitorClient release";
}

void H2SMonitorClient::InitDir()
{
    WCHAR path[MAX_PATH];
    DWORD dw = GetModuleFileName(NULL, path, MAX_PATH);
    QString instDir = QString::fromWCharArray(path);
    int index = instDir.lastIndexOf("\\");
    instDir.resize(index);

    m_qstrInstDir = instDir;
    m_qstrExportdir = QString("%1\\export").arg(instDir);
    char export_d[256] = ".\\export";
    strcpy(export_d, m_qstrExportdir.toStdString().c_str());
    if (0 > _access(export_d, 00))
    {
        if (ENOENT == errno)	//不存在,则创建
        {
            _mkdir(export_d);
        }
    }
    m_qstrDBDir = QString("%1\\database").arg(instDir);
    char database_d[256] = ".\\database";
    strcpy(database_d, m_qstrDBDir.toStdString().c_str());
    if (0 > _access(database_d, 00))
    {
        if (ENOENT == errno)	//不存在,则创建
        {
            _mkdir(database_d);
        }
    }
}
void H2SMonitorClient::InitDatabase()
{
    m_pMutexForMsgDB = new QMutex();
    m_msgdb = QSqlDatabase::addDatabase("QSQLITE","MSGDB");
    m_msgdb.setDatabaseName(QString("%1\\warnInfo.db").arg(m_qstrDBDir));
    //m_msgdb.setDatabaseName("./database/warnInfo.db");
    bool  ok = m_msgdb.open();  
    QSqlError errid;
    QString strerr;
    if(!ok)  
    {  
        errid = m_msgdb.lastError();
        strerr = errid.text();
        QMessageBox::information(NULL, tr("Title"), strerr );
        m_msgdb.close();
        return;
    }  
    QSqlQuery *m_pQuery = new QSqlQuery(m_msgdb);

    QString strSql  = QString::fromLocal8Bit("create table if not exists WarnMsg( 内容 varchar);");
    if(!m_pQuery->exec( strSql ) )
    {
        QSqlError str = m_pQuery->lastError();
        strerr = str.text();
        QMessageBox::information( NULL, QString::fromLocal8Bit("提示"), strerr);				
    
        m_msgdb.close();
        delete m_pQuery;
        return;
    }
    m_datadb = QSqlDatabase::addDatabase("QSQLITE","DATADB");
    QString strName = "./database/dataInfo.db";
    m_datadb.setDatabaseName(QString("%1\\dataInfo.db").arg(m_qstrDBDir));
    //m_datadb.setDatabaseName(strName);
    ok = false;
    ok = m_datadb.open();
    if( !ok )
    {
        errid = m_datadb.lastError();
        strerr = errid.text();
        QMessageBox::information(NULL, tr("Title"), strerr);
        m_msgdb.close();
        m_datadb.close();
        delete m_pQuery;
        return;
    }
    QSqlQuery *m_pdQuery = new QSqlQuery(m_datadb);
    QString strsql = QString::fromLocal8Bit("create table if not exists dataInfo( 设备名称 varchar, 设备ID varchar, 时间 varchar, 类型 varchar, 测量值 varchar,  primary key(时间,类型,设备ID));");
    if(!m_pdQuery->exec( strsql ) )
    {
        QSqlError str = m_pdQuery->lastError();
        QString strerror = str.text();
        QMessageBox::information( NULL, QString::fromLocal8Bit("提示"),strerror);
        m_msgdb.close();
        m_datadb.close();
        delete m_pdQuery;
        delete m_pQuery;
        return;
        
    }
 
    QDate curDate = QDate::currentDate();
    QDate lastDate = curDate.addDays(-1);
    QString  laststr = lastDate.toString("yyyy-MM-dd 00:00:00");
    QDateTime lastdatetime = QDateTime::fromString( laststr ,"yyyy-MM-dd hh:mm:ss");
    laststr = lastdatetime.toString("yyyy-MM-dd hh:mm:ss");
    qint64 xx = lastdatetime.toMSecsSinceEpoch();

    strsql = QString::fromLocal8Bit("delete from dataInfo where 时间<%1").arg(xx);
    if(!m_pdQuery->exec( strsql ) )
    {
        QSqlError str = m_pdQuery->lastError();
        QString strerror = str.text();
        QMessageBox::information( NULL, QString::fromLocal8Bit("提示"),strerror);
        m_msgdb.close();
        m_datadb.close();
        delete m_pdQuery;
        delete m_pQuery;
        return;
    }

    delete m_pdQuery;
    delete m_pQuery;
}
//构建主画面UI
void H2SMonitorClient::BuildUI()
{
    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::white);
    
    m_lb_title1 = new QLabel(QString::fromLocal8Bit("环境监测系统"), this );
    m_lb_title1->setFont( QFont(QString::fromLocal8Bit("宋体"), 25, QFont::Bold));
    m_lb_title1->setPalette(pa);
    QRect rc = this->geometry();
    m_lb_title1->setGeometry(rc.width()*0.03, rc.height()*0.05, rc.width()*0.24, rc.height()*0.05 );
    m_lb_title2 = new QLabel("Environment monitor system", this );
    m_lb_title2->setFont( QFont(QString::fromLocal8Bit("宋体"), 15, QFont::Bold));
    m_lb_title2->setPalette(pa);
    m_lb_title2->setGeometry(rc.width()*0.03, rc.height()*0.11, rc.width()*0.31,   rc.height()*0.03 );


    //添加菜单按钮
    QImage image;
    image.load(":/images/images/button_top1.png");
    //QIcon icon( "./Resources/images/button_top1.png");
    m_ButtonPanelShow = new QCustomButton(image, this);
    
    m_lb_show = new QLabel(QString::fromLocal8Bit("面板显示"), this );
    m_lb_show->setFont( QFont(QString::fromLocal8Bit("宋体"), 10, QFont::Bold));
    m_lb_show->setPalette(pa);
    
    //添加菜单按钮
    image.load(":/images/images/button_top2.png");
    m_pButtonParamsSet = new QCustomButton(image,this);
    
    
    m_lb_set = new QLabel(QString::fromLocal8Bit("参数设置"), this );
    m_lb_set->setFont( QFont(QString::fromLocal8Bit("宋体"), 10, QFont::Bold));
    m_lb_set->setPalette(pa);
    

    //添加菜单按钮
    image.load(":/images/images/button_top3.png");
    m_pButtonDataShow = new QCustomButton(image,this);
    
    m_lb_data = new QLabel(QString::fromLocal8Bit("数据显示"), this );
    m_lb_data->setFont( QFont(QString::fromLocal8Bit("宋体"), 10, QFont::Bold));
    m_lb_data->setPalette(pa);
    
    //添加菜单按钮
    image.load(":/images/images/button_top4.png");
    m_pButtonImageShow = new QCustomButton(image,this);
    
    m_lb_image = new QLabel(QString::fromLocal8Bit("图像显示"), this );
    m_lb_image->setFont( QFont(QString::fromLocal8Bit("宋体"), 10, QFont::Bold));
    m_lb_image->setPalette(pa);
    
    //添加数据导出
    image.load(":/images/images/bt_export.png");
    m_pButtonExport = new QCustomButton( image, this );
    m_lb_export = new QLabel( QString::fromLocal8Bit("数据导出"), this );
    m_lb_export->setFont( QFont(QString::fromLocal8Bit("宋体"), 10, QFont::Bold));
    m_lb_export->setPalette(pa);
    //添加警报信息
    image.load(":/images/images/bt_warn.png");
    m_pButtonWarningInfo = new QCustomButton( image, this );
    m_lb_warnInfo = new QLabel( QString::fromLocal8Bit("警报信息"), this );
    m_lb_warnInfo->setFont( QFont(QString::fromLocal8Bit("宋体"), 10, QFont::Bold));
    m_lb_warnInfo->setPalette(pa);

    //添加菜单按钮
    image.load(":/images/images/button_top5.png");
    m_pButtonExit = new QCustomButton(image,this);
    
    m_lb_exit = new QLabel(QString::fromLocal8Bit("退出系统"), this );
    m_lb_exit->setFont( QFont(QString::fromLocal8Bit("宋体"), 10, QFont::Bold));
    m_lb_exit->setPalette(pa);
    m_ButtonPanelShow->setGeometry( rc.width()*0.35, rc.height()*0.04, rc.width()*0.05, rc.height()*0.07);
    m_lb_show->setGeometry( rc.width()*0.35, rc.height()*0.12, rc.width()*0.06, rc.height()*0.03 );

    m_pButtonParamsSet->setGeometry(  rc.width()*0.4, rc.height()*0.04, rc.width()*0.05, rc.height()*0.07);
    m_lb_set->setGeometry( rc.width()*0.4, rc.height()*0.12, rc.width()*0.06, rc.height()*0.03 );

    m_pButtonDataShow->setGeometry( rc.width()*0.5, rc.height()*0.04, rc.width()*0.05, rc.height()*0.07);
    m_lb_data->setGeometry(rc.width()*0.5, rc.height()*0.12, rc.width()*0.06, rc.height()*0.03 );

    m_pButtonImageShow->setGeometry( rc.width()*0.6, rc.height()*0.04, rc.width()*0.05, rc.height()*0.07);
    m_lb_image->setGeometry(rc.width()*0.6, rc.height()*0.12, rc.width()*0.06, rc.height()*0.03);

    m_pButtonExport->setGeometry( rc.width()*0.7, rc.height()*0.04, rc.width()*0.05, rc.height()*0.07);
    m_lb_export->setGeometry(rc.width()*0.7, rc.height()*0.12, rc.width()*0.06, rc.height()*0.03);

    m_pButtonWarningInfo->setGeometry( rc.width()*0.8, rc.height()*0.04, rc.width()*0.05, rc.height()*0.07);
    m_lb_warnInfo->setGeometry(rc.width()*0.8, rc.height()*0.12, rc.width()*0.06, rc.height()*0.03);

    m_pButtonExit->setGeometry( rc.width()*0.9, rc.height()*0.04, rc.width()*0.05, rc.height()*0.07);
    m_lb_exit->setGeometry(rc.width()*0.9, rc.height()*0.12, rc.width()*0.06, rc.height()*0.03 );

    m_lb_wel = new QLabel(QString::fromLocal8Bit("Welcome! 欢迎使用环境监测系统！"), this );
    m_lb_wel->setFont(QFont(QString::fromLocal8Bit("宋体"), 13, QFont::Bold));
    m_lb_wel->setPalette( pa );
    
    m_lb_user = new QLabel(  this );
    m_lb_user->setPalette( pa );
    m_lb_user->setStyleSheet("font-size:15pt;");
    
    m_lb_panel = new QLabel( QString::fromLocal8Bit("仪器面板"), this );
    m_lb_panel->setFont(QFont(QString::fromLocal8Bit("宋体"), 15, QFont::Bold));
    m_lb_panel->setPalette( pa );
    
    m_lb_monit = new QLabel( QString::fromLocal8Bit("仪器监控"), this );
    m_lb_monit->setFont(QFont(QString::fromLocal8Bit("宋体"), 15, QFont::Bold));
    m_lb_monit->setPalette( pa );

    m_lb_ver = new QLabel( QString::fromLocal8Bit("@ 版权所有 环境监测系统 Environment monitor system"), this );
    m_lb_ver->setFont(QFont(QString::fromLocal8Bit("宋体"), 8, QFont::Bold));
    m_lb_ver->setPalette( pa );

    m_lb_wel->setGeometry(rc.width()*0.02, rc.height()*0.17, rc.width()*0.29, rc.height()*0.03);
    m_lb_user->setGeometry(rc.width()*0.4 ,rc.height()*0.17, rc.width()*0.5,   rc.height()*0.03);
    m_lb_panel->setGeometry(rc.width()*0.07,rc.height()* 0.20, rc.width()*0.1,   rc.height()*0.07);
    m_lb_monit->setGeometry(rc.width()*0.23, rc.height()* 0.20, rc.width()*0.1,   rc.height()*0.07);
    m_lb_ver->setGeometry(rc.width()*0.35, rc.height()* 0.97, rc.width()*0.4,   rc.height()*0.04);

    //面板显示
    m_pTabWidget = new QTabWidget(this);
    m_pTabWidget->setGeometry(rc.width()*0.23,rc.height()*0.26, 1,1 );
    
    m_allPannelInnerwidget = new QAll_pannel_widget( this );
    m_pTabWidget->addTab( m_allPannelInnerwidget ,QStringLiteral("数据显示"));

    m_pDeviceDataCurveContainer = new QWidget(m_pTabWidget);
    m_pTabWidget->addTab(m_pDeviceDataCurveContainer, QStringLiteral("曲线显示"));

    m_allSplineInnerwidget = new QInnerWidget( m_pDeviceDataCurveContainer );
    m_allSplineInnerwidget->setMinimumWidth(m_pTabWidget->geometry().width());
    m_allSplineInnerwidget->setMinimumHeight(m_pTabWidget->geometry().height() );
    m_allSplineInnerwidget->setGeometry(QRect(0,0,1, 1));
    m_allSplineShow = new QPhotoShowWidget( m_allSplineInnerwidget );
    m_allSplineShow->setGeometry(0, 0, 1, 1 );
    m_allSplineShow->setHideChangeDevice();

    //20 635

    m_state_widget = new QRunStateInfo_Widget( this );
    m_state_widget->setGeometry( rc.width()*0.01, rc.height()*0.82, 1, 1);
    //数据显示
    DATA_SHOW_INFO st;
    st.datetime = "";
    st.latitude = "";
    st.longitude = "";
    st.types = QString::fromLocal8Bit("");
    st.value = 0.0;
    m_dataShowInfo.push_back( st );

    st.datetime = "";
    st.latitude = "";
    st.longitude = "";
    st.types = QString::fromLocal8Bit("");
    st.value = 0.0;
    m_dataShowInfo.push_back( st );
    m_dataShow = new QDataShowWidget(m_dataShowInfo, this );
    m_dataShow->setGeometry(rc.width()*0.23,rc.height()*0.26, 1,1);
    // strName = QString::fromLocal8Bit("");
    m_dataShow->hide();
    m_dataShow->setDatabase(m_datadb);

    //图像显示
    m_photoShow = new QPhotoShowWidget( this );
    QVector<double> yv(POINT_COUNT,0);
    QVector<QVector<double> >vt_yva;
    vt_yva.push_back( yv );
    vt_yva.push_back( yv );
    QVector<BUTTON_INSTRUMENT> vt_tmp;
    BUTTON_INSTRUMENT bntst;
    vt_tmp.push_back( bntst );
    m_photoShow->setCurvcnt("",2,vt_tmp );
    m_photoShow->setGeometry(rc.width()*0.23,rc.height()*0.26, 1, 1 );
    m_photoShow->hide();

    //参数设置
    m_paramSetWidget = new QParamSetWidget( this );
    m_paramSetWidget->setGeometry(rc.width()*0.23,rc.height()*0.26, 1, 1  );
    m_paramSetWidget->hide();

    //数据导出 
    m_exportWidget = new QExportWidget( m_datadb, this );
    m_exportWidget->setGeometry(rc.width()*0.23,rc.height()*0.26, rc.width()*0.75, rc.height()*0.70 );
    m_exportWidget->hide();

    //报警信息
    m_allWarnInfoWidget = new QAllWarnInfoWidget( m_msgdb, this );
    m_allWarnInfoWidget->setGeometry(rc.width()*0.23,rc.height()*0.26, rc.width()*0.75, rc.height()*0.70 );
    m_allWarnInfoWidget->hide();
    
    //20,635
    //一级警报
//	image.load("./Resources/images/warning/warn_1st.png");
    m_warnline_1st = new QWarning_Widget(WARNING_1ST_STATE, this );
    m_warnline_1st->setNormal();

    m_warnline_1str_count = new QLabel("0", this );
    
    m_warnline_1str_count->hide();
    //二级警报
//	image.load("./Resources/images/warning/warn_2nd.png");
    m_warnline_2nd = new QWarning_Widget(WARNING_2ND_STATE, this );
    m_warnline_2nd->setNormal();
    
    m_warnline_2nd_count = new QLabel("0", this );
    m_warnline_1st->setGeometry(rc.width()*0.8,rc.height()*0.02+96, m_warnline_1st->width(), m_warnline_1st->height());
    m_warnline_1str_count->setGeometry(m_warnline_1st->geometry().x()+20,m_warnline_1st->geometry().y()+20/*rc.width()*0.72 ,rc.height()*0.02+116*/, 30, 30   );
    m_warnline_2nd->setGeometry(rc.width()*0.85, rc.height()*0.02+96, m_warnline_2nd->width(), m_warnline_2nd->height());
    m_warnline_2nd_count->setGeometry(m_warnline_2nd->geometry().x()+20,m_warnline_2nd->geometry().y()+20, 30, 30);
    m_warnline_2nd_count->hide();

    //清除
    image.load(":/warning/images/warning/clear.png");
    m_warn_clean = new QStateButton(image, image, this );
    m_warn_clean->setGeometry(rc.width()*0.9, rc.height()*0.015+96, 45, 45 );
    m_warn_clean->hide();
    //一级警报信息
    m_warnline_1st_dlg = new QShowWarnDlg(ONE_LEVEL, this );
    m_warnline_1st_dlg->hide();
    m_warnline_1st_dlg->setMsgdb( m_msgdb, m_pMutexForMsgDB );
    //二级警报 信息
    m_warnline_2nd_dlg = new QShowWarnDlg(TWO_LEVEL, this );
    m_warnline_2nd_dlg->hide();
    m_warnline_2nd_dlg->setMsgdb( m_msgdb, m_pMutexForMsgDB );

    m_innerwidget = new QAllPanelTableWidget( this );
    m_innerwidget->setGridStyle(Qt::NoPen);
    m_innerwidget->setFrameShape(QFrame::NoFrame);

    m_innerwidget->verticalHeader()->setVisible(false );
    m_innerwidget->horizontalHeader()->setVisible( false );
    m_innerwidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //设置背景为透明
    QPalette pll = m_innerwidget->palette();  
    pll.setBrush(QPalette::Base,QBrush(QColor(255,255,255,0)));  
    m_innerwidget->setPalette(pll);


    connect( m_pButtonExit, SIGNAL(clicked()), this, SLOT(ExitSlot()));
    connect( m_pButtonImageShow, SIGNAL(clicked()), this, SLOT(ImageShowSlot()));
    connect( m_pButtonDataShow, SIGNAL(clicked()), this, SLOT(DataShowSlot()));
    connect( m_pButtonParamsSet, SIGNAL(clicked()), this, SLOT(ParamSetSlot()));
    connect( m_ButtonPanelShow, SIGNAL(clicked()), this, SLOT(PanelShowSlot()));
    connect( m_pButtonExport, SIGNAL(clicked()), this, SLOT(ExportShowSlot()));
    connect( m_pButtonWarningInfo, SIGNAL(clicked()), this, SLOT(AllWarnInfoShow()));

    connect( m_warnline_1st, SIGNAL(clicked()), this, SLOT(ShowWarning1st()));
    connect( m_warnline_2nd, SIGNAL(clicked()), this, SLOT(ShowWarning2nd()));
    connect( m_warn_clean, SIGNAL(clicked()), this, SLOT(CleanWarningInfo()));

    connect( m_dataShow, SIGNAL(clicked()), this, SLOT(ChangeDeviceSlot()));
    connect( m_dataShow, SIGNAL(BeginQueryData()), m_wait_dlg, SLOT(exec()));
    connect( m_dataShow, SIGNAL(QueryDataOver()), this, SLOT(QueryWarnInfoSlot()));
    connect( m_photoShow, SIGNAL(clicked()), this, SLOT(ChangeDeviceSlot()));
    connect( m_paramSetWidget, SIGNAL(clicked()), this, SLOT(ChangeDeviceSlot()));
    connect( m_paramSetWidget, SIGNAL(SaveParamSignal(QString)), this, SLOT(SaveParamSlot(QString)));
    connect( m_paramSetWidget, SIGNAL(SaveWarnParam( int , int )), this, SLOT(SaveWarnParamSlot(int, int)));
    connect( m_allWarnInfoWidget, SIGNAL(QueryMsgOver()), this, SLOT(QueryWarnInfoSlot()));

    connect( m_exportWidget, SIGNAL(BeginExport(QString)), this, SLOT(DataToExcelBegin(QString)));
	connect( m_exportWidget, SIGNAL(BeginLoadData(QString)), this, SLOT(DataToExcelBegin(QString)));
    connect( m_exportWidget, SIGNAL(ExportCompelete(int)), this , SLOT(DataToExcelEnd(int)));
}

void H2SMonitorClient::BuildBtnWidget()
{
    if( m_innerwidget->rowCount() != 0 )
        m_innerwidget->clear();
    QRect thisRect = geometry();
    QTextCodec::setCodecForLocale( QTextCodec::codecForName("GB2312"));
    int icount = m_deviceListInfo.count();
    m_innerwidget->setRowCount( icount );
    m_innerwidget->setColumnCount( 2 );
    m_innerwidget->setColumnWidth(0, 30 );
    m_innerwidget->setColumnWidth(1, 100);
    m_innerwidget->setMinimumHeight( 400 );
    
    m_innerwidget->setGeometry(thisRect.width()*0.02,thisRect.height()*0.26,thisRect.width()*0.19, /*thisRect.height()*0.55)*/400);
    QRect rc = m_innerwidget->geometry();
    for( int i = 0; i < m_btn_instrument.count(); i++ )
    {
        
        QImage image1;
        image1.load(":/images/images/status_left1.png");
        QImage image2;
        image2.load(":/images/images/status_left2.png");
        QStateButton *states = new QStateButton( image1,image2, m_innerwidget );
        states->setStateEnable( false );
        states->setMaximumWidth( 28 );
        states->setMaximumHeight( 30 );

        QString strName = m_btn_instrument[i].deviceName ;
        QPushButton *pb_btn = new QPushButton(strName, m_innerwidget );
        if( i == 0 )
        {
            pb_btn->setStyleSheet("QPushButton { background-color: rgb(78,156,236); border-radius: 3px; color: rgb(255, 255, 255); }QPushButton:hover { background-color: rgb(245, 69, 234); }");
        }
        else
        {
            pb_btn->setStyleSheet("QPushButton { background-color: rgb(255, 132, 139); border-radius: 3px; color: rgb(255, 255, 255); } QPushButton:hover { background-color: rgb(245, 69, 234); }");
        }
        
        m_innerwidget->setCellWidget(i, 0, states);
        m_innerwidget->setCellWidget(i, 1, pb_btn );
        //pb_btn->setAutoFillBackground(true);
        states->setMaximumWidth( 100 );
        pb_btn->setMaximumHeight( 30 );
        pb_btn->setMinimumWidth(150);
    //	pb_btn->setGeometry(rc.x() + 55, rc.y() + m_leftBtnHight,pb_btn->geometry().width(), pb_btn->geometry().height());
        connect( pb_btn, SIGNAL(clicked()), this, SLOT(InstrumentClickSlot()));
        m_btn_instrument[i].btn = pb_btn;
        m_btn_instrument[i].states = states;

        m_leftBtnHight += 30;
    }
    
    m_innerwidget->show();
}

void H2SMonitorClient::BuildAllPannel()
{	
    m_allPannelInnerwidget->InitIntrumentInfo(m_btn_instrument);
}

void H2SMonitorClient::BuildAllSpline()
{
    QVector<QVector<double> > vt_vy;
    QString msg = QString::fromLocal8Bit("全部仪器");
    int icount = m_btn_instrument.count()*2;
    for( int i = 0; i < icount; ++i )
    {
        QVector<double> yval;
        for( int j = 0; j < POINT_COUNT;j++ )
        {
            yval.push_back(0.0);
        }
        vt_vy.push_back( yval );
    }
    m_allSplineShow->setCurvcnt( msg, icount, m_btn_instrument );
}
//美化主画面方法
void H2SMonitorClient::BeautifyUI()
{
  
}

// 翻译界面元素
void H2SMonitorClient::TranslateUI()
{
    // do nothing
}

// 连接与本界面相关的信号和槽
void H2SMonitorClient::BindSlots()
{
    // do nothing
}

void H2SMonitorClient::BindLoginForm(CFormLogin * formLogin)
{
    m_pFormLogin = formLogin;

    connect(formLogin, SIGNAL(LoginCompleted(bool)), this, SLOT(HandleLoginResult(bool)));
    connect( formLogin, SIGNAL(SendUserName(QString,QString)), this, SLOT(GetUserNameSlot(QString,QString)));
}

void H2SMonitorClient::HandleLoginResult(bool loginSucceed)
{
    if ( loginSucceed) {
       if(  isHidden() )
            this->show();
        
        QDesktopWidget* desktop = QApplication::desktop();
        this->move((desktop->width() - this->width())/2, (desktop->height() - this->height())/2);
        qDebug() << "Login succeed, and mainWindow show.";
        GetLoginInfo();
        GetDeviceListInfo();
        m_isLogin = true;

    } else {
        qDebug() << "LoginResult: " << loginSucceed 
            << " ,mainWindows isVisible:" << this->isVisible();
        this->close();
    }
}

void H2SMonitorClient::GetUserNameSlot(QString strname ,QString pwd )
{
    m_userName = strname;
    m_userPwd = pwd;
    QString str = QString::fromLocal8Bit("欢迎您！  ");//TODO:  tangqiao 2015/12/24
    str += m_userName;
    str += "  ";
    str += QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm");
    str += "  ";
    str +=  QDate::longDayName(QDate::currentDate().dayOfWeek());
    m_lb_user->setText( str );

    QString titleFormat = QStringLiteral("欢迎您！  %1  %2  %3");

    str = m_userName;
    str += "#";
    str += QString::fromLocal8Bit("登录成功!");
    m_state_widget->setMsg( str );
    m_messageThread->setReConnectInfo( m_userName, m_userPwd );
}
void H2SMonitorClient::paintEvent(QPaintEvent *p)
{
    QPainter painter(this);
    QImage image;
    QRect thisRect = this->geometry();
    image.load(QStringLiteral(":/images/images/top1.png"));
    painter.drawImage(QRect(0,0, this->size().width(),96/*thisRect.height()*0.135*/ ), image);

    image.load(":images/images/top2.png");
    painter.drawImage(QRect(0,0, 374,100/*thisRect.height()*0.137*/ ), image);

    image.load( ":images/images/top3.png");
    painter.drawImage(QRect(350,89, thisRect.width()-350,11/*thisRect.height()*0.137*/ ), image);

    image.load(":/images/images/Status_bar.png");
    painter.drawImage(QRect(0,thisRect.height()*0.01+96, this->size().width(), thisRect.height()*0.065 ), image);

    image.load(":/images/images/mian_left.png");
    painter.drawImage(QRect(20,thisRect.height()*0.085+96, thisRect.width()*0.19, thisRect.height()*0.63), image);

    /*image.load(":/images/images/status.png");
    painter.drawImage(QRect(20,thisRect.height()*0.83, thisRect.width()*0.19, thisRect.height()*0.11), image);*/

    image.load(":/images/images/main_right.png");
    painter.drawImage(QRect(thisRect.width()*0.22,thisRect.height()*0.085+96, thisRect.width()*0.77, thisRect.height()*0.77), image);

    image.load(":/images/images/mian_botton.png");
    painter.drawImage(QRect(20,thisRect.height()*0.97,  thisRect.width(), thisRect.height()*0.03), image);
}
void H2SMonitorClient::resizeEvent(QResizeEvent *e)
{
    QMainWindow::resizeEvent( e );
    QRect thisRect = geometry();
    m_lb_title1->move(thisRect.width()*0.03,15 /*thisRect.height()*0.03*/ );
    m_lb_title2->move(thisRect.width()*0.03, 70/*thisRect.height()*0.095*/ );
    m_ButtonPanelShow->move( thisRect.width()*0.38,15 /*thisRect.height()*0.03*/);
    m_lb_show->move( thisRect.width()*0.38, 70/*thisRect.height()*0.095*/ );

    m_pButtonParamsSet->move(  thisRect.width()*0.47,15/* thisRect.height()*0.03*/);
    m_lb_set->move( thisRect.width()*0.47,70 /*thisRect.height()*0.095*/ );

    m_pButtonDataShow->move( thisRect.width()*0.56,15 /*thisRect.height()*0.03*/);
    m_lb_data->move(thisRect.width()*0.56,70 /*thisRect.height()*0.095*/ );

    m_pButtonImageShow->move( thisRect.width()*0.65, 15/*thisRect.height()*0.03*/);
    m_lb_image->move(thisRect.width()*0.65, 70/*thisRect.height()*0.095*/);

    m_pButtonExport->move(thisRect.width()*0.74,15);
    m_lb_export->move( thisRect.width()*0.74, 70);

    m_pButtonWarningInfo->move( thisRect.width()*0.83, 15);
    m_lb_warnInfo->move( thisRect.width()*0.83, 70);


    m_pButtonExit->move( thisRect.width()*0.92, 15/*thisRect.height()*0.03*/);
    m_lb_exit->move(thisRect.width()*0.92, 70/*thisRect.height()*0.095*/);

    m_lb_wel->move(thisRect.width()*0.02,thisRect.height()*0.03+96/* thisRect.height()*0.17*/);
    m_lb_user->move(thisRect.width()*0.4 ,thisRect.height()*0.03+96/*thisRect.height()*0.17*/);
    m_lb_panel->move(thisRect.width()*0.07,thisRect.height()*0.075+96);
    m_lb_monit->move(thisRect.width()*0.23, thisRect.height()*0.075+96);
    m_lb_ver->move(thisRect.width()*0.35, thisRect.height()* 0.97);
    
    m_pTabWidget->setGeometry(thisRect.width()*0.23,thisRect.height()*0.26, thisRect.width()*0.75, thisRect.height()*0.70 );
    // m_allPannelInnerwidget->resize(thisRect.width()*0.77, thisRect.height()*0.72);
    m_pDeviceDataCurveContainer->resize(thisRect.width()*0.77, thisRect.height()*0.72);
    m_allSplineInnerwidget->resize(thisRect.width()*0.77, thisRect.height()*0.72);
    m_allSplineShow->setGeometry(0, 0, thisRect.width()*0.77, thisRect.height()*0.72 );

    m_dataShow->setGeometry(thisRect.width()*0.23,thisRect.height()*0.26, thisRect.width()*0.75, thisRect.height()*0.70 );
    m_paramSetWidget->setGeometry(thisRect.width()*0.23,thisRect.height()*0.26, thisRect.width()*0.75, thisRect.height()*0.70 );
    m_photoShow->setGeometry(thisRect.width()*0.23,thisRect.height()*0.26, thisRect.width()*0.75, thisRect.height()*0.70 );
    m_exportWidget->setGeometry(thisRect.width()*0.23,thisRect.height()*0.26, thisRect.width()*0.75, thisRect.height()*0.70 );
    m_allWarnInfoWidget->setGeometry(thisRect.width()*0.23,thisRect.height()*0.26, thisRect.width()*0.75, thisRect.height()*0.70 );

    m_warnline_1st->setGeometry(thisRect.width()*0.8, thisRect.height()*0.02+96, m_warnline_1st->width(), m_warnline_1st->height());
    m_warnline_1str_count->move(m_warnline_1st->geometry().x()+20,m_warnline_1st->geometry().y()+20  );
    m_warnline_2nd->setGeometry(thisRect.width()*0.85, thisRect.height()*0.02+96, m_warnline_2nd->width(), m_warnline_2nd->height());
    m_warnline_2nd_count->move(m_warnline_2nd->geometry().x()+20,m_warnline_2nd->geometry().y()+20);
    m_warn_clean->setGeometry(thisRect.width()*0.9, thisRect.height()*0.015+96, 45, 45 );
    m_state_widget->setGeometry( thisRect.width()*0.015, thisRect.height()*0.83, thisRect.width()*0.2, thisRect.height()*0.14);

    m_innerwidget->setGeometry(thisRect.width()*0.02,thisRect.height()*0.26,thisRect.width()*0.18, thisRect.height()*0.55);

}

void H2SMonitorClient::PanelShowSlot()
{
    QString str = m_userName;
    str += "#";
    str += QString::fromLocal8Bit("查看所有数据!");
    m_state_widget->setMsg( str );
    m_dataShow->hide();
    m_photoShow->hide();
    m_paramSetWidget->hide();
    m_exportWidget->hide();
    m_allWarnInfoWidget->hide();
    m_pTabWidget->show();
}
void H2SMonitorClient::ParamSetSlot()
{
    m_dataShow->hide();
    m_photoShow->hide();
    m_pTabWidget->hide();
    m_exportWidget->hide();
    m_allWarnInfoWidget->hide();
    SetParamSetDlg(m_currentDeviceNo);
    m_paramSetWidget->show();

}
void H2SMonitorClient::ImageShowSlot()
{
    QString str = m_userName;
    str += "#";
    str += QString::fromLocal8Bit("查看数据!");
    m_pTabWidget->hide();
    m_dataShow->hide();
    m_paramSetWidget->hide();
    m_exportWidget->hide();
    m_allWarnInfoWidget->hide();
    SetPhotoShowDlg(m_currentDeviceNo);
    m_photoShow->show();
}
void H2SMonitorClient::DataShowSlot()
{
    QString str = m_btn_instrument[m_currentDeviceNo].deviceno;
    str += "#";
    str += QString::fromLocal8Bit("查看数据!");
    m_pTabWidget->hide();
    m_photoShow->hide();
    m_paramSetWidget->hide();
    m_exportWidget->hide();
    m_allWarnInfoWidget->hide();
    SetDataShowDlg( m_currentDeviceNo );
    m_dataShow->show();
}
void H2SMonitorClient::ExportShowSlot()
{
    m_pTabWidget->hide();
    m_photoShow->hide();
    m_paramSetWidget->hide();
    m_dataShow->hide();
    m_allWarnInfoWidget->hide();
    m_exportWidget->show();
}
void H2SMonitorClient::AllWarnInfoShow()
{
    m_pTabWidget->hide();
    m_photoShow->hide();
    m_paramSetWidget->hide();
    m_dataShow->hide();
    m_exportWidget->hide();
	m_allWarnInfoWidget->SetBufferMsg(m_warnline_1st_dlg->getMsg(), 1 );
	m_allWarnInfoWidget->SetBufferMsg( m_warnline_2nd_dlg->getMsg(), 2 );
    m_allWarnInfoWidget->show();
    m_allWarnInfoWidget->BeginQueryMsg();
    m_wait_dlg->exec();
}
void H2SMonitorClient::ExitSlot()
{
    QMessageBox box(QMessageBox::Question,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("是否确定退出？"));
    QPushButton *okButton = (box.addButton(QString::fromLocal8Bit("确定"),QMessageBox::AcceptRole));
    QPushButton *cancelButton = (box.addButton(QString::fromLocal8Bit("取消"), QMessageBox::RejectRole));
    box.exec();
    if( box.clickedButton() == okButton )
    {
        if( QCheckRecvQueueThread::GetInstance()->isRunning())
        {
            QCheckRecvQueueThread::GetInstance()->QuitThread();
            QCheckRecvQueueThread::GetInstance()->wait();
        }
        CMessage *message = new CMessage();
        message->m_header.m_struct.cmd = PC_CLIENT_CMD_EXIT;
        message->m_header.m_struct.messageNo = 2;
        message->m_header.m_struct.cmdResult = 1;// 非0表示 需要获取错误信息
		if( CMessageQueue::getInstance()->GetConnectState() )
				 m_messageThread->SendMessage( message );
        close();
    }
}

void H2SMonitorClient::GetDeviceListInfo()
{
    for( int i = 0; i < m_btn_instrument.count(); ++i )
    {
        if( m_btn_instrument[i].btn )
            delete m_btn_instrument[i].btn;
        if( m_btn_instrument[i].states )
            delete m_btn_instrument[i].states;
    }
    m_btn_instrument.clear();
    m_deviceListInfo.clear();
    CMessage * message = new CMessage();
    message->m_header.m_struct.cmd = PC_CLIENT_CMD_GET_DEVICE_LIST;
    message->m_header.m_struct.messageNo = 2;
    message->m_header.m_struct.cmdResult = 1;
    m_messageThread->SendMessage(message);
    m_timer->start( 20000 );
    m_wait_dlg->exec();
}

void H2SMonitorClient::ServerRepInfoSlot(CMessage* msg )
{
    if( msg == NULL )
        return;
    if( msg->m_header.m_struct.cmd == PC_SERVER_CMD_GET_DEVICE_LIST_REP && msg->m_header.m_struct.cmdResult == 0 )
    {
        QString str = m_userName;
        str += "#";
        str += QString::fromLocal8Bit("获取设备列表成功!");
        m_state_widget->setMsg( str );
        m_wait_dlg->hide();
        if( m_timer->isActive() )
        {
            m_timer->stop();
        }
        CMessage *devicesinfo = msg ;//m_messageThread->SysQueuePopFront();
        CMessageGetDeviceListRep* devicerep = static_cast<CMessageGetDeviceListRep*>(devicesinfo->m_pBody);
         QList<CMessageDevicesListItem *> listinfo = devicerep->m_lstDevices;
         for( int i = 0; i < listinfo.count(); ++i )
         {
             m_deviceListInfo.push_back( listinfo[i]->m_struct );
            QString samplestr = listinfo[i]->GetSampleRate();
            QString strName;
            //	QTextStream stream(&strName);
            char *str;
            str = (char*)listinfo[i]->m_struct.deviceName ;
            //strName = stream.readAll();
            //stream.flush();
            strName = QString::fromLocal8Bit(str);
            BUTTON_INSTRUMENT st;
            st.isSelect = false;
            st.states = NULL;
            st.btn = NULL;
            str = (char *)listinfo[i]->m_struct.deviceId;
            st.deviceid = QString::fromLocal8Bit( str );
            st.deviceid = st.deviceid.left(36);
            qint8 itype = m_deviceListInfo[i].devType;
            st.devType = QString::fromLocal8Bit( "%1" ).arg(itype);
            str = (char *)listinfo[i]->m_struct.deviceNo;
            st.deviceno = QString::fromLocal8Bit( str );
            st.deviceName = strName;	
            st.flag = false;
            str = (char*)listinfo[i]->m_struct.warningLine1st;
            st.warnLine1st = QString::fromLocal8Bit( str );
            str = (char*)listinfo[i]->m_struct.warningLine2nd;
            st.warnLine2nd = QString::fromLocal8Bit( str );
            str = (char *)listinfo[i]->m_struct.maxRange;
            st.maxRange = QString::fromLocal8Bit( str );
			st.recvTime = QDateTime();
            //QString strtmp = (char*)m_deviceListInfo[i].sampleRateLen;
            QStringList strsamp = samplestr.split("#");
            if( strsamp.count() == 2 )
            {
                st.sample1 = strsamp[0];
                st.sample2 = strsamp[0];
            }
            else
            {
                st.sample1 = "10";
                st.sample2 = "10";
            }
            m_btn_instrument.push_back( st );
         }
         m_recvXYData.clear();
         m_recvXYData.resize( m_deviceListInfo.count()*2 );
         m_recvAveData.clear();
         m_recvAveData.resize( m_deviceListInfo.count()*2);
         m_recvCaculate.clear();
         m_recvCaculate.resize( m_deviceListInfo.count()*2 );

         BuildBtnWidget();
         BuildAllPannel();
         BuildAllSpline();
		 m_exportWidget->setDeviceInfo(m_btn_instrument);
         m_tm_UpdateTimer->setInterval( 10000 );
         m_tm_UpdateTimer->start(  );
         delete msg;
		 if( m_tm_HeartBeat->isActive() )
		 {
			 m_tm_HeartBeat->stop();
		 }
		  QCheckRecvDataQueueThread::GetInstance()->setRecvData(true);
		 m_tm_HeartBeat->start();
    }
    else if( msg->m_header.m_struct.cmd == PC_SERVER_CMD_SET_DEVICE_PARAM_REP && msg->m_header.m_struct.cmdResult == 0)
    {
        QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("参数设置成功!"));	
        delete msg;
		 QCheckRecvDataQueueThread::GetInstance()->setRecvData(true);
    }
	else if( msg->m_header.m_struct.cmd == PC_SERVER_CMD_SEND_HEARTBEAT_REP && msg->m_header.m_struct.messageNo == 3 && msg->m_header.m_struct.cmdResult == 0 )
	{
		CMessageHeartbeat *msg_heart = static_cast<CMessageHeartbeat*>(msg->m_pBody);
		QString strtime = QDateTime::fromMSecsSinceEpoch( msg_heart->m_struct.heartbeatTime).toString("yyyy-MM-dd hh:mm");
		 QString weekday = QDate::longDayName(QDateTime::fromMSecsSinceEpoch(msg_heart->m_struct.heartbeatTime).date().dayOfWeek());
		UpdateServerTime(strtime, weekday);
	}   
}
void H2SMonitorClient::TimeoutSlot()
{
    QString str = m_userName;
    str += "#";
    str += QString::fromLocal8Bit("获取设备列表超时!");
    m_state_widget->setMsg( str );
    m_wait_dlg->hide();
    QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("设备信息查询超时！,请重新启动！"));
    //emit GetDeviceInfoFalse();
    this->close();
    //BuildBtnWidget();
    //BuildAllPannel();
    //BuildAllSpline();
    repaint();
}

void H2SMonitorClient::setMessageThread(QMessageThread *messageThread)
{
    m_messageThread = messageThread;
    //connect( m_messageThread, SIGNAL(ServerRepData()), this, SLOT(ServerRepDataSlot()));
   // connect( m_messageThread, SIGNAL(ServerDisconnect()), this, SLOT(ServerDisconnectSlot()));
	//connect( m_messageThread, SIGNAL(ServierConnect()), this, SLOT(ServerConnectSlot()));
	
    QCheckRecvDataQueueThread::GetInstance()->SetMessageThread(messageThread );
	m_exportWidget->setMessageThread( messageThread,m_saveDataThread);
    if( !m_messageThread->isRunning() )
    {
        m_messageThread->start();
    }
    connect( QCheckRecvQueueThread::GetInstance(),SIGNAL(ServerRepInfo(CMessage*)), this, SLOT(ServerRepInfoSlot(CMessage*)));
    connect( QCheckRecvDataQueueThread::GetInstance(), SIGNAL(ServerRepData(const CMessage *)), this, SLOT(ServerRepDataSlot(const CMessage *)));
    if( !QCheckRecvQueueThread::GetInstance()->isRunning() )
    {
        QCheckRecvQueueThread::GetInstance()->start();
    }
    if( !QCheckRecvDataQueueThread::GetInstance()->isRunning())
    {
        QCheckRecvDataQueueThread::GetInstance()->start();
    }
}

void H2SMonitorClient::InstrumentClickSlot()
{
    QPushButton *psender = (QPushButton*)sender();
    for( int i = 0; i < m_btn_instrument.count(); ++i )
    {
        
        if(psender == m_btn_instrument[i].btn )
        {
            m_currentDeviceNo = i;
            setBtnBackGround( i );
            QString msg = QString("%1").arg( m_btn_instrument[i].deviceno );
            if( !m_photoShow->isHidden())
                SetPhotoShowDlg(m_currentDeviceNo);
            if( !m_dataShow->isHidden() )
                SetDataShowDlg( m_currentDeviceNo );
            if( !m_paramSetWidget->isHidden() )
                SetParamSetDlg( m_currentDeviceNo );
            break;
        }
    }
}
void H2SMonitorClient::SetDataShowDlg(int index)
{
    if( m_btn_instrument.count() <= index )
    {
        return;
    }
    QString strName = m_btn_instrument[index].deviceName;
    strName += "_";
    strName += m_btn_instrument[index].deviceno;
    m_dataShow->setCurrentDevice( strName);
    m_dataShowInfo[0].datetime = m_btn_instrument[index].datetimes1;
    m_dataShowInfo[0].latitude = QString("%1").arg( m_btn_instrument[index].latitude1);
    m_dataShowInfo[0].longitude = QString("%1").arg(m_btn_instrument[index].longitude1);
    m_dataShowInfo[0].value = m_btn_instrument[index].value1;
    m_dataShowInfo[0].types = m_btn_instrument[index].types1;
    m_dataShowInfo[0].units = m_btn_instrument[index].units1;

    m_dataShowInfo[1].datetime = m_btn_instrument[index].datetimes2;
    m_dataShowInfo[1].latitude = QString("%1").arg(m_btn_instrument[index].latitude2);
    m_dataShowInfo[1].longitude = QString("%1").arg(m_btn_instrument[index].longitude2);
    m_dataShowInfo[1].value = m_btn_instrument[index].value2;
    m_dataShowInfo[1].types = m_btn_instrument[index].types2;
    m_dataShowInfo[1].units = m_btn_instrument[index].units2;
    QString strtmp = m_btn_instrument[index].maxRange;
    QStringList maxlist = strtmp.split("#");
    if( maxlist.count() == 2 )
    {
        m_dataShow->setDialMaxRange( maxlist[0].toDouble(), maxlist[1].toDouble());
    }
    else 
    {
        m_dataShow->setDialMaxRange();
    }
    m_dataShow->setInfo( m_dataShowInfo , m_recvXYData[index*2], m_recvXYData[index*2+1], m_recvAveData[index*2], m_recvAveData[index*2+1]);
}

void H2SMonitorClient::SetPhotoShowDlg( int index )
{
    if( m_btn_instrument.count() <= index )
    {
        return;
    }
    QString strName = m_btn_instrument[index].deviceName;
    strName += "_";
    strName += m_btn_instrument[index].deviceno;

    QVector<XY_VALUE> xy_vt;
    xy_vt.push_back( m_recvXYData[index*2] );
    xy_vt.push_back( m_recvXYData[index*2+1] );
    
    m_photoShow->ShowSpline( strName, xy_vt);
}
void H2SMonitorClient::SetParamSetDlg(int index)
{
    PARAM_SET st;
    st.code = m_btn_instrument[m_currentDeviceNo].deviceno;
    st.name = m_btn_instrument[m_currentDeviceNo].deviceName;
    st.types = m_btn_instrument[m_currentDeviceNo].devType;
    st.maxs = m_btn_instrument[m_currentDeviceNo].maxRange;
    st.warn_one = m_btn_instrument[m_currentDeviceNo].warnLine1st;
    st.warn_two = m_btn_instrument[m_currentDeviceNo].warnLine2nd;
    st.sample = m_btn_instrument[m_currentDeviceNo].sample1;
    st.sample += "#";
    st.sample += m_btn_instrument[m_currentDeviceNo].sample2;
    m_paramSetWidget->InitInfo( st );
}
void H2SMonitorClient::ChangeDeviceSlot()
{
    QWidget *psender = (QWidget*)sender();
    if( m_currentDeviceNo == m_btn_instrument.count()-1 )
        m_currentDeviceNo = -1;
    m_currentDeviceNo++;
    setBtnBackGround( m_currentDeviceNo );
    if( psender == m_dataShow )
        SetDataShowDlg( m_currentDeviceNo );
    else if( psender == m_photoShow)
    {
        SetPhotoShowDlg( m_currentDeviceNo );		
    }
    else if( psender == m_paramSetWidget )
    {		
        SetParamSetDlg(m_currentDeviceNo);
    }
}

void H2SMonitorClient::GetLoginInfo()
{
    
}
void H2SMonitorClient::ServerRepDataSlot(const CMessage *msg)
{
    static int doCount = 0;
    int deviceno = -1;
    DATA_INFO *st = new DATA_INFO();
    PANNEL_DATA st_panel;
    //CMessage *msg = m_messageThread->DataQueuePopFront();
    if( msg == NULL )
        return;
    if( msg->m_header.m_struct.cmd == PC_SERVER_CMD_BROADCAST_REALTIME_DATA && msg->m_header.m_struct.cmdResult == 0 )
    {
        if( m_btn_instrument.count() == 0 )
        {
            delete msg;
            msg = NULL;
            return;
        }
            
        CMessageBroadcastDeviceRealtimeData *msgdata = static_cast<CMessageBroadcastDeviceRealtimeData*>(msg->m_pBody);
		unsigned char battery = msgdata->m_struct.realtimeData.reserved;
		batteryValue( battery );
        char *str = (char*)msgdata->m_struct.deviceId;
        QString strid = QString::fromLocal8Bit( str );
        strid = strid.left(36);
        QString strchannel;
        QString strtmp ;
        int index = -1;
        QString strType;
        
        qDebug() <<"out queue: " <<  msgdata->m_struct.data ;
        for( int i = 0; i < m_btn_instrument.count(); ++i )
        {

            if( m_btn_instrument[i].deviceid == strid  )
            {
                st->name = m_btn_instrument[i].deviceName;
                st->id = m_btn_instrument[i].deviceno;
                st_panel.deviceid = st->id;
				st_panel.battery = QString("%1%").arg( QString::number(battery));              
                deviceno = i;
                /*strtmp = m_btn_instrument[i].warnLine1st;
                QStringList str1stlist = strtmp.split("#");
                strtmp = m_btn_instrument[i].warnLine2nd;
                QStringList str2ndlist = strtmp.split("#");*/
                strchannel = QString::fromLocal8Bit( "%1" ).arg(msgdata->m_struct.realtimeData.channelNo);
                strtmp = m_btn_instrument[i].deviceno;
                strtmp +="_";
                strtmp += strchannel;
                strtmp +="#";
                strtmp +=QString::fromLocal8Bit("收到数据");
                m_state_widget->setMsg( strtmp );
                 m_btn_instrument[i].flag = true;
				 m_btn_instrument[i].recvTime = QDateTime::currentDateTime();
				 m_btn_instrument[i].states->setStateEnable( true );
                // 设置报警值时，不同气体的报警值范围不一样，根据通道号无法确定是哪种气体，这里只判断气体类型  tangqiao 2015/12/25
                if (H2S == msgdata->m_struct.realtimeData.datatype)
                {
                    double data = msgdata->m_struct.data > 100.0 ? 100.0 : msgdata->m_struct.data;
                    st_panel.probe = 1;
                    st_panel.value = QString("%1").arg(data, 0, 'f', 1);
                    m_btn_instrument[i].types1 = GetGasDisplayNameByCode(msgdata->m_struct.realtimeData.datatype );
                    strType = m_btn_instrument[i].types1;
                    strtmp = QString::fromLocal8Bit( "%1" ).arg(msgdata->m_struct.realtimeData.timestamp);
                    m_btn_instrument[i].datetimes1 = strtmp;
                    st->times = strtmp;                   
                    m_btn_instrument[i].latitude1 = msgdata->m_struct.realtimeData.latitude;
                    m_btn_instrument[i].longitude1 = msgdata->m_struct.realtimeData.longitude;
                    // H2S气体值超量程(100ppm)时，设置为最大量程 tangqiao 2015/12/25
                    m_btn_instrument[i].value1 = msgdata->m_struct.data > 100.0 ? 100.0 : msgdata->m_struct.data;
                    m_btn_instrument[i].units1 = GetMeasurementDisplayNameByCode(msgdata->m_struct.realtimeData.data_[6] );
                    st_panel.units = m_btn_instrument[i].units1;
                    st->types = "probe1:";
                    st->types += m_btn_instrument[i].types1;
                    m_recvXYData[i*2].y_value.push_back( msgdata->m_struct.data );
                    st->values = QString::number( msgdata->m_struct.data,'f' );
                    m_recvXYData[i*2].x_value.push_back( msgdata->m_struct.realtimeData.timestamp);
                    st->times = QString::number( msgdata->m_struct.realtimeData.timestamp );
                    if( m_recvXYData[i*2].y_value.count() >POINT_COUNT )
                    {
                        //st->values = QString::number( m_recvXYData[i*2].y_value.takeFirst(),'f' );
                        m_recvXYData[i*2].y_value.takeFirst();
                    }
                    if( m_recvXYData[i*2].x_value.count() >POINT_COUNT )
                    {
                        //st->times = QString::number( m_recvXYData[i*2].x_value.takeFirst(),'f' );
                         m_recvXYData[i*2].x_value.takeFirst();
                    }
                     m_vtbasedata.push_back( st );
                    //yvalue[i*2] = msgdata->m_struct.data;
                    index = i * 2;
                    //WarnTestValue(msgdata->m_struct.data, 1, i);
                    AveCalculate(msgdata->m_struct.realtimeData.timestamp, msgdata->m_struct.data, i, 1, st);
                }
                //else if( strchannel == "2" )
                else if (SO2 == msgdata->m_struct.realtimeData.datatype)
                {
                    double data = msgdata->m_struct.data > 20.0 ? 20.0 : msgdata->m_struct.data;
                    st_panel.probe = 2;
                    st_panel.value = QString("%1").arg(data, 0, 'f', 2);
                    m_btn_instrument[i].types2 = GetGasDisplayNameByCode(msgdata->m_struct.realtimeData.datatype );
                    strtmp = QString::fromLocal8Bit( "%1" ).arg(msgdata->m_struct.realtimeData.timestamp);
                    strType = m_btn_instrument[i].types2;
                    m_btn_instrument[i].datetimes2 = strtmp;
                    //st.times = strtmp;
                  
                    m_btn_instrument[i].latitude2 = msgdata->m_struct.realtimeData.latitude;
                    m_btn_instrument[i].longitude2 = msgdata->m_struct.realtimeData.longitude;
                    // SO2气体值超量程后，设置为最大量程值(20ppm) tangqiao 2015/12/25
                    m_btn_instrument[i].value2 = msgdata->m_struct.data > 20.0 ? 20.0 : msgdata->m_struct.data;
                    m_btn_instrument[i].units2 = GetMeasurementDisplayNameByCode(msgdata->m_struct.realtimeData.data_[6] );
                    st_panel.units = m_btn_instrument[i].units2;
                    st->types = "probe2:";
                    st->types += m_btn_instrument[i].types2;
                    m_recvXYData[i*2+1].y_value.push_back( msgdata->m_struct.data );
                    st->values = QString::number( msgdata->m_struct.data,'f' );
                    m_recvXYData[i*2+1].x_value.push_back( msgdata->m_struct.realtimeData.timestamp);
                    st->times = QString::number( msgdata->m_struct.realtimeData.timestamp );
                    if( m_recvXYData[i*2+1].y_value.count() >POINT_COUNT )
                    {
                    //	st->values = QString::number( m_recvXYData[i*2+1].y_value.takeFirst(),'f' );
                        m_recvXYData[i*2+1].y_value.takeFirst();
                    }
                    if( m_recvXYData[i*2+1].x_value.count() >POINT_COUNT )
                    {
                        //st->times = QString::number( m_recvXYData[i*2+1].x_value.takeFirst(),'f' );
                        m_recvXYData[i*2+1].x_value.takeFirst();
                    }
                     m_vtbasedata.push_back( st );
                    //yvalue[i*2+1] = msgdata->m_struct.data;
                    index = i * 2 + 1;
                    //WarnTestValue(msgdata->m_struct.data, 2, i);
                    AveCalculate(msgdata->m_struct.realtimeData.timestamp, msgdata->m_struct.data, i, 2, st);
                
                }
                break;
            }
        }
        if( index == -1 )
        {
            delete msg;
            msg = NULL;
            return;
        }
        st_panel.times = st->times; 
        st_panel.types = st->types;



        QDateTime datetime = QDateTime::fromMSecsSinceEpoch(st->times.toULongLong());
        QString time = datetime.toString("yyyy-MM-dd hh:mm");
        QString weekday = QDate::longDayName(datetime.date().dayOfWeek());
        UpdateServerTime(time, weekday);

        m_allPannelInnerwidget->UpdateInstrumentInfo(st_panel);
        m_allSplineShow->UpdateCurvcnt(msgdata->m_struct.data, index , msgdata->m_struct.realtimeData.timestamp);
        qDebug() << QDateTime::fromMSecsSinceEpoch(msgdata->m_struct.realtimeData.timestamp).toString("hh:mm:ss") << ":" << msgdata->m_struct.data;
        if( m_currentDeviceNo >= 0 && m_currentDeviceNo < m_btn_instrument.count() )
        {
            SetPhotoShowDlg(m_currentDeviceNo);
        }
        if(m_currentDeviceNo == deviceno && !m_dataShow->isHidden())
        {
            m_dataShow->UpdateInfo(strchannel.toInt(), msgdata->m_struct.realtimeData.timestamp, msgdata->m_struct.data);
        }
         
         qDebug() << "Do count:" << doCount;

        if( m_vtbasedata.count() > 5 )
        {
            m_saveDataThread->setDataInfo( m_vtbasedata );
            m_vtbasedata.clear();
        }
        delete msg;
        msg = NULL;
        doCount++;
    }
    
}

void H2SMonitorClient::SaveWarnParamSlot(int itype, int aveTime)
{
    m_aveTime = aveTime;
    if( itype == 1)
    {
        m_warnType = AVE_VALUE_WARN;
    }
    else if( itype == 2)
    {
        m_warnType = TEST_VALUE_WARN;
    }

    
}
void H2SMonitorClient::UpdateButtonState()
{
    m_connectCount = 0;
    for( int i = 0; i < m_btn_instrument.count(); ++i )
    {
		QString str = m_btn_instrument[i].recvTime.toString("yyyy-MM-dd hh:mm:ss");
		if( m_btn_instrument[i].recvTime.addSecs(120) <  QDateTime::currentDateTime() )
		{
			 m_btn_instrument[i].states->setStateEnable( false );
		}
        m_btn_instrument[i].flag  = false;
    }
    m_warnline_1st_dlg->setConnectCount( m_connectCount );
}

void H2SMonitorClient::SaveParamSlot(QString msg)
{
	
    if( !CMessageQueue::getInstance()->GetConnectState() )
    {
        QMessageBox box(QMessageBox::Question,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("客户端与服务器连接已断开，请稍后重试！"));
        QPushButton *okButton = (box.addButton(QString::fromLocal8Bit("确定"),QMessageBox::AcceptRole));		
        box.exec();
        return;
    }
     QCheckRecvDataQueueThread::GetInstance()->setRecvData(false);
    QStringList strlist = msg.split(";");
    Q_ASSERT(strlist.count() == 5);
    CMessage *message = new CMessage();
    message->m_header.m_struct.cmd = PC_CLIENT_CMD_SET_DEVICE_PARAM;
    message->m_header.m_struct.messageNo = 2;
    message->m_header.m_struct.cmdResult = 1;// 非0表示 需要获取错误信息
    CMessageSetDeviceParam *msg_set = new CMessageSetDeviceParam();
    QByteArray ba = m_btn_instrument[m_currentDeviceNo].deviceid.toLatin1();
    memcpy(msg_set->m_struct.deviceID, ba.data(), ba.count());
    ba = strlist[1].toLatin1();
    memcpy(msg_set->m_struct.warningLine1st, ba.data(), ba.count());
    ba = strlist[2].toLatin1();
    memcpy(msg_set->m_struct.warningLine2nd, ba.data(), ba.count() );
    ba = strlist[3].toLatin1();
    memcpy(msg_set->m_struct.maxRange, ba.data(), ba.count());
    //ba = strlist[0].toLatin1();
    msg_set->SetDeviceName( strlist[0]);
    msg_set->SetSampleRate( strlist[4]);

    QStringList listsamp = strlist[4].split("#");
    message->SetMessageBody(msg_set);
    m_messageThread->SendMessage(message);
    m_btn_instrument[m_currentDeviceNo].warnLine1st = strlist[1];
    m_btn_instrument[m_currentDeviceNo].warnLine2nd = strlist[2];
    m_btn_instrument[m_currentDeviceNo].maxRange = strlist[3];
    m_btn_instrument[m_currentDeviceNo].deviceName = strlist[0];
    m_btn_instrument[m_currentDeviceNo].btn->setText( strlist[0] );
    if( listsamp.count() == 2 )
    {
        m_btn_instrument[m_currentDeviceNo].sample1 = listsamp[0];
        m_btn_instrument[m_currentDeviceNo].sample2 = listsamp[1];
    }
    else
    {
        m_btn_instrument[m_currentDeviceNo].sample1 = "10";
        m_btn_instrument[m_currentDeviceNo].sample2 = "10";
    }
    
    m_allPannelInnerwidget->InitIntrumentInfo(m_btn_instrument);
    //QMessageBox::information( this, QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("参数已提交服务器！"));
}

void H2SMonitorClient::ShowWarning1st()
{
    m_warnline_1st_dlg->show();
}
void H2SMonitorClient::ShowWarning2nd()
{
    m_warnline_2nd_dlg->show();
}
void H2SMonitorClient::CleanWarningInfo()
{
    m_warnline_2nd_dlg->CleanMsg();
    m_warnline_1st_dlg->CleanMsg();
    m_1stcount = 0;
    m_2ndcount = 0;
	m_isFisrtPlayer = false;
	m_isSecondPlayer = false;
	m_WarnPlayer->stop();
    emit UpdateWarnSignal();	
}

void H2SMonitorClient::UpdateWarnState()
{
	
    m_warnline_1str_count->setText(QString("%1").arg(m_1stcount));
    m_warnline_2nd_count->setText( QString("%1").arg( m_2ndcount ));
    if( m_1stcount > 0 || m_2ndcount > 0 )
    {
        m_warn_clean->show();
        if(  m_1stcount >0 )
        {
            m_warnline_1str_count->show();
            m_warnline_1st->set1stWarning();
			if(m_isFisrtPlayer == false && m_2ndcount <= 0 && m_isSecondPlayer == false )
			{
				m_mediaList->setCurrentIndex( 0 );
				m_WarnPlayer->play();
				m_isFisrtPlayer = true;
			}			
        }
        if( m_2ndcount > 0 )
        {
            m_warnline_2nd_count->show();
            m_warnline_2nd->set2ndWarning();
			if( m_isFisrtPlayer == true) 
			{
				m_WarnPlayer->stop();
				m_isFisrtPlayer = false;
			}
			if( m_isSecondPlayer == false)
			{
				m_mediaList->setCurrentIndex( 1 );
				m_WarnPlayer->play();
				m_isSecondPlayer = true;
			}
			
        }
    }	
    else
    {
        m_warnline_1str_count->hide();
        m_warnline_2nd_count->hide();
        m_warn_clean->hide();
        m_warnline_1st->setNormal();
        m_warnline_2nd->setNormal();
    }
}
void H2SMonitorClient::ServerDisconnectSlot()
{
    QString msg = m_userName;
    msg += "#";
    msg += QString::fromLocal8Bit("与服务器连接中断，等待重新连接!");
    m_state_widget->setMsg( msg );
	if( m_tm_HeartBeat->isActive() )
	{
		m_tm_HeartBeat->stop();
	}
}
void H2SMonitorClient::ServerConnectSlot()
{
	if( m_isLogin )
	{
		if( !m_tm_HeartBeat->isActive() )
		{
			m_tm_HeartBeat->start();
		}
	}
	
}
void H2SMonitorClient::setBtnBackGround(int index)
{
     if( m_btn_instrument.count() == 0 || index < 0 || index >= m_btn_instrument.count() )
         return;
     m_btn_instrument[index].btn->setStyleSheet("QPushButton { background-color: rgb(78, 156, 236); border-radius: 3px; color: rgb(255, 255, 255); }QPushButton:hover { background-color: rgb(245, 69, 234); }");
    // m_btn_instrument[index].btn->update();border-image:url(:images/SimpleButtom.png) 

     for( int i = 0; i < m_btn_instrument.count(); ++i )
     {
         if( i != index )
         {
             m_btn_instrument[i].btn->setStyleSheet("QPushButton { background-color: rgb(255, 132, 139); border-radius: 3px; color: rgb(255, 255, 255); } QPushButton:hover { background-color: rgb(245, 69, 234); }");
            // m_btn_instrument[i].btn->update();
         }
     }

}
void H2SMonitorClient::QueryWarnInfoSlot()
{
    m_wait_dlg->hide();
}
void H2SMonitorClient::DataToExcelBegin(QString str)
{
    QString msg = m_userName;
    msg += "#";
    msg += str;
    m_state_widget->setMsg( msg );
    m_wait_dlg->exec();
}
void H2SMonitorClient::DataToExcelEnd(int index)
{
    m_wait_dlg->hide();
	if( index == 1 )
    {
		QString msg = m_userName;
		msg += "#";
		msg += QString::fromLocal8Bit("数据导出结束!");
		m_state_widget->setMsg( msg );
		QMessageBox::information( this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("数据导出结束！"));
        // 不打开保存目录
		//QString strPath = QDir::currentPath();//获取程序当前目录
		//strPath += "./export";
		//strPath.replace("/","\\");//将地址中的"/"替换为"\"，因为在Windows下使用的是"\"。  
		//QFileInfo dir(strPath);
		//if( dir.isDir() )
		//	QProcess::startDetached("explorer "+strPath);//打开上面获取的目录  
	}
    
}


void H2SMonitorClient::WarnTestValue(double value, int index, int probe)
{
    QString strtmp = m_btn_instrument[index].warnLine1st;
    QStringList str1stlist = strtmp.split("#");
    strtmp = m_btn_instrument[index].warnLine2nd;
    QStringList str2ndlist = strtmp.split("#");
    if( probe == 1 )
    {
        if( str1stlist.count() == 2 && str2ndlist.count() == 2 )
        {
            double warn1st = str1stlist[0].toDouble();
            double warn2nd = str2ndlist[0].toDouble();
            QString msg = m_btn_instrument[index].deviceno;
            msg += "_";
            msg += m_btn_instrument[index].deviceName;
            msg += "_";
            
            msg +=QString::fromLocal8Bit("探头 1 ");
            msg += m_btn_instrument[index].types1;
            msg +="_";
            if( value >= warn1st && value < warn2nd )
            {

                msg += QString("%1_%2").arg( warn1st ).arg( value,0,'f', 2);
                msg += "_";
                if( m_warnType == TEST_VALUE_WARN )
                {
                    msg +=QString::fromLocal8Bit("测量值：一级");
                }
                else if( m_warnType == AVE_VALUE_WARN )
                {
                    msg +=QString::fromLocal8Bit("平均值：一级");
                }			
                msg += "_";
                msg += m_btn_instrument[index].datetimes1;
                m_warnline_1st_dlg->AddMsg( msg );
                m_1stcount++;
                emit UpdateWarnSignal();
            }
            else if( value >= warn2nd )
            {							
                msg += QString("%1_%2").arg( warn2nd ).arg( value, 0, 'f',2 );
                msg += "_";
                if( m_warnType == TEST_VALUE_WARN )
                {
                    msg +=QString::fromLocal8Bit("测量值：二级");
                }
                else if( m_warnType == AVE_VALUE_WARN )
                {
                    msg +=QString::fromLocal8Bit("平均值：二级");
                }			
                msg += "_";
                msg += m_btn_instrument[index].datetimes1;
                m_warnline_2nd_dlg->AddMsg( msg );
                m_2ndcount++;
                emit UpdateWarnSignal();
            }
        }		
    }
    else if( probe == 2 )
    {
        if( str1stlist.count() == 2 && str2ndlist.count() == 2 )
        {
            double warn1st = str1stlist[1].toDouble();
            double warn2nd = str2ndlist[1].toDouble();
            QString msg = m_btn_instrument[index].deviceno;
            msg += "_";
            msg += m_btn_instrument[index].deviceName;
            msg += "_";
            msg +=QString::fromLocal8Bit("探头 2 ");
            msg += m_btn_instrument[index].types2;
            msg +="_";
            if( value >= warn1st && value < warn2nd )
            {

                msg += QString("%1_%2").arg( warn1st ).arg( value );
                msg += "_";
                if( m_warnType == TEST_VALUE_WARN )
                {
                    msg +=QString::fromLocal8Bit("测量值：一级");
                }
                else if( m_warnType == AVE_VALUE_WARN )
                {
                    msg +=QString::fromLocal8Bit("平均值：一级");
                }			
                msg += "_";
                msg += m_btn_instrument[index].datetimes2;
                m_warnline_1st_dlg->AddMsg( msg );
                m_1stcount++;
                emit UpdateWarnSignal();
            }
            else if( value >= warn2nd )
            {

                msg += QString("%1_%2").arg( warn2nd ).arg( value );
                msg += "_";
                if( m_warnType == TEST_VALUE_WARN )
                {
                    msg +=QString::fromLocal8Bit("测量值：二级");
                }
                else if( m_warnType == AVE_VALUE_WARN )
                {
                    msg +=QString::fromLocal8Bit("平均值：二级");
                }			
                msg += "_";
                msg += m_btn_instrument[index].datetimes2;
                m_warnline_2nd_dlg->AddMsg( msg );
                m_2ndcount++;
                emit UpdateWarnSignal();
            }

        }
    }		
}

void H2SMonitorClient::AveCalculate(double times, double value, int index, int probe ,DATA_INFO *st)
{
    if( m_warnType == TEST_VALUE_WARN )
    {
        WarnTestValue(value, index,probe);
    }
    if( index >= m_recvCaculate.count() )
        return;
    QString strsamp ;
    QString strtime;
    int icount; 
    double ave =0.0;
    double ave_time = 0.0;
    if( probe == 1 )
    {
        m_recvCaculate[index*2].x_value.push_back( times );
        m_recvCaculate[index*2].y_value.push_back( value );
        strsamp = m_btn_instrument[index].sample1;
        icount = m_aveTime / strsamp.toInt();
        if( m_recvCaculate[index*2].y_value.count() >= icount )
        {
            ave = AveCalculatefromQVector(m_recvCaculate[index*2].y_value);
            ave_time = Max( m_recvCaculate[index*2].x_value);
            st->values = QString("%1").arg( ave );
            strtime = QString::number( (unsigned long long)ave_time );
            st->times  = strtime;
            m_recvAveData[index*2].y_value.push_back( ave );
            m_recvAveData[index*2].x_value.push_back( ave_time );
            m_recvCaculate[index*2].x_value.clear();
            m_recvCaculate[index*2].y_value.clear();
            if( m_warnType == AVE_VALUE_WARN )
            {
                WarnTestValue( ave, index, probe );
            }
        //	QString dates = QString("%1").arg( times );
        /*	if(m_currentDeviceNo == index && !m_dataShow->isHidden())
            {
                m_dataShow->UpdateInfo(probe, times, ave, true);
            }
            m_vt_avebasedata.push_back( st );*/
        }
    }
    else if( probe == 2 )
    {
        m_recvCaculate[index*2+1].x_value.push_back( times );
        m_recvCaculate[index*2+1].y_value.push_back( value );
        strsamp = m_btn_instrument[index].sample2;
        icount = m_aveTime / strsamp.toInt();
        if( m_recvCaculate[index*2+1].y_value.count() >= icount )
        {
            ave = AveCalculatefromQVector(m_recvCaculate[index*2+1].y_value);
            ave_time = Max( m_recvCaculate[index*2+1].x_value);
            st->values = QString("%1").arg( ave );
            strtime =QString::number( (unsigned long long)ave_time );
            st->times  = strtime;
            m_recvAveData[index*2+1].y_value.push_back( ave );
            m_recvAveData[index*2+1].x_value.push_back( ave_time );
            m_recvCaculate[index*2+1].x_value.clear();
            m_recvCaculate[index*2+1].y_value.clear();
            if( m_warnType == AVE_VALUE_WARN )
            {
                WarnTestValue( ave, index, probe );
            }
        }
    }
}

void H2SMonitorClient::SaveDataCommpeleteSlot()
{
    QRemoveDataToBaseThread *thread = (QRemoveDataToBaseThread*)sender();
    delete thread;
}


//******************************************************
//** 函数名:   UpdateServerTime
//** 功能简述: 更新显示服务器时间
//** 输入参数: 
//   const QString & serverTime:
//   const QString & weekday:
//
//** 返回值: 
//   void:
//
//** 创建日期：2015/12/24
//** 创建人：  唐桥
//** 修改记录：
//   日期        版本      修改人    修改内容
//
//** 其它说明：
//******************************************************
void H2SMonitorClient::UpdateServerTime(const QString & serverTime, const QString & weekday)
{
    QString titleFormat = QStringLiteral("欢迎您！  %1  %2  %3");
    QString title = titleFormat.arg(m_userName).arg(serverTime).arg(weekday);
    m_lb_user->setText(title);
}
void H2SMonitorClient::HeartBeatSlot()
{
	CMessage *message = new CMessage();
	message->m_header.m_struct.cmd = PC_CLIENT_CMD_SEND_HEARTBEAT;
	message->m_header.m_struct.messageNo = 3;
	message->m_header.m_struct.cmdResult = 1;// 非0表示 需要获取错误信息
	m_messageThread->SendMessage( message );
}
void H2SMonitorClient::batteryValue(unsigned char &value)
{
	value = value & 0x7f;
}