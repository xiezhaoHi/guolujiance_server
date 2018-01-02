#ifndef DEVICE_TREE_MODEL_H
#define DEVICE_TREE_MODEL_H

#include <QObject>
#include <QAbstractItemModel>
#include "device_tree_node.h"

class CDeviceTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit CDeviceTreeModel(QObject *parent = 0);

    ~CDeviceTreeModel();

    virtual QModelIndex index(int row, int column,
        const QModelIndex &parent = QModelIndex()) const;

    virtual QModelIndex parent(const QModelIndex &child) const;

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;

    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant headerData(int section, Qt::Orientation orientation, 
        int role = Qt::DisplayRole) const;

    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    virtual Qt::ItemFlags flags(const QModelIndex &index);

    void SetupData(CDeviceTreeNode * root);

private:
    CDeviceTreeNode * m_root;
};

#endif // DEVICE_TREE_MODEL_H
