#include "device_tree_node.h"

CDeviceTreeNode::CDeviceTreeNode(QObject *parent)
    : QObject(parent)
{
    this->m_pParent = NULL;
}

CDeviceTreeNode::CDeviceTreeNode(
                                 base_entity_ptr nodeInfo,
                                 QObject *parent /*= 0*/)
                                 : QObject(parent)
{
    this->m_nodeEntity = nodeInfo;
    this->m_pParent = NULL;
}

CDeviceTreeNode::CDeviceTreeNode(const CDeviceTreeNode & node)
{
    this->m_nodeEntity = node.m_nodeEntity;
    this->m_pParent = node.m_pParent;
    this->m_listChildren.clear();
    this->m_listChildren.append(node.m_listChildren);
}

CDeviceTreeNode::~CDeviceTreeNode()
{
    while (!m_listChildren.isEmpty()) {
        CDeviceTreeNode * child = m_listChildren.first();
        delete child;

        m_listChildren.pop_front();
    }
}

CDeviceTreeNode & CDeviceTreeNode::operator=(const CDeviceTreeNode & node)
{
    this->m_pParent = node.m_pParent;
    this->m_nodeEntity = node.m_nodeEntity;
    return *this;
}

//******************************************************
//** 函数名:   GetDisplayText
//** 功能简述: 返回本节点在树状图结点上要显示的文本
//** 输入参数: 
//
//** 返回值: 
//   QT_NAMESPACE::QString:
//
//** 创建日期：2015/07/13
//** 创建人：  唐桥
//** 修改记录：
//   日期        版本      修改人    修改内容
//
//** 其它说明：
//******************************************************
QString CDeviceTreeNode::GetDisplayText()
{
    if (!m_nodeEntity) {
        return QStringLiteral("未知");
    }
    if (ENTITY_ROOT == m_nodeEntity->type) {
        return QStringLiteral("H2S监控服务器");
    } else if (ENTITY_OIL_FIELD == m_nodeEntity->type) {
        return m_nodeEntity->name;
    } else if (ENTITY_RIG == m_nodeEntity->type) {
        return m_nodeEntity->name;
    } else if (ENTITY_DEVICE == m_nodeEntity->type) {
        device_info_ptr ptr = std::static_pointer_cast<CDeviceInfo>(m_nodeEntity);
        return QStringLiteral("%1 %2").arg(m_nodeEntity->name).arg(ptr->code);
    } else {
        return QStringLiteral("未知");
    }
}

//******************************************************
//** 函数名:   SetParent
//** 功能简述: 设置父结点
//** 输入参数: 
//   CDeviceTreeNode * parent:
//
//** 返回值: 
//   void:
//
//** 创建日期：2015/07/13
//** 创建人：  唐桥
//** 修改记录：
//   日期        版本      修改人    修改内容
//
//** 其它说明：
//******************************************************
void CDeviceTreeNode::SetParent(CDeviceTreeNode * parent)
{
    if (m_pParent && m_pParent != parent) {
        m_pParent->RemoveChild(this);
        m_pParent = NULL;
    }

    m_pParent = parent;
}

//******************************************************
//** 函数名:   AppendChild
//** 功能简述: 追加子结点
//** 输入参数: 
//   CDeviceTreeNode * child:
//
//** 返回值: 
//   void:
//
//** 创建日期：2015/07/13
//** 创建人：  唐桥
//** 修改记录：
//   日期        版本      修改人    修改内容
//
//** 其它说明：
//******************************************************
void CDeviceTreeNode::AppendChild(CDeviceTreeNode * child)
{
    if (m_listChildren.contains(child)) {
        return;
    }

    child->SetParent(this);
    m_listChildren.append(child);
}

//******************************************************
//** 函数名:   RemoveChild
//** 功能简述: 移除子结点
//** 输入参数: 
//   CDeviceTreeNode * child:
//
//** 返回值: 
//   void:
//
//** 创建日期：2015/07/13
//** 创建人：  唐桥
//** 修改记录：
//   日期        版本      修改人    修改内容
//
//** 其它说明：
//******************************************************
void CDeviceTreeNode::RemoveChild(CDeviceTreeNode * child)
{
    if (m_listChildren.contains(child)) {
        m_listChildren.removeOne(child);
    }
}

//******************************************************
//** 函数名:   ChildrentCount
//** 功能简述: 获取子结点数
//** 输入参数: 
//
//** 返回值: 
//   int:
//
//** 创建日期：2015/07/13
//** 创建人：  唐桥
//** 修改记录：
//   日期        版本      修改人    修改内容
//
//** 其它说明：
//******************************************************
int CDeviceTreeNode::ChildrenCount()
{
    return m_listChildren.count();
}

//******************************************************
//** 函数名:   ColumnCount
//** 功能简述: 获取列数
//** 输入参数: 
//
//** 返回值: 
//   int:
//
//** 创建日期：2015/07/13
//** 创建人：  唐桥
//** 修改记录：
//   日期        版本      修改人    修改内容
//
//** 其它说明：
//******************************************************
int CDeviceTreeNode::ColumnCount()
{
    return 1;
}

//******************************************************
//** 函数名:   GetChild
//** 功能简述: 获取指定索引处的子结点
//** 输入参数: 
//   int index:
//
//** 返回值: 
//   CDeviceTreeNode *:
//
//** 创建日期：2015/07/13
//** 创建人：  唐桥
//** 修改记录：
//   日期        版本      修改人    修改内容
//
//** 其它说明：
//******************************************************
CDeviceTreeNode * CDeviceTreeNode::GetChild(int index)
{
    _ASSERT(index >= 0);
    if (index <= m_listChildren.count()) {
        return m_listChildren.at(index);
    } else {
        return NULL;
    }
}

//******************************************************
//** 函数名:   GetParent
//** 功能简述: 获取父结点
//** 输入参数: 
//
//** 返回值: 
//   CDeviceTreeNode *:
//
//** 创建日期：2015/07/13
//** 创建人：  唐桥
//** 修改记录：
//   日期        版本      修改人    修改内容
//
//** 其它说明：
//******************************************************
CDeviceTreeNode * CDeviceTreeNode::GetParent()
{
    return m_pParent;
}

//******************************************************
//** 函数名:   GetRow
//** 功能简述: 获取当前结点在父结点的子结点列表里的索引值
//** 输入参数: 
//
//** 返回值: 
//   int:
//
//** 创建日期：2015/07/13
//** 创建人：  唐桥
//** 修改记录：
//   日期        版本      修改人    修改内容
//
//** 其它说明：
//******************************************************
int CDeviceTreeNode::GetRow()
{
    int index = 0;

    if (m_pParent) {
        index = m_pParent->m_listChildren.indexOf(this, 0);
        index = (index == -1) ? 0 : index;
    }

    return index;
}

base_entity_ptr CDeviceTreeNode::GetNodeEntity()
{
    return m_nodeEntity;
}

