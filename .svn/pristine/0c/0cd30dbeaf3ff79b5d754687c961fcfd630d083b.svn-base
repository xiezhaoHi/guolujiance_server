#include "form_device_data_curve.h"
#include <QGridLayout>
#include <QPushButton>

CFormDeviceDataCurve::CFormDeviceDataCurve(QWidget *parent)
    : QWidget(parent)
{
    BuildUI();
    SetupData();
}

CFormDeviceDataCurve::~CFormDeviceDataCurve()
{

}

void CFormDeviceDataCurve::BuildUI()
{
    m_plot = new QwtPlot(this);
    m_plot->setFixedSize(360, 220);
    
    m_curve = new QwtPlotCurve(QStringLiteral("curve"));
    m_curve->attach(m_plot);

    QGridLayout * pLayoutSelf = new QGridLayout(this);
    pLayoutSelf->addWidget(m_plot, 0, 0, 1, 1);

    this->setLayout(pLayoutSelf);
    this->setMinimumSize(380, 240);

    BeautifyUI();
    TranslateUI();
}

void CFormDeviceDataCurve::BeautifyUI()
{
    // TODO
    QPalette palette;
    palette.setColor(QPalette::Background, QColor(255, 224, 248));
    this->setAutoFillBackground(false);
    this->setPalette(palette);
}

void CFormDeviceDataCurve::TranslateUI()
{

}

void CFormDeviceDataCurve::SetupData()
{
    //XÖá
    double time[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    //YÖá
    double val[10] = {3, 5, 8, 7, 2, 0, 7, 9, 1};
    m_curve->setSamples(time, val, 10);
}
