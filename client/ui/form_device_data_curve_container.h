#ifndef FORM_DEVICE_DATA_CURVE_CONTAINER_H
#define FORM_DEVICE_DATA_CURVE_CONTAINER_H

#include <QScrollArea>
#include <QWidget>
#include <QList>
#include "form_device_data_curve.h"
#include "flow_layout.h"

class CFormDeviceDataCurveContainer : public QWidget
{
    Q_OBJECT

public:
    CFormDeviceDataCurveContainer(QWidget *parent);
    ~CFormDeviceDataCurveContainer();

    // 显示新曲线图
    void DisplayCurve(CFormDeviceDataCurve * curve);

protected:

    virtual void resizeEvent(QResizeEvent *);

private:

    // 构建包含多个设备数据曲线图界面
    void BuildUI();

    // 美化界面
    void BeautifyUI();

    // 翻译界面元素
    void TranslateUI();

    // 连接与本界面相关的信号与槽
    void BindSlots();

    // 重新设置容器窗口的大小 
    void ResizeContainer();

    // 对子界面重新布局
    void TestReLayoutSubWidget();

private:
    QList<CFormDeviceDataCurve *>   m_listCurves;
    QScrollArea                   * m_pScrollArea;
    QWidget                       * m_pScrollAreaWidget;
    FlowLayout                    * m_pLayoutOfScrollAreaWidget;
};

#endif // FORM_DEVICE_DATA_CURVE_CONTAINER_H
