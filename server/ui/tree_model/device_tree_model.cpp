#include "device_tree_model.h"
#include "service/log_service/log_service.h"

CDeviceTreeModel::CDeviceTreeModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    CBaseEntity nodeInfo;
    nodeInfo.id = -1;
    nodeInfo.name = QStringLiteral("Root");
    m_root = new CDeviceTreeNode(NULL, NULL);
}

CDeviceTreeModel::~CDeviceTreeModel()
{
    if (m_root) {
        delete m_root;
    }
}

QModelIndex CDeviceTreeModel::index(int row, int column,
                                    const QModelIndex &parent /*= QModelIndex()*/) const
{
    if(!hasIndex(row, column, parent)){
        return QModelIndex();
    }

    CDeviceTreeNode * parentNode;
    if (!parent.isValid()) {
        parentNode = m_root;
    } else {
        parentNode = static_cast<CDeviceTreeNode *>(parent.internalPointer());
    }

    CDeviceTreeNode * childNode = parentNode->GetChild(row);
    if (childNode) {
        return createIndex(row, column, childNode);
    } else {
        return QModelIndex();
    }
}

QModelIndex CDeviceTreeModel::parent(const QModelIndex &child) const
{
    if (!child.isValid()) {
        return QModelIndex();
    }

    CDeviceTreeNode * childNode = static_cast<CDeviceTreeNode *>(child.internalPointer());
    if (childNode) {
        CDeviceTreeNode * parentNode = childNode->GetParent();
        if (parentNode && parentNode != m_root) {
            return createIndex(childNode->GetRow(), 0, parentNode);
        }
    }

    return QModelIndex();
}

int CDeviceTreeModel::rowCount(const QModelIndex &parent /*= QModelIndex()*/) const
{
    CDeviceTreeNode * parentNode;
    if (!parent.isValid()) {
        parentNode = m_root;
    } else {
        parentNode = static_cast<CDeviceTreeNode *>(parent.internalPointer());
    }

    return parentNode->ChildrenCount();
}

int CDeviceTreeModel::columnCount(const QModelIndex &parent /*= QModelIndex()*/) const
{
    return 1;
}

QVariant CDeviceTreeModel::data(const QModelIndex &index,
                                int role /*= Qt::DisplayRole*/) const
{
    CDeviceTreeNode * node;
    if (index.isValid()) {
        node = static_cast<CDeviceTreeNode *>(index.internalPointer());
    } else {
        node = m_root;
    }

    if (Qt::DisplayRole == role) {
        return node->GetDisplayText();
    } else if (Qt::EditRole == role) {

    } else if (Qt::TextAlignmentRole == role) {

    }

    return QVariant();
}

void CDeviceTreeModel::SetupData(CDeviceTreeNode * root)
{
    if (root) {
        if (root != m_root) {
            delete m_root;
            m_root = root;
        }
    } else {
        delete m_root;
        m_root = NULL;
    }
}

QVariant CDeviceTreeModel::headerData(int section, Qt::Orientation orientation,
                                      int role /*= Qt::DisplayRole*/) const
{
    if (Qt::DisplayRole == role) {
        if (Qt::Horizontal == orientation) {
            return QStringLiteral("H2SºÏ≤‚…Ë±∏");
        }
    }

    return QVariant();
}

Qt::ItemFlags CDeviceTreeModel::flags(const QModelIndex &index)
{
    if (!index.isValid()) {
        return 0;
    }

    return QAbstractItemModel::flags(index);
}
