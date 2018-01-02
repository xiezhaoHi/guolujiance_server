/*****************************************************
**  文 件 名：form_login.cpp
**  版 权：XXX 版权所有
**  版    本：Ver.1.0
**  内容简述：登录画面类实现文件
**
**  创建日期：2015/07/22 
**  创建人：杨恒
**  修改记录：
    日期        版本      修改人    修改内容   
*****************************************************/

#include "form_login.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QString>
#include <QFile>
#include <QLayoutItem>
#include "settings\settings.h"
#include <QPainter>
#include <QLabel>
#include <QTimer>
#include <QRegExpValidator>
#include "message/message_login_rep.h"
#include <QMetaType>
#include <QProcess>
#include "message_queue.h"
#include "qregistdlg.h"
#include <QMediaPlayer>



QString g_comdescrip = "Silicon Labs CP210x USB to UART Bridge";
CFormLogin::CFormLogin(QWidget *parent)
    : QWidget(parent)
{
    // 暂时不进行注册 tangqiao 2015/12/14
    //bool ret = isRegist();
    //if( !ret )
    //{
    //    ResgitLocalHost();
    //    if(!isRegist())
    //    {
    //        QMessageBox::critical(this, tr("ERROR"), tr("Regist Code error!"));
    //        QTimer::singleShot(0, qApp, SLOT(quit()));		                     //退出程序
    //    }
    //}
    setWindowTitle(QString::fromLocal8Bit("环境监测系统客户端"));
    QIcon image(":/images/images/main.png");
    setWindowIcon(image);
    this->setWindowFlags(Qt::Dialog | Qt::WindowMinimizeButtonHint |Qt::WindowCloseButtonHint);
    setFixedSize(1024, 768);
    BuildUI();
    qRegisterMetaType<CMessage*>("CMessage*");
    m_messageThread = new QMessageThread(  );
    //connect( m_messageThread, SIGNAL(ServerRepInfo(int)), this, SLOT(ServerRepInfoSlot(int)));
    connect( m_messageThread, SIGNAL(ServerRepMsg(QString)), this, SLOT(ServerRepMsgSlot(QString)));
    connect(QCheckRecvQueueThread::GetInstance(), SIGNAL(ServerRepInfo(CMessage*)), this, SLOT(ServerRepInfoSlot(CMessage*)));
    QCheckRecvQueueThread::GetInstance()->SetMessageThread( m_messageThread );
    //threads->StartTcpServer();
    
    QCheckRecvQueueThread::GetInstance()->start();
    m_wait_dlg = new QWaitWidget( this );
    m_wait_dlg->hide();
    m_timer= new QTimer( this );
    m_timer->setSingleShot( true );
    
    connect( m_timer, SIGNAL(timeout()), this, SLOT(WaitTimeOut()));
    setFocusPolicy(Qt::StrongFocus);
    m_pEditAccount->setFocus();
    m_pButtonLogin->setAutoDefault(true);
    QCheckRecvQueueThread::GetInstance()->setRecvData(false);
    m_passwd_dlg = NULL;
    m_collect_dlg = NULL;
	
}

CFormLogin::~CFormLogin()
{

}

