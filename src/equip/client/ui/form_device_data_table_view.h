#ifndef FORM_DEVICE_DATA_TABLE_VIEW_H
#define FORM_DEVICE_DATA_TABLE_VIEW_H

#include <QWidget>
#include <QTableView>


class CFormDeviceDataTableView : public QWidget
{
    Q_OBJECT

public:
    CFormDeviceDataTableView(QWidget *parent);
    ~CFormDeviceDataTableView();

private:

    // �������������豸���ݱ����ͼ����
    void BuildUI();

    // ��������
    void BeautifyUI();

    // �������Ԫ��
    void TranslateUI();

private:
    QTableView          * m_pTableView;
};

#endif // FORM_DEVICE_DATA_TABLE_VIEW_H
