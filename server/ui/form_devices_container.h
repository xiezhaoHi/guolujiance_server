#ifndef FORM_MAIN_H
#define FORM_MAIN_H

#include <QWidget>
#include <QTreeView>
#include <QStandardItem>
#include "ui/tree_model/device_tree_model.h"
#include "ui/tree_model/device_tree.h"



// 树状图，列出所有的地区、井队、设备
class CFormDevicesContainer : public QWidget
{
    Q_OBJECT

public:
    CFormDevicesContainer(QWidget *parent);
    ~CFormDevicesContainer();

private:
    // 构建界面布局
    void BuildUI();

    // 美化界面
    void BeautifyUI();

    // 翻译界面元素
    void TranslateUI();

    // 绑定信号
    void BindSlots();

signals:
    // 初始化设备树结点
    void InitDeviceTreeNodes();

private slots:
    // 构建设备树
    void BuildTreeNodes();

public:
    CDeviceTree       * m_pTreeDevice;        // 设备结点树视图

private:
    CDeviceTreeModel* m_pTreeDeviceModel;   // 数据模型
};

#endif // FORM_MAIN_H
