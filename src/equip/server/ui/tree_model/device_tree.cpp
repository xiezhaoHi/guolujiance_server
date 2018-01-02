#include "device_tree.h"
#include <QAbstractItemView>
#include "tree_model/device_tree_node.h"
#include "service/business_handle_service/business_model/oil_field.h"


CDeviceTree::CDeviceTree(QWidget *parent)
    : QTreeView(parent)
{

}

CDeviceTree::~CDeviceTree()
{

}

void CDeviceTree::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    QAbstractItemView::currentChanged(current, previous);

    CDeviceTreeNode * pNode = static_cast<CDeviceTreeNode *>(current.internalPointer());
    if (pNode && pNode->GetNodeEntity()) {
        QList<rig_ptr> lst;
        base_entity_ptr pEntity = pNode->GetNodeEntity();
        if (pEntity->type == ENTITY_OIL_FIELD) {
            oil_field_ptr pOildField = std::static_pointer_cast<COilField>(pEntity);
            if (pOildField) {
                pOildField->GetRigs(lst);
            }
        } else if (pNode->GetNodeEntity()->type == ENTITY_RIG) {
            rig_ptr pRig = std::static_pointer_cast<CRig>(pEntity);
            if (pRig) {
                lst.append(pRig);
            }
        }

        emit SelectedRigsChanged(lst);
    }
}
