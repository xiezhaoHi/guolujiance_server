#include "form_log_viewer.h"
#include <QGridLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QCheckBox>
#include <QListView>
#include <QListWidget>
#include <QSortFilterProxyModel>
#include <QSpacerItem>
#include <QStringListModel>
#include <QModelIndex>
#include <QApplication>
#include <QFile>
#include <QMutexLocker>
#include <QRect>
#include <QAbstractItemView>
#include <QListWidgetItem>
#include "service/log_service/log_service.h"
#include "service/configuration_service/configuration_service.h"


CFormLogViewer::CFormLogViewer(QWidget *parent)
    : QWidget(parent)
{
    BuildUI();
    BindEventHandlers();
    InitUIData();
    m_nTimerID = startTimer(500);
}

CFormLogViewer::~CFormLogViewer()
{
    killTimer(m_nTimerID);
}

void CFormLogViewer::BuildUI()
{
    QGridLayout * pLayoutSelf = new QGridLayout(this);

    m_pGroupBox = new QGroupBox(QStringLiteral("显示日志"), this);
    m_pCheckBoxShowDebugLevel = new QCheckBox(QStringLiteral("调试"), this);
    m_pCheckBoxShowInfoLevel = new QCheckBox(QStringLiteral("信息"), this);
    m_pCheckBoxShowWarningLevel = new QCheckBox(QStringLiteral("警告"), this);
    m_pCheckBoxShowErrorLevel = new QCheckBox(QStringLiteral("错误"), this);

    QHBoxLayout * pLayoutOfGroupBoxItem = new QHBoxLayout();
    pLayoutOfGroupBoxItem->addWidget(m_pCheckBoxShowDebugLevel);
    pLayoutOfGroupBoxItem->addWidget(m_pCheckBoxShowInfoLevel);
    pLayoutOfGroupBoxItem->addWidget(m_pCheckBoxShowWarningLevel);
    pLayoutOfGroupBoxItem->addWidget(m_pCheckBoxShowErrorLevel);
    pLayoutOfGroupBoxItem->addSpacerItem(new QSpacerItem(10, 10));
    pLayoutOfGroupBoxItem->setAlignment(Qt::AlignLeading);
    m_pGroupBox->setLayout(pLayoutOfGroupBoxItem);

    m_pListWidgetLogView = new QListWidget(this);

    pLayoutSelf->addWidget(m_pGroupBox, 0, 0, 1, 1, Qt::AlignTop);
    pLayoutSelf->addWidget(m_pListWidgetLogView, 1, 0, 1, 1);
    pLayoutSelf->setRowStretch(0, 0);
    pLayoutSelf->setRowStretch(1, 1);

    this->setWindowTitle(QStringLiteral("日志查看器"));
    this->setMinimumSize(600, 400);
    this->setLayout(pLayoutSelf);

    BeautifyUI();
    TranslateUI();
}

void CFormLogViewer::BeautifyUI()
{
    QPalette palette;
    palette.setColor(QPalette::Background, QColor(224, 224, 248));
    this->setAutoFillBackground(true);
    this->setPalette(palette);

    QFile qssFile(":/qss/form_log_viewer.qss");
    if (qssFile.open(QFile::ReadOnly)) {
        QString stylesheet = QLatin1String(qssFile.readAll());
        this->setStyleSheet(stylesheet);
        qssFile.close();
    }

    m_listCache = new QList<T_LEVEL_LOG_PAIR>();
    m_listUI = new QList<T_LEVEL_LOG_PAIR>();
}

void CFormLogViewer::TranslateUI()
{

}

void CFormLogViewer::BindEventHandlers()
{
    qRegisterMetaType<E_LOG_LEVEL>("E_LOG_LEVEL");

    connect(m_pCheckBoxShowDebugLevel, SIGNAL(clicked(bool)), this, SLOT(UpdateLogLevel()));
    connect(m_pCheckBoxShowInfoLevel, SIGNAL(clicked(bool)), this, SLOT(UpdateLogLevel()));
    connect(m_pCheckBoxShowWarningLevel, SIGNAL(clicked(bool)), this, SLOT(UpdateLogLevel()));
    connect(m_pCheckBoxShowErrorLevel, SIGNAL(clicked(bool)), this, SLOT(UpdateLogLevel()));
}

void CFormLogViewer::HandleLog(E_LOG_LEVEL level, const QString & log)
{
    bool ok = false;
    switch (level)
    {
    case LEVEL_DEBUG:
        ok = m_pCheckBoxShowDebugLevel->isChecked();
        break;

    case LEVEL_INFO:
        ok = m_pCheckBoxShowInfoLevel->isChecked();
        break;

    case LEVEL_WARING:
        ok = m_pCheckBoxShowWarningLevel->isChecked();
        break;

    case LEVEL_ERROR:
        ok = m_pCheckBoxShowErrorLevel->isChecked();
        break;

    default:
        break;
    }

    if (!ok) {
        return;
    }

    QMutexLocker lock(&m_mutex);
    m_listCache->push_back(T_LEVEL_LOG_PAIR(level, log));
}

void CFormLogViewer::LogSlot(/*E_LOG_LEVEL level,*/ const QString & log)
{
    m_pListWidgetLogView->insertItem(0, log);
    if (m_pListWidgetLogView->count() > 500) {
        for (int i = m_pListWidgetLogView->count() - 1; i >= 300; i--) {
            QListWidgetItem * item = m_pListWidgetLogView->takeItem(i);
            delete item;
        }
    }
}

void CFormLogViewer::timerEvent(QTimerEvent *)
{
    {
        QMutexLocker lock(&m_mutex);
        QList<T_LEVEL_LOG_PAIR> * pTemp = m_listUI;
        m_listUI = m_listCache;
        m_listCache = pTemp;
        if (m_listUI->empty()) {
            return;
        }
    }

    QListWidgetItem * first = m_pListWidgetLogView->item(0);
    QListWidgetItem * current = m_pListWidgetLogView->currentItem();
    for (int i = 0; i < m_listUI->size(); i++) {
        LogSlot(m_listUI->at(i).log);
    }

    if (!current || first == current) {
        m_pListWidgetLogView->setCurrentRow(0);
    }

    m_listUI->clear();
}

void CFormLogViewer::InitUIData()
{
    switch (CConfigrationService::GetInstance()->GetLogLevel())
    {
    case LEVEL_NONE:
        break;

    case LEVEL_DEBUG:
        m_pCheckBoxShowDebugLevel->setChecked(true);

    case LEVEL_INFO:
        m_pCheckBoxShowInfoLevel->setChecked(true);

    case LEVEL_WARING:
        m_pCheckBoxShowWarningLevel->setChecked(true);

    case LEVEL_ERROR:
        m_pCheckBoxShowErrorLevel->setChecked(true);

    default:
        break;
    }
}

void CFormLogViewer::UpdateLogLevel()
{
    if (m_pCheckBoxShowDebugLevel->isChecked()) {
        CConfigrationService::GetInstance()->SetLogLevel(LEVEL_DEBUG);
    } else if (m_pCheckBoxShowInfoLevel->isChecked()) {
        CConfigrationService::GetInstance()->SetLogLevel(LEVEL_INFO);
    } else if (m_pCheckBoxShowWarningLevel->isChecked()) {
        CConfigrationService::GetInstance()->SetLogLevel(LEVEL_WARING);
    } else if (m_pCheckBoxShowErrorLevel->isChecked()) {
        CConfigrationService::GetInstance()->SetLogLevel(LEVEL_ERROR);
    }
}

QSize CFormLogViewer::sizeHint() const
{
    return QSize(1050, 550);
}
