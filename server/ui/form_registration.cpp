#include "form_registration.h"
#include "service/log_service/log_service.h"
#include "registration/registration.h"
#include <QGridLayout>
#include <QMessageBox>
#include <QFile>



CFormRegistration::CFormRegistration(QWidget *parent)
    : QDialog(parent)
{
    BuildUI();
    BindSlots();
}

CFormRegistration::~CFormRegistration()
{

}

void CFormRegistration::BuildUI()
{
    QGridLayout * pLayoutSelf = new QGridLayout(this);
    pLayoutSelf->setMargin(15);

    m_pLabelSerialNumber = new QLabel(this);
    m_pLabelSerialNumber->setText(QStringLiteral("ÐòÁÐºÅ£º"));
    pLayoutSelf->addWidget(m_pLabelSerialNumber, 0, 0, 1, 1, Qt::AlignRight);

    m_pLineEditSerialNumber = new QLineEdit(this);
    m_pLineEditSerialNumber->setReadOnly(true);
    m_pLabelSerialNumber->setBuddy(m_pLineEditSerialNumber);
    pLayoutSelf->addWidget(m_pLineEditSerialNumber, 0, 1, 1, 1);

    m_pLabelRegistrationCode = new QLabel(this);
    m_pLabelRegistrationCode->setText(QStringLiteral("×¢²áÂë£º"));
    pLayoutSelf->addWidget(m_pLabelRegistrationCode, 1, 0, 1, 1, Qt::AlignRight | Qt::AlignTop);

    m_pTextEditRegistrationCode = new QTextEdit(this);
    m_pLabelRegistrationCode->setBuddy(m_pTextEditRegistrationCode);
    pLayoutSelf->addWidget(m_pTextEditRegistrationCode, 1, 1, 1, 1);

    m_pButtonApply = new QPushButton(this);
    m_pButtonApply->setText(QStringLiteral("×¢²á"));
    m_pButtonApply->setEnabled(false);
    pLayoutSelf->addWidget(m_pButtonApply, 2, 0, 1, 2, Qt::AlignHCenter);

    this->setLayout(pLayoutSelf);
    this->setFixedSize(400, 200);
    this->setWindowTitle(QStringLiteral("×¢²áÈí¼þ"));

    BeautifyUI();
    TranslateUI();
}

void CFormRegistration::BeautifyUI()
{
    QPalette palette;
    palette.setColor(QPalette::Background, QColor(224, 224, 248));
    this->setAutoFillBackground(true);
    this->setPalette(palette);

    QFile qssFile(":/qss/form_registration.qss");
    if (qssFile.open(QFile::ReadOnly)) {
        QString stylesheet = QLatin1String(qssFile.readAll());
        this->setStyleSheet(stylesheet);
        qssFile.close();
    }
}

void CFormRegistration::TranslateUI()
{

}

void CFormRegistration::BindSlots()
{
    connect(m_pTextEditRegistrationCode, SIGNAL(textChanged()), this, SLOT(UpdateButtonState()));
    connect(m_pButtonApply, SIGNAL(clicked()), this, SLOT(ButtonApplyClicked()));
}

void CFormRegistration::ShowRegistDialog(QString serialNumber)
{
    m_pLineEditSerialNumber->setText(serialNumber);
    exec();
}

void CFormRegistration::ButtonApplyClicked()
{
    QString registrationCode = m_pTextEditRegistrationCode->toPlainText();
    LOG_INFO() << QStringLiteral("×¢²á");

    if (Regist(registrationCode)) {
        close();
    } else {
        QMessageBox::warning(this, QStringLiteral("¾¯¸æ"), 
            QStringLiteral("×¢²áÂëÎÞÐ§£¬ÇëÁªÏµ³§ÉÌ»ñÈ¡ÓÐÐ§×¢²áÂë£¡"), QMessageBox::Ok);
    }
}

void CFormRegistration::UpdateButtonState()
{
    if (m_pTextEditRegistrationCode->toPlainText().simplified().isEmpty()) {
        m_pButtonApply->setEnabled(false);
    } else {
        m_pButtonApply->setEnabled(true);
    }
}

