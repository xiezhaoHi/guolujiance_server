#include "h2smonitorserver.h"
#include <QFile>
#include <QGridLayout>
#include <QMenu>
#include <QAction>
#include <QMenuBar>
#include "service/service_hub.h"
#include "form_log_viewer.h"


H2SMonitorServer::H2SMonitorServer(QWidget *parent)
    : QMainWindow(parent)
{
    BuildUI();
    BindSlots();
}

H2SMonitorServer::~H2SMonitorServer()
{
    
}

void H2SMonitorServer::BuildUI()
{
    m_pFormDevicesContainer = new CFormDevicesContainer(this);
    m_pFormDevicesContainer->setMinimumWidth(300);
    m_pFormDeviceDetail = new CFormRigDeviceDetail(this);
    m_pFormDeviceDetail->setMinimumWidth(490);
    m_pFormServerControlPanel = new CFormServerControlPanel(this);

    m_pFormServerControlPanel->setFixedHeight(85);

    m_pSplitter = new QSplitter(this);
    m_pSplitter->setAutoFillBackground(true);
    m_pSplitter->setObjectName(QStringLiteral("mainwindow_splitter"));
    m_pSplitter->setOrientation(Qt::Horizontal);
  
    m_pSplitter->addWidget(m_pFormDevicesContainer);

    QWidget * pWidgetRight = new QWidget(this);
    QGridLayout * pLayoutOfWidgetRight = new QGridLayout(pWidgetRight);
    pLayoutOfWidgetRight->addWidget(m_pFormDeviceDetail, 0, 0, 1, 1);
    pLayoutOfWidgetRight->addWidget(m_pFormServerControlPanel, 1, 0, 1, 1);
    pLayoutOfWidgetRight->setRowStretch(0, 1);
    pLayoutOfWidgetRight->setRowStretch(1, 0);
    pLayoutOfWidgetRight->setVerticalSpacing(3);
    pWidgetRight->setLayout(pLayoutOfWidgetRight);
    m_pSplitter->addWidget(pWidgetRight);
    m_pSplitter->setStretchFactor(0, 2);
    m_pSplitter->setStretchFactor(1, 5);

    this->setCentralWidget(m_pSplitter);
    this->setMinimumSize(850, 400);
    this->setBaseSize(this->sizeHint());

    // 菜单栏
    m_pMenuViews = new QMenu(QStringLiteral("视图"), this);
    m_pActionLogView = new QAction(QStringLiteral("显示日志"), this);
    m_pMenuViews->addAction(m_pActionLogView);
    menuBar()->addMenu(m_pMenuViews);

    this->setWindowTitle(QStringLiteral("H2S检测服务器"));

    m_pLogViewer = new CFormLogViewer(NULL);

    BeautifyUI();
    TranslateUI();
}

void H2SMonitorServer::BeautifyUI()
{
    QPalette palette;
    palette.setColor(QPalette::Background, QColor(224, 224, 248));
    m_pSplitter->setAutoFillBackground(true);
    m_pSplitter->setPalette(palette);

    menuBar()->setAutoFillBackground(true);
    menuBar()->setPalette(palette);
}

void H2SMonitorServer::TranslateUI()
{
    // do nothing
}

void H2SMonitorServer::BindSlots()
{
    connect(m_pActionLogView, SIGNAL(triggered(bool)), this, SLOT(menuLogViewClicked(bool)));
    connect(m_pFormServerControlPanel, SIGNAL(SwitchSerivceStatus(bool)), this, SLOT(switchServiceStatus(bool)));
    connect(m_pFormDevicesContainer->m_pTreeDevice, SIGNAL(SelectedRigsChanged(QList<rig_ptr> &)),
        m_pFormDeviceDetail->m_pFormRigDetail->m_pDeviceTableModel, SLOT(SetInitialData(QList<rig_ptr> &)));
    connect(m_pFormDevicesContainer->m_pTreeDevice, SIGNAL(SelectedRigsChanged(QList<rig_ptr> &)),
        m_pFormDeviceDetail->m_pFormRigDetail->m_pUserTableModel, SLOT(SetInitialData(QList<rig_ptr> &)));
}

void H2SMonitorServer::runServices()
{
    CServiceHub::GetInstance()->CreateServicesOnlyOnce();

    bool ret = CServiceHub::GetInstance()->StartServices();
    if (!ret) {
        m_pFormServerControlPanel->serviceStartInterrupted();
    } else {
        emit m_pFormDevicesContainer->InitDeviceTreeNodes();
    }
}

void H2SMonitorServer::closeEvent(QCloseEvent * event)
{
    destroyServices();
    if (m_pLogViewer->isVisible()) {
        m_pLogViewer->close();
    }
}

void H2SMonitorServer::menuLogViewClicked(bool checked)
{
    m_pLogViewer->show();
}

void H2SMonitorServer::switchServiceStatus(bool startService)
{
    if (startService) {
        CLogService::GetInstance()->RegisterLogObserver(m_pLogViewer);
        runServices();
    } else {
        destroyServices();
    }
}

void H2SMonitorServer::destroyServices()
{
    // 停止运行所有内部服务
    CServiceHub::GetInstance()->StopServices();
    // 销毁所有内部服务实例
    CServiceHub::GetInstance()->DestroyServicesForEver();
    // 销毁内部服务管理器实例
    CServiceHub::DestroyInstance();
}
