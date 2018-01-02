#include "qparamsetwidget.h"
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QRegExpValidator>
#include <QPainter>
#include <QMessageBox>



QParamSetWidget::QParamSetWidget(QWidget *parent)
    : QWidget(parent)
{
    QPalette paletet = this->palette();
    paletet.setBrush(QPalette::Background,Qt::blue);
    setPalette( paletet );
    //setAutoFillBackground( true );
    InitWidget();
}

QParamSetWidget::~QParamSetWidget()
{

}
void QParamSetWidget::InitWidget()
{
    QLabel *lb_warn = new QLabel(QString::fromLocal8Bit("报警参数设置:"), this );
    lb_warn->setGeometry( 10, 10, 120, 30 );

    m_radio_ave = new QRadioButton(QString::fromLocal8Bit("平均值报警"), this );
    m_radio_test = new QRadioButton(QString::fromLocal8Bit("测量值报警"), this );
    m_radio_test->setChecked(true);
    m_lb_ave_time = new QLabel(QString::fromLocal8Bit("平均时间："), this );
    m_lb_ave_unit = new QLabel(QString::fromLocal8Bit("秒"), this );
    m_lne_ave_time = new QLineEdit( this );
    m_lne_ave_time->setText( QInitConfiguration::GetInitInstance()->m_aveTime );

    m_radio_ave->setGeometry( 20, 50, 120, 30);
    m_radio_test->setGeometry( 360, 50, 120, 30);

    m_lb_ave_time->setGeometry(20, 80, 100, 30);
    m_lne_ave_time->setGeometry(100, 80, 120, 30);
    m_lb_ave_unit->setGeometry( 230, 80, 50, 30);
    
    int itype = QInitConfiguration::GetInitInstance()->m_warnType.toInt();
    if( itype == 1 )
    {
        m_radio_ave->setChecked( true );
        m_radio_test->setChecked( false );
        m_lb_ave_time->show();
        m_lb_ave_unit->show();
        m_lne_ave_time->show();
    }
    else if( itype == 2 )
    {
        m_radio_ave->setChecked( false );
        m_radio_test->setChecked( true );
        m_lb_ave_time->hide();
        m_lb_ave_unit->hide();
        m_lne_ave_time->hide();
    }

    m_pb_warnParam = new QPushButton(QString::fromLocal8Bit("报警设置"), this );
    m_pb_warnParam->setGeometry( 540, 80, 100, 30);

    QLabel *lb_device = new QLabel(QString::fromLocal8Bit("设备参数设置:"), this );
    lb_device->setGeometry(20, 130, 120, 30 );

    m_lb_code = new QLabel(QString::fromLocal8Bit("设备编号："), this );
    m_lb_name = new QLabel( QString::fromLocal8Bit("设备名称："), this );
    m_lb_type = new QLabel(this );

    m_lb_code->setGeometry(20, 170, 160, 30 );
    m_lb_name->setGeometry(200, 170, 30, 30 );
    m_lb_type->setGeometry(550, 170, 200, 30 );
    m_lne_Name_set = new QLineEdit( this );
    m_lne_Name_set->setGeometry(250, 170, 200, 30);

    QLabel *lb_probe1 = new QLabel(QString::fromLocal8Bit("H2S:"), this );
    QLabel *lb_probe1_one = new QLabel(QString::fromLocal8Bit("一级预警:"), this );
    QLabel *lb_probe1_two = new QLabel(QString::fromLocal8Bit("二级预警:"), this );
    QLabel *lb_probe1_max = new QLabel(QString::fromLocal8Bit("最大值:"), this );

    QLabel *lb_probe2 = new QLabel(QString::fromLocal8Bit("SO2:"), this );
    QLabel *lb_probe2_one = new QLabel(QString::fromLocal8Bit("一级预警:"), this );
    QLabel *lb_probe2_two = new QLabel(QString::fromLocal8Bit("二级预警:"), this );
    QLabel *lb_probe2_max = new QLabel(QString::fromLocal8Bit("最大值:"), this );
    
    m_probe1_one_warn = new QLineEdit( this );
    m_probe1_two_warn = new QLineEdit( this );
    m_probe1_max_value = new QLineEdit( this );

    m_probe2_max_value = new QLineEdit( this );
    m_probe2_two_warn = new QLineEdit( this );
    m_probe2_one_warn = new QLineEdit( this );
    m_pb_saveParam = new QPushButton(QString::fromLocal8Bit("保存设置"), this );
    m_pb_nextDevice = new QPushButton( QString::fromLocal8Bit("切换设备"), this );

    m_lb_sample1 = new QLabel(QString::fromLocal8Bit("采样率:"), this );
    m_lb_sample2 = new QLabel(QString::fromLocal8Bit("采样率:"), this );
    
    m_lne_sample1 = new QLineEdit(QString::fromLocal8Bit("10"), this );
    m_lne_sampl2 = new QLineEdit(QString::fromLocal8Bit("10"), this );

    // 暂时屏蔽采样率输入框
    m_lne_sample1->setEnabled(false);
    m_lne_sampl2->setEnabled(false);

    lb_probe1->setGeometry(20, 200, 100, 30 );
    lb_probe1_one->setGeometry(20, 230, 100, 30 );
    lb_probe1_two->setGeometry(20, 290, 100, 30 );
    lb_probe1_max->setGeometry(20, 350, 100, 30 );

    QLabel *lb_unit_probe1_one = new QLabel(QString::fromLocal8Bit("PPM"), this );
    QLabel *lb_unit_probe1_two = new QLabel(QString::fromLocal8Bit("PPM"), this );
    QLabel *lb_unit_probe1_max = new QLabel(QString::fromLocal8Bit("PPM"), this );
    m_probe1_one_warn->setGeometry(100, 230, 120, 30 );
    m_probe1_two_warn->setGeometry(100, 290, 120, 30 );
    m_probe1_max_value->setGeometry(100, 350, 120, 30 );

    lb_unit_probe1_one->setGeometry(230, 230, 50, 30);
    lb_unit_probe1_two->setGeometry(230, 290, 50, 30);
    lb_unit_probe1_max->setGeometry(230, 350, 50, 30);

    lb_probe2->setGeometry(360, 200, 100, 30 );
    lb_probe2_one->setGeometry(360, 230, 100, 30 );
    lb_probe2_two->setGeometry(360, 290, 100, 30 );
    lb_probe2_max->setGeometry(360, 350, 100, 30 );

    QLabel *lb_unit_probe2_one = new QLabel(QString::fromLocal8Bit("PPM"), this );
    QLabel *lb_unit_probe2_two = new QLabel(QString::fromLocal8Bit("PPM"), this );
    QLabel *lb_unit_probe2_max = new QLabel(QString::fromLocal8Bit("PPM"), this );
    m_probe2_one_warn->setGeometry(450, 230, 120, 30 );
    m_probe2_two_warn->setGeometry(450, 290, 120, 30 );
    m_probe2_max_value->setGeometry(450, 350, 120, 30 );

    lb_unit_probe2_one->setGeometry(580, 230, 50, 30);
    lb_unit_probe2_two->setGeometry(580, 290, 50, 30);
    lb_unit_probe2_max->setGeometry(580, 350, 50, 30);

    m_pb_saveParam->setGeometry(100, 460, 80, 30 );
    m_pb_nextDevice->setGeometry(540, 460, 80, 30 );

    m_lb_sample1->setGeometry(20, 420, 100, 30);
    m_lb_sample2->setGeometry( 360, 420, 100, 30);

    QLabel *lb_samp_probe1 = new QLabel(QString::fromLocal8Bit("秒"), this );
    QLabel *lb_samp_probe2 = new QLabel(QString::fromLocal8Bit("秒"), this );
    m_lne_sample1->setGeometry(100, 420, 120, 30);
    m_lne_sampl2->setGeometry( 450, 420, 120, 30);

    lb_samp_probe1->setGeometry( 230, 420, 50, 30);
    lb_samp_probe2->setGeometry( 580, 420, 50, 30);

    QRegExp regexp("^[0-9]+(.[0-9]{1,9})?$"); 
    m_probe1_one_warn->setValidator(new QRegExpValidator(regexp, m_probe1_one_warn));
    m_probe1_two_warn->setValidator(new QRegExpValidator(regexp, m_probe1_two_warn));
    m_probe1_max_value->setValidator(new QRegExpValidator(regexp, m_probe1_max_value));
    m_probe2_one_warn->setValidator(new QRegExpValidator(regexp, m_probe2_one_warn));
    m_probe2_two_warn->setValidator(new QRegExpValidator(regexp, m_probe2_two_warn));
    m_probe2_max_value->setValidator(new QRegExpValidator(regexp, m_probe2_max_value));

    

    QRegExp regexp1("^[1-9][0-9]*$"); 
    m_lne_sampl2->setValidator(new QRegExpValidator(regexp1, m_probe2_max_value));
    m_lne_sample1->setValidator(new QRegExpValidator(regexp1, m_probe2_max_value));
	m_lne_ave_time->setValidator(new QRegExpValidator(regexp1, m_lne_ave_time));
    connect( m_pb_nextDevice, SIGNAL(clicked()), this ,SLOT(NextDeviceSlot()));
    connect( m_pb_saveParam, SIGNAL(clicked()), this, SLOT(SaveParamSlot()));
    connect( m_pb_warnParam, SIGNAL(clicked()), this, SLOT(SaveWarnSlot()));
    connect( m_radio_ave, SIGNAL(toggled(bool)), this, SLOT(AveWarnClickSlot(bool)));
}