//构建登录画面UI
void CFormLogin::BuildUI()
{
    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::blue);

    QLabel *lb_title = new QLabel( QString::fromLocal8Bit("环境监测系统"), this );
    //
    lb_title->setFont(QFont(QString::fromLocal8Bit("宋体"), 20, QFont::Bold));
    lb_title->setPalette( pa );
    lb_title->setGeometry(20, 50, 200, 30);

    pa.setColor( QPalette::WindowText, Qt::black );
    QLabel *lb_title2 = new QLabel( QString::fromLocal8Bit("Environment monitor system"), this );
    lb_title2->setFont(QFont(QString::fromLocal8Bit("宋体"), 10, QFont::Bold));
    lb_title2->setPalette( pa );
    lb_title2->setGeometry(280, 50, 200, 30);

    QLabel *lb_title3 = new QLabel( QString::fromLocal8Bit("欢迎使用监测系统！"), this );
    lb_title3->setFont(QFont(QString::fromLocal8Bit("宋体"), 12, QFont::Bold));
    lb_title3->setPalette( pa );
    lb_title3->setGeometry(830, 80, 200, 30);

    QLabel *lb_ver = new QLabel( QString::fromLocal8Bit("@ 版权所有 环境监测系统 Environment monitor system"), this );
    lb_ver->setFont(QFont(QString::fromLocal8Bit("宋体"), 12, QFont::Bold));
    lb_ver->setPalette( pa );
    lb_ver->setGeometry(400, 600, 450,   30);
    //600,250
    pa.setColor(QPalette::WindowText,Qt::blue);
    QLabel *lb_loging = new QLabel( QString::fromLocal8Bit("用户登录"), this );
    lb_loging->setFont(QFont(QString::fromLocal8Bit("宋体"), 18, QFont::Bold));
    lb_loging->setPalette( pa );
    lb_loging->setGeometry(690, 300, 150,   30);

    QLabel *lb_user = new QLabel( QString::fromLocal8Bit("用户名"), this );
    lb_user->setFont(QFont(QString::fromLocal8Bit("宋体"), 15, QFont::Bold));
    lb_user->setPalette( pa );
    lb_user->setGeometry(650, 360, 80,   30);

    QLabel *lb_pwd = new QLabel( QString::fromLocal8Bit("密  码"), this );
    lb_pwd->setFont(QFont(QString::fromLocal8Bit("宋体"), 15, QFont::Bold));
    lb_pwd->setPalette( pa );
    lb_pwd->setGeometry(650, 410, 80,   30);

    QImage image;
    //image.load("./Resources/images/login/login_commit.png");
    image.load(":/login/images/login/login_commit.png");
    m_pButtonLogin = new QCustomButton(image,this);
    //m_pButtonLogin->setText("xxxxx");
    m_pButtonLogin->setGeometry(730, 465, 94, 34);

    image.load(":/login/images/login/login_cancel.png");
    m_pButtonCancel = new QCustomButton(image,this);
    m_pButtonCancel->setGeometry(885, 465, 94, 34);

     m_pEditAccount = new QLineEdit(this);
     m_pEditAccount->setGeometry(730, 355, 250, 38 );
     m_pEditPassword = new QLineEdit(this);
     m_pEditPassword->setGeometry(730, 405, 250, 38 );
     m_pEditAccount->setObjectName(QStringLiteral("lineedit_account"));
     m_pEditPassword->setObjectName(QStringLiteral("lineedit_password"));
     m_pEditAccount->setPlaceholderText(QStringLiteral("请输入登录账号"));
     m_pEditPassword->setPlaceholderText(QStringLiteral("请输入登录密码"));
     m_pEditPassword->setEchoMode(QLineEdit::Password);
    connect(m_pButtonLogin, SIGNAL(clicked()), this, SLOT(Login()));
    connect(m_pButtonCancel,SIGNAL(clicked()), this, SLOT(LoginCancel()));

    QRegExp IdentityCard("^[A-Za-z0-9]+$"); 
    m_pEditAccount->setValidator(new QRegExpValidator(IdentityCard, m_pEditAccount));
    m_pEditPassword->setValidator(new QRegExpValidator(IdentityCard, m_pEditPassword));

    image.load(":/login/images/login/bt_device_collect.png");
    m_pButton_Collect = new QCustomButton(image, this );
    m_pButton_Collect->setGeometry(100, 650, 100, 30 );
    connect( m_pButton_Collect, SIGNAL(clicked()), this, SLOT(DeviceCollectSlot()));

}

