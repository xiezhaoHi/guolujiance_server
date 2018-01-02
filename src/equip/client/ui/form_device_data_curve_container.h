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

    // ��ʾ������ͼ
    void DisplayCurve(CFormDeviceDataCurve * curve);

protected:

    virtual void resizeEvent(QResizeEvent *);

private:

    // ������������豸��������ͼ����
    void BuildUI();

    // ��������
    void BeautifyUI();

    // �������Ԫ��
    void TranslateUI();

    // �����뱾������ص��ź����
    void BindSlots();

    // ���������������ڵĴ�С 
    void ResizeContainer();

    // ���ӽ������²���
    void TestReLayoutSubWidget();

private:
    QList<CFormDeviceDataCurve *>   m_listCurves;
    QScrollArea                   * m_pScrollArea;
    QWidget                       * m_pScrollAreaWidget;
    FlowLayout                    * m_pLayoutOfScrollAreaWidget;
};

#endif // FORM_DEVICE_DATA_CURVE_CONTAINER_H