void QParamSetWidget::InitInfo(PARAM_SET st)
{
    CleanParam();
    QString msg = QString::fromLocal8Bit("编号：");
    msg += st.code;
    m_lb_code->setText( msg );
    msg = QString::fromLocal8Bit("名称：");
    m_lb_name->setText( msg );
    msg = st.name;
    m_lne_Name_set->setText( msg );
    msg = QString::fromLocal8Bit("类型：");
    if( st.types == "1" )
    {
        msg += QString::fromLocal8Bit("便携式H2S检测仪");
    }
    else if( st.types == "2" )
    {
        msg += QString::fromLocal8Bit("固定式H2S检测仪");
    }
    // 界面不显示是哪种设备类型，who TM cares! tangqiao 2015/12/25

    QStringList listinfo = st.warn_one.split("#");
    if( listinfo.count() == 1 )
    {
        m_probe1_one_warn->setText( listinfo[0] );
    }
    else if( listinfo.count() == 2 )
    {
        m_probe1_one_warn->setText( listinfo[0] );
        m_probe2_one_warn->setText( listinfo[1] );
    }
    //Q_ASSERT(listinfo.count() == 2 );
    //m_probe1_one_warn->setText( listinfo[0] );
    //m_probe2_one_warn->setText( listinfo[1] );
    listinfo = st.warn_two.split("#");
    if( listinfo.count() == 1 )
    {
        m_probe1_two_warn->setText( listinfo[0] );
    }
    else if( listinfo.count() == 2 )
    {
        m_probe1_two_warn->setText( listinfo[0] );
        m_probe2_two_warn->setText( listinfo[1] );
    }
    //Q_ASSERT(listinfo.count() == 2 );	
    //m_probe1_two_warn->setText( listinfo[0] );
    //m_probe2_two_warn->setText( listinfo[1] );
    listinfo = st.maxs.split("#");
    if( listinfo.count() == 1 )
    {
        m_probe1_max_value->setText( listinfo[0] );
    }
    else if( listinfo.count() == 2 )
    {
        m_probe1_max_value->setText( listinfo[0] );
        m_probe2_max_value->setText( listinfo[1] );
    }
    listinfo = st.sample.split("#");
    if( listinfo.count() == 2 )
    {
        m_lne_sample1->setText( listinfo[0] );
        m_lne_sampl2->setText( listinfo[1] );
    }
    else
    {
        m_lne_sample1->setText( "10" );
        m_lne_sampl2->setText( "10" );
    }
}
void QParamSetWidget::NextDeviceSlot()
{
    emit clicked();
}
void QParamSetWidget::SaveParamSlot()
{
    QString strmsg  = m_lne_Name_set->text();
    strmsg += ";";
    strmsg += m_probe1_one_warn->text();
    strmsg += "#";
    strmsg += m_probe2_one_warn->text();
    strmsg += ";";
    strmsg += m_probe1_two_warn->text();
    strmsg += "#";
    strmsg += m_probe2_two_warn->text();
    strmsg += ";";
    strmsg += m_probe1_max_value->text();
    strmsg += "#";
    strmsg += m_probe2_max_value->text();
    strmsg +=";";
    strmsg += m_lne_sample1->text();
    strmsg += "#";
    strmsg += m_lne_sampl2->text();
    int itype = -1;
    if( m_radio_test->isChecked() )
    {
        itype = 2;
    }
    else if( m_radio_ave->isChecked() )
    {
        itype = 1;
    }
    emit SaveParamSignal( strmsg );
}
void QParamSetWidget::CleanParam()
{
    m_probe1_max_value->clear();
    m_probe1_one_warn->clear();
    m_probe1_two_warn->clear();
    m_probe2_max_value->clear();
    m_probe2_one_warn->clear();
    m_probe2_two_warn->clear();
}