//美化登录画面方法
void CFormLogin::BeautifyUI()
{
    //设置背景图片
    QPalette palette;
    QPixmap pixmap(QDir::toNativeSeparators(":/login/images/login_bg.png"));
    palette.setBrush(QPalette::Window, QBrush(pixmap));

    //设置背景颜色
    /*QPalette palette;
    palette.setColor(QPalette::Background, QColor(224, 224, 248));*/

    this->setPalette(palette);
    this->setObjectName(QStringLiteral("form_login"));
    m_pEditAccount->setObjectName(QStringLiteral("lineedit_account"));
    m_pEditPassword->setObjectName(QStringLiteral("lineedit_password"));
    m_pCheckSavePassword->setObjectName(QStringLiteral("checkbox_savepassword"));
    m_pCheckAutoLogin->setObjectName(QStringLiteral("checkbox_autologin"));
    m_pButtonLogin->setObjectName(QStringLiteral("pushbutton_login"));
    m_pButtonCancel->setObjectName(QStringLiteral("pushbutton_login"));

    m_pEditAccount->setPlaceholderText(QStringLiteral("账号"));
    m_pEditPassword->setPlaceholderText(QStringLiteral("密码"));
    m_pEditPassword->setEchoMode(QLineEdit::Password);
    /*if (CClientSetting::EnableQss()) {
        QString stylesheet;
        QFile qssFile(":/qss/form_login.qss");
        if (qssFile.open(QFile::ReadOnly)) {
            stylesheet = QLatin1String(qssFile.readAll());
            this->setStyleSheet(stylesheet);
            qssFile.close();
        }
    }*/
    
}

void CFormLogin::TranslateUI()
{
    // do nothing
}
//绑定本界面相关的信号与槽
void CFormLogin::BindSlots()
{
  //  connect(m_pButtonLogin, SIGNAL(clicked()), this, SLOT(Login()));
    //connect(m_pButtonCancel,SIGNAL(clicked()), this, SLOT(LoginCancel()));
}
//点击登录方法
void CFormLogin::Login()
{
    if( !m_messageThread->isRunning() )
	{
		m_messageThread->start();
	}
    if( !CMessageQueue::getInstance()->GetConnectState() )
    {
        QMessageBox box(QMessageBox::Question,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("客户端与服务器连接已断开，请稍后重试！"));
        QPushButton *okButton = (box.addButton(QString::fromLocal8Bit("确定"),QMessageBox::AcceptRole));		
        box.exec();
        return;
    }
    SetAccount(m_pEditAccount->text());
    SetPassword(m_pEditPassword->text());


    CMessageLogin *login = new CMessageLogin();
    if( GetAccount() == "" )
    {
        QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("帐号为空,请重新输入！"));
        return;
    }
    if( GetPassword() == "" )
    {
        QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("密码为空,请重新输入！"));
        return;
    }
    login->m_qstrAccount = GetAccount();
    login->m_qstrPassword = GetPassword();

    CMessage * message = new CMessage();
    message->m_header.m_struct.cmd = PC_CLIENT_CMD_LOGIN;
    message->m_header.m_struct.messageNo = 1;
    message->m_header.m_struct.cmdResult = 1;
    message->SetMessageBody(login);
    //QMessageThread::getInstance()->SendMessage(message);

    m_messageThread->SendMessage(message);
    m_timer->start(10000 );
    m_wait_dlg->exec();
    
}

