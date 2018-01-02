#include "form_rig_device_detail.h"
#include <QFile>
#include <QString>
#include <QStackedLayout>
#include <QPushButton>
#include "service/log_service/log_service.h"



CFormRigDeviceDetail::CFormRigDeviceDetail(QWidget *parent)
    : QWidget(parent)
{
    BuildUI();
    BindSlots();
}

CFormRigDeviceDetail::~CFormRigDeviceDetail()
{

}

void CFormRigDeviceDetail::BuildUI()
{
    QStackedLayout * pLayoutSelf = new QStackedLayout(this);

    m_pFormRigDetail = new CFormRigDetail(this);
    pLayoutSelf->addWidget(m_pFormRigDetail);

    this->setLayout(pLayoutSelf);

    BeautifyUI();
    TranslateUI();
}

void CFormRigDeviceDetail::BeautifyUI()
{
    this->setObjectName(QStringLiteral("formdevicedetail"));

    QFile qssFile(":/qss/form_device_detail.qss");
    if (qssFile.open(QFile::ReadOnly)) {
        QString stylesheet = QLatin1String(qssFile.readAll());
        this->setStyleSheet(stylesheet);
        qssFile.close();
    }
}

void CFormRigDeviceDetail::TranslateUI()
{
    // do nothing
}

void CFormRigDeviceDetail::BindSlots()
{

}

