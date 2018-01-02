#ifndef DEVICE_TREE_NODE_H
#define DEVICE_TREE_NODE_H

#include <QObject>
#include <QString>
#include <QList>
#include "structs/structs.h"

// 树结点类型
typedef enum _E_TREE_NODE_TYPE
{
    UNKNOWN = -1,
    ROOT = 0,       // 根结点
    OIL_FIELD,      // 油田
    RIG,            // 井队
    DEVICE,         // 设备
}E_TREE_NODE_TYPE;



// 树状视图上的单个结点
class CDeviceTreeNode : public QObject
{
    Q_OBJECT

public:
    explicit CDeviceTreeNode(QObject *parent = 0);

    explicit CDeviceTreeNode(
        base_entity_ptr nodeInfo, QObject *parent = 0);

    CDeviceTreeNode(const CDeviceTreeNode & node);

    ~CDeviceTreeNode();

    CDeviceTreeNode & operator=(const CDeviceTreeNode & node);

    // 返回树结点上显示的文本
    QString GetDisplayText();

    // 设置父结点
    void SetParent(CDeviceTreeNode * parent);

    // 追加子结点
    void AppendChild(CDeviceTreeNode * child);

    // 移除子结点
    void RemoveChild(CDeviceTreeNode * child);

    // 获取子结点
    CDeviceTreeNode * GetChild(int index);

    // 获取父结点
    CDeviceTreeNode * GetParent();

    // 子结点数
    int ChildrenCount();

    // 列数
    int ColumnCount();

    // 返回当前结点在父结点的子结点列表里占用的索引号
    int GetRow();

    // 获取结点实体
    base_entity_ptr GetNodeEntity();

private:
    base_entity_ptr         m_nodeEntity;     // 结点信息
    CDeviceTreeNode       * m_pParent;      // 
    QList<CDeviceTreeNode *>m_listChildren; // 
};



Q_DECLARE_METATYPE(CDeviceTreeNode)

#endif // DEVICE_TREE_NODE_H