//点击取消方法
void  CFormLogin::LoginCancel()
{
	if( m_messageThread->isRunning() )
	{
		m_messageThread->SetThreadOver();
		m_messageThread->wait();
	}
    close();
}
void CFormLogin::paintEvent(QPaintEvent *e )
{
    QPainter painter(this);
    QImage image;
    image.load(":/login/images/login/login_bg.jpg");
    painter.drawImage(QRect(0,0, image.width(), image.height()), image);

    image.load(":/login/images/login/login.png");
    painter.drawImage(QRect(600,250, image.width(), image.height()), image);
}
void CFormLogin::ServerRepInfoSlot(CMessage* msg)
{
    if( msg->m_header.m_struct.cmd == PC_SERVER_CMD_LOGIN_REP && msg->m_header.m_struct.cmdResult == 0 )
    {
        if( !m_wait_dlg->isHidden() )
            m_wait_dlg->hide();
        if( m_timer->isActive() )
        {
            m_timer->stop();
        }
            CMessage *logininfo = msg;
            delete msg;
            msg = NULL;
            //CMessageLoginRep* logins = static_cast<CMessageLoginRep*>(logininfo->m_pBody);
            //unsigned long long servertime = logins->m_struct.serverTime;
            m_bLoginSucceed = true;
        //	QCheckRecvQueueThread::GetInstance()->setItLoginSysMsg( false );
            //disconnect( QCheckRecvQueueThread::GetInstance(), SIGNAL(ServerRepInfo(CMessage*)), this, SLOT(ServerRepInfoSlot(CMessage*)));
            if( !this->isHidden() )
            {
                this->hide();
            }
            
        //发送登录成功信号
            emit SendUserName( m_strAccount , m_strPassword);
            emit LoginCompleted(true);	
        
    }

}
void CFormLogin::WaitTimeOut()
{
	if( m_messageThread->isRunning() )
	{
		m_messageThread->SetThreadOver();
		m_messageThread->wait();
	}
    m_wait_dlg->hide();
    QMessageBox::information( this,QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("登录超时， 请重启程序或登录！"));
}
void CFormLogin::keyPressEvent(QKeyEvent *event )
{
    if( event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return )
    {
        emit m_pButtonLogin->click();
    }
}
void CFormLogin::ServerRepMsgSlot(QString msg)
{
    m_wait_dlg->hide();
    if( m_timer->isActive() )
    {
        m_timer->stop();
    }
    QMessageBox::information(this, QString::fromLocal8Bit("提示"), msg );

}

bool CFormLogin::isRegist()
{
    m_diskid = QString("%1").arg(getDiskInfo().serialNumber);
    m_diskid = m_diskid.trimmed();
    yxyDES2* my_des = new yxyDES2();

    my_des->InitializeKey("12345678",0);

    QByteArray ba = m_diskid.toLatin1();
    my_des->EncryptData(ba.data(),0);

    QString RegistCode = QString("%1").arg( my_des->GetCiphertextInHex());

    QString strRcode;
    strRcode = QInitConfiguration::GetInitInstance()->m_registCode;
    delete my_des;
    my_des = NULL;
    if( strRcode == RegistCode )
    {		
        return true;
    }
    return false;
}
void CFormLogin::ResgitLocalHost()
{
    QRegistDlg *RegistDlg = new QRegistDlg(m_diskid, this );

    if(QDialog::Accepted == RegistDlg->exec())
    {
        QString str = RegistDlg->m_code;
        QInitConfiguration::GetInitInstance()->setRegistCode( str );
    }
    delete RegistDlg;
    RegistDlg = NULL;
    
}
void CFormLogin::DeviceCollectSlot()
{
    QProcess * pProcess = new QProcess(this);
    pProcess->setProgram(QString("%1\\gas-config-v1.0.exe").arg(QDir::currentPath()));
    pProcess->start();
    if (!pProcess->waitForStarted())
    {
        delete pProcess;
    }
    return;

    QList<QSerialPortInfo> serialPortInfoList = QSerialPortInfo::availablePorts();  
    bool isfind = false;
    foreach (const QSerialPortInfo &serialPortInfo, serialPortInfoList) 
    {
        QString str = serialPortInfo.description();
        if( str == g_comdescrip )
        {
            isfind = true;
            m_commid = serialPortInfo.portName();
            break;
        }
    }  
    if( !isfind )
    {
        QMessageBox box(QMessageBox::Question,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("未找到设备，请检查设备连接是否正确！"));
        QPushButton *okButton = (box.addButton(QString::fromLocal8Bit("确定"),QMessageBox::AcceptRole));		
        box.exec();
        return;
    }
    
    if( !m_passwd_dlg )
    {
        m_passwd_dlg = new QPasswd_dlg( this );
    }
    if( QDialog::Accepted == m_passwd_dlg->exec() )
    {
        if( !m_collect_dlg )
        {
            m_collect_dlg = new QDevice_Collect_dlg( this );
        }
        m_collect_dlg->setCommId(m_commid);
        m_collect_dlg->exec();
    }
    
    delete m_passwd_dlg;
    m_passwd_dlg = NULL;
    if( m_collect_dlg )
    {
        delete m_collect_dlg;
        m_collect_dlg = NULL;
    }
}
