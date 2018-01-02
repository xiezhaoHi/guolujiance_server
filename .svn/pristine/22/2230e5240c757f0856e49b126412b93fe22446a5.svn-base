#include "form_server_control_panel.h"
#include "service/service_hub.h"
#include "service/log_service/log_service.h"
#include <QGridLayout>
#include <QFile>
#include <QVariant>

CFormServerControlPanel::CFormServerControlPanel(QWidget *parent)
    : QWidget(parent)
{
    BuildUI();
    BindSlots();
}

CFormServerControlPanel::~CFormServerControlPanel()
{

}

void CFormServerControlPanel::BuildUI()
{
    QGridLayout * pMainLayout = new QGridLayout(this);
    pMainLayout->setObjectName(QStringLiteral("formservercontrolpanel_mainlayout"));
    pMainLayout->setVerticalSpacing(5);
    pMainLayout->setHorizontalSpacing(5);

    m_pLabelDurationOfServicePrefix = new QLabel(this);
    m_pLabelDurationOfServicePrefix->setText(QStringLiteral("服务器已持续运行"));
    pMainLayout->addWidget(m_pLabelDurationOfServicePrefix, 0, 0, 1, 1, Qt::AlignLeft);

    m_pLabelDurationOfService = new QLabel(this);
    m_pLabelDurationOfService->setText(QStringLiteral("服务器未启动"));
    pMainLayout->addWidget(m_pLabelDurationOfService, 1, 0, 1, 1, Qt::AlignLeft);

    m_pButtonServiceStateSwitch = new QPushButton(this);
    m_pButtonServiceStateSwitch->setText(QStringLiteral("开始运行"));
    m_pButtonServiceStateSwitch->setMinimumHeight(50);
    m_pButtonServiceStateSwitch->setMinimumWidth(100);
    m_pButtonServiceStateSwitch->setFixedWidth(220);
    pMainLayout->addWidget(m_pButtonServiceStateSwitch, 0, 1, 2, 1, Qt::AlignHCenter);
    pMainLayout->setRowStretch(1, 1);
    this->setLayout(pMainLayout);

    m_pLabelDurationOfServicePrefix->setVisible(false);

    m_pTimerUpdateTotalRunTime = new QTimer(this);

    BeautifyUI();
    TranslateUI();
}

void CFormServerControlPanel::BeautifyUI()
{
    QPalette palette;
    palette.setColor(QPalette::Background, QColor(208, 209, 215));
    this->setAutoFillBackground(true);
    this->setPalette(palette);

    m_pLabelDurationOfServicePrefix->setAttribute(Qt::WA_TranslucentBackground);
    m_pLabelDurationOfService->setAttribute(Qt::WA_TranslucentBackground);

    this->setObjectName(QStringLiteral("formservercontrolpanel"));
    m_pLabelDurationOfServicePrefix->setObjectName(QStringLiteral("formservercontrolpanel_label_durationofserviceprefix"));
    m_pLabelDurationOfService->setObjectName(QStringLiteral("formservercontrolpanel_label_durationofservice"));
    m_pButtonServiceStateSwitch->setObjectName(QStringLiteral("formservercontrolpanel_button_servicestateswitch"));

    QFile qssFile(":/qss/form_server_control_panel.qss");
    if (qssFile.open(QFile::ReadOnly)) {
        QString stylesheet = QLatin1String(qssFile.readAll());
        this->setStyleSheet(stylesheet);
        qssFile.close();
    }

    m_pButtonServiceStateSwitch->setProperty("IsInService", false);
}

void CFormServerControlPanel::TranslateUI()
{
    // do nothing
}

void CFormServerControlPanel::BindSlots()
{
    connect(m_pButtonServiceStateSwitch, SIGNAL(clicked()), this, SLOT(SwitchServiceStatus()));
    connect(m_pTimerUpdateTotalRunTime, SIGNAL(timeout()), this, SLOT(UpdateTotalRunTime()));
}

void CFormServerControlPanel::SwitchServiceStatus()
{
    m_pLabelDurationOfServicePrefix->setVisible(CServiceHub::GetInstance()->IsServicesRunning());
    if (!CServiceHub::GetInstance()->IsServicesRunning()) {
        
        m_pLabelDurationOfService->setText(QStringLiteral("服务器正在启动"));
    } else {
        m_pLabelDurationOfService->setText(QStringLiteral("服务器正在停止"));
    }

    // 启动/停止服务
    emit SwitchSerivceStatus(!CServiceHub::GetInstance()->IsServicesRunning());
    m_pLabelDurationOfServicePrefix->setVisible(CServiceHub::GetInstance()->IsServicesRunning());
    m_pButtonServiceStateSwitch->setProperty("IsInService", CServiceHub::GetInstance()->IsServicesRunning());
    ReloadQSS();

    if (CServiceHub::GetInstance()->IsServicesRunning()) {
        m_pButtonServiceStateSwitch->setText(QStringLiteral("停止运行"));
        m_timeStartup = QDateTime::currentMSecsSinceEpoch();
        m_pTimerUpdateTotalRunTime->setInterval(1000);
        m_pTimerUpdateTotalRunTime->start();
    } else {
        if (m_pTimerUpdateTotalRunTime->isActive()) {
            m_pTimerUpdateTotalRunTime->stop();
        }
        m_pLabelDurationOfService->setText(QStringLiteral("服务已停止"));
        m_pButtonServiceStateSwitch->setText(QStringLiteral("开始运行"));
    }
}

void CFormServerControlPanel::UpdateTotalRunTime()
{
    qint64 current = QDateTime::currentMSecsSinceEpoch();
    qint64 elapsed = current - m_timeStartup;

    static int milliSecondsInOneSecond = 1000;
    static int milliSecondsInOneMinute = 60 * milliSecondsInOneSecond;
    static int milliSecondsInOneHour = 60 * milliSecondsInOneMinute;
    static int milliSecondsInOneDay = 24 * milliSecondsInOneHour;

    int days, hours, minutes, seconds;
    days = elapsed / milliSecondsInOneDay;
    hours = (elapsed % milliSecondsInOneDay) / milliSecondsInOneHour;
    minutes = (elapsed % milliSecondsInOneHour) / milliSecondsInOneMinute;
    seconds = (elapsed % milliSecondsInOneMinute) / milliSecondsInOneSecond;

    QString qstrTotalTime;
    if (days > 0) {
        qstrTotalTime = QStringLiteral("%1天%2小时%3分%4秒")
            .arg(days, 2).arg(hours, 2).arg(minutes, 2).arg(seconds, 2);
    } else if (hours > 0) {
        qstrTotalTime = QStringLiteral("   %1小时%2分%3秒")
            .arg(hours, 2).arg(minutes, 2).arg(seconds, 2);
    } else if (minutes > 0) {
        qstrTotalTime = QStringLiteral("       %1分%2秒")
            .arg(minutes, 2).arg(seconds, 2);
    } else {
        qstrTotalTime = QStringLiteral("         %2秒")
            .arg(seconds, 2);
    }

    m_pLabelDurationOfService->setText(qstrTotalTime);
}

void CFormServerControlPanel::ReloadQSS()
{
    QFile qssFile(":/qss/form_server_control_panel.qss");
    if (qssFile.open(QFile::ReadOnly)) {
        QString stylesheet = QLatin1String(qssFile.readAll());
        this->setStyleSheet(stylesheet);
        qssFile.close();
    }
}

void CFormServerControlPanel::serviceStartInterrupted()
{
    m_pLabelDurationOfServicePrefix->setVisible(false);
    m_pLabelDurationOfService->setText(QStringLiteral("服务已中止启动"));
}
