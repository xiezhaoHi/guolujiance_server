#ifndef FORM_DEVICE_DATA_CURVE_H
#define FORM_DEVICE_DATA_CURVE_H

#include <QWidget>
#include "qwt.h"
#include "qwt_plot_curve.h"
#include "qwt_plot.h"

class CFormDeviceDataCurve : public QWidget
{
    Q_OBJECT

public:
    CFormDeviceDataCurve(QWidget *parent);

    ~CFormDeviceDataCurve();


private:
    
    // 构建单个设备数据曲线图界面
    void BuildUI();

    // 美化界面
    void BeautifyUI();

    // 翻译界面元素
    void TranslateUI();

    // 设置数据
    void SetupData();

private:
     QwtPlot      * m_plot;
     QwtPlotCurve * m_curve;
};

#endif // FORM_DEVICE_DATA_CURVE_H