void QParamSetWidget::paintEvent(QPaintEvent *e)
{
    QPainter painter( this );
    QRect rc = geometry();
    QPen pen;
    pen.setColor(Qt::black);
    pen.setWidth( 2 );
    pen.setStyle(Qt::DotLine);
    painter.setPen( pen );
    painter.drawLine(20, 120, 600, 120);
    painter.drawLine(20, 400, 600, 400);
}
void QParamSetWidget::SaveWarnSlot()
{
    int itype = -1;
    if( m_radio_test->isChecked() )
    {
        itype = 2;
    }
    else if( m_radio_ave->isChecked() )
    {
        itype = 1;
    }
    int itime = m_lne_ave_time->text().toInt();
    QString strtype = QString("%1").arg( itype );
    QString strtime = QString("%1").arg( itime  );
    QInitConfiguration::GetInitInstance()->setWarnInfo(strtime, strtype);
    emit SaveWarnParam( itype, itime );

    // 提示用户报警参数设置完成
    QMessageBox::information(this, QStringLiteral("信息"), QStringLiteral("报警参数设置成功"));
}

void QParamSetWidget::AveWarnClickSlot(bool ischecked)
{
    if( ischecked )
    {
        m_lb_ave_time->show();
        m_lb_ave_unit->show();
        m_lne_ave_time->show();
    }
    else
    {
        m_lb_ave_time->hide();
        m_lb_ave_unit->hide();
        m_lne_ave_time->hide();
    }
    
}