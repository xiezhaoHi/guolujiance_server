#include "form_device_data_curve_container.h"
#include <QPushButton>
#include <QGridLayout>
#include <QSize>
#include <QDebug>
#include <QScrollBar>
#include <QFile>

CFormDeviceDataCurveContainer::CFormDeviceDataCurveContainer(QWidget *parent)
    : QWidget(parent)
{
    BuildUI();
    BindSlots();
}

CFormDeviceDataCurveContainer::~CFormDeviceDataCurveContainer()
{

}

void CFormDeviceDataCurveContainer::BuildUI()
{
    m_pScrollAreaWidget = new QWidget(this);
    m_pLayoutOfScrollAreaWidget = new FlowLayout(this, 5, 6, 6);
    m_pScrollAreaWidget->setLayout(m_pLayoutOfScrollAreaWidget);
    //m_pScrollAreaWidget->setMinimumSize(880, 560);

    m_pScrollArea = new QScrollArea(this);
    m_pScrollArea->setBackgroundRole(QPalette::Dark);
    m_pScrollArea->setWidget(m_pScrollAreaWidget);

    QGridLayout * pLayoutSelf = new QGridLayout(this);
    pLayoutSelf->addWidget(m_pScrollArea);
    pLayoutSelf->setRowStretch(0, 1);
    pLayoutSelf->setColumnStretch(0, 1);
    this->setLayout(pLayoutSelf);

    BeautifyUI();
    TranslateUI();
    TestReLayoutSubWidget();
}

void CFormDeviceDataCurveContainer::BeautifyUI()
{
    this->setObjectName(QStringLiteral("formdevicedatacurvecontainer"));
    m_pScrollArea->setObjectName(QStringLiteral("formdevicedatacurvecontainer_scrollarea"));
    m_pScrollAreaWidget->setObjectName(QStringLiteral("formdevicedatacurvecontainer_scrollareawidget"));

    //QPalette palette;
    //palette.setColor(QPalette::Background, QColor(224, 224, 248));
    //m_pScrollAreaWidget->setPalette(palette);

    QString stylesheet;
    QFile qssFile(":/qss/form_device_data_curve_container.qss");
    if (qssFile.open(QFile::ReadOnly)) {
        stylesheet = QLatin1String(qssFile.readAll());
        this->setStyleSheet(stylesheet);
        qssFile.close();
    }
}

void CFormDeviceDataCurveContainer::TranslateUI()
{
    // do nothing
}

void CFormDeviceDataCurveContainer::DisplayCurve(CFormDeviceDataCurve * curve)
{
    m_listCurves.append(curve);
    m_pLayoutOfScrollAreaWidget->addWidget(curve);
    m_pScrollAreaWidget->setLayout(m_pLayoutOfScrollAreaWidget);
}

void CFormDeviceDataCurveContainer::BindSlots()
{
    // TODO
}


void CFormDeviceDataCurveContainer::TestReLayoutSubWidget()
{
    // TODO

    for (int i = 0; i < 10; i++) {
        CFormDeviceDataCurve * curve = new CFormDeviceDataCurve(this);
        DisplayCurve(curve);
    }

    ResizeContainer();
}

void CFormDeviceDataCurveContainer::resizeEvent(QResizeEvent *)
{
  //  ResizeContainer();
}

void CFormDeviceDataCurveContainer::ResizeContainer()
{
    QSize sizeOfMine = this->size();
    if (sizeOfMine.width() == 0 || sizeOfMine.height() == 0) {
        return;
    }

    QSize sizeOfCurve = m_pLayoutOfScrollAreaWidget->itemAt(0)->sizeHint();
    if (sizeOfMine.width() < sizeOfCurve.width()) {
        return;
    }

    int countOfCurves = m_listCurves.count();
    int spacingOfEachRow = sizeOfCurve.width() > 0 ?
        sizeOfMine.width() % sizeOfCurve.width() : 0;
    int totalWidth = (sizeOfCurve.width() + m_pLayoutOfScrollAreaWidget->horizontalSpacing() * 2) * countOfCurves;
    int countOfRow = totalWidth / (sizeOfMine.width() - spacingOfEachRow);
    if ((countOfCurves * sizeOfCurve.width()) % sizeOfMine.width() > 0) {
        countOfRow++;
    }
    int countOfEachRow = countOfCurves / countOfRow;
    int width = sizeOfMine.width();
        width -= m_pScrollArea->verticalScrollBar()->width() * 3;
    int height = sizeOfCurve.height() * countOfRow 
        + m_pLayoutOfScrollAreaWidget->verticalSpacing() * countOfRow * 2;

    int hSpacing = spacingOfEachRow / (2 * countOfEachRow);
    m_pLayoutOfScrollAreaWidget->setContentsMargins(hSpacing, 5, hSpacing, 5);

    qDebug() << "total spacing: " << spacingOfEachRow << " hSpacing: " << hSpacing
        << "count of Row:" << countOfEachRow;

    // 面板设置大小
    m_pScrollAreaWidget->setFixedSize(width, height);

    // 布局设置左右空白的宽度
    // TODO
}

