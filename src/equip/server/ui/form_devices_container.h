#ifndef FORM_MAIN_H
#define FORM_MAIN_H

#include <QWidget>
#include <QTreeView>
#include <QStandardItem>
#include "ui/tree_model/device_tree_model.h"
#include "ui/tree_model/device_tree.h"



// ��״ͼ���г����еĵ��������ӡ��豸
class CFormDevicesContainer : public QWidget
{
    Q_OBJECT

public:
    CFormDevicesContainer(QWidget *parent);
    ~CFormDevicesContainer();

private:
    // �������沼��
    void BuildUI();

    // ��������
    void BeautifyUI();

    // �������Ԫ��
    void TranslateUI();

    // ���ź�
    void BindSlots();

signals:
    // ��ʼ���豸�����
    void InitDeviceTreeNodes();

private slots:
    // �����豸��
    void BuildTreeNodes();

public:
    CDeviceTree       * m_pTreeDevice;        // �豸�������ͼ

private:
    CDeviceTreeModel* m_pTreeDeviceModel;   // ����ģ��
};

#endif // FORM_MAIN_H
