#ifndef DEVICE_TREE_H
#define DEVICE_TREE_H

#include <QList>
#include <QTreeView>
#include "tree_model/device_tree_model.h"
#include "service/business_handle_service/business_model/rig.h"
#include "service/business_handle_service/business_model/device.h"


class CDeviceTree : public QTreeView
{
    Q_OBJECT

public:
    CDeviceTree(QWidget *parent);
    ~CDeviceTree();

    virtual void currentChanged(const QModelIndex &current, const QModelIndex &previous);

signals:
    void SelectedRigsChanged(QList<rig_ptr> & lst);

private:
    CDeviceTreeModel* m_pTreeDeviceModel;   // 数据模型
};

#endif // DEVICE_TREE_H
