#include "form_devices_container.h"
#include <QFile>
#include <QString>
#include <QGridLayout>
#include <QPushButton>
#include "device_tree_model.h"
#include "QPalette"
#include "service/business_handle_service/business_model/business_model.h"
#include "service/db_service/db_service.h"
#include "service/log_service/log_service.h"


CFormDevicesContainer::CFormDevicesContainer(QWidget *parent)
    : QWidget(parent)
{
    BuildUI();
    BindSlots();
}

CFormDevicesContainer::~CFormDevicesContainer()
{

}

void CFormDevicesContainer::BuildUI()
{
    QGridLayout * pLayoutSelf = new QGridLayout(this);

    m_pTreeDeviceModel = new CDeviceTreeModel(this);
    m_pTreeDevice = new CDeviceTree(this);
    m_pTreeDevice->setModel(m_pTreeDeviceModel);
    pLayoutSelf->addWidget(m_pTreeDevice, 0, 0, 1, 1);
    pLayoutSelf->setRowStretch(0, 1);

    this->setLayout(pLayoutSelf);

    BeautifyUI();
    TranslateUI();
}

void CFormDevicesContainer::BeautifyUI()
{
    this->setObjectName(QStringLiteral("formdevicescontainer"));

    QFile qssFile(":/qss/form_devices_container.qss");
    if (qssFile.open(QFile::ReadOnly)) {
        QString stylesheet = QLatin1String(qssFile.readAll());
        this->setStyleSheet(stylesheet);
        qssFile.close();
    }
}

void CFormDevicesContainer::TranslateUI()
{
    // do nothing
}

void CFormDevicesContainer::BindSlots()
{
    connect(this, SIGNAL(InitDeviceTreeNodes()), this, SLOT(BuildTreeNodes()));
}

void CFormDevicesContainer::BuildTreeNodes()
{
    base_entity_ptr rootEntity = std::make_shared<CBaseEntity>();
    rootEntity->id = QStringLiteral("H2S含量监控系统");
    rootEntity->name = QStringLiteral("H2S含量监控系统");
    rootEntity->type = ENTITY_ROOT;

    CDeviceTreeNode * root = new CDeviceTreeNode(rootEntity, this);

    QList<CBaseEntity> lst;
    CDBService::GetInstance()->GetOilFieldList(lst);

    for (CBaseEntity entity : lst)
    {
        oil_field_ptr pOilField = CBusinessModel::GetInstance()->GetOrCreateOilField(entity.id);
        pOilField->CopyFrom(entity);
        pOilField->type = ENTITY_OIL_FIELD;

        CDeviceTreeNode * pOilFieldNode = new CDeviceTreeNode(pOilField, root);
        root->AppendChild(pOilFieldNode);

        QList<CBaseEntity> lstRig;
        CDBService::GetInstance()->GetRigListByOilFieldID(entity.id, lstRig);

        for (CBaseEntity rigEntity : lstRig)
        {
            rig_ptr pRig = pOilField->GetOrCreateRig(rigEntity.id);
            pRig->CopyFrom(rigEntity);
            pRig->type = ENTITY_RIG;

            CDeviceTreeNode * pRigNode = new CDeviceTreeNode(pRig, pOilFieldNode);
            pOilFieldNode->AppendChild(pRigNode);

            QList<CDeviceInfo> lstDevice;
            CDBService::GetInstance()->GetDeviceListByRigID(rigEntity.id, lstDevice);
            for (CDeviceInfo deviceEntity : lstDevice)
            {
                device_ptr pDevice = pRig->GetOrCreateDevice(deviceEntity.id);
                pDevice->CopyFrom(deviceEntity);
                pDevice->type = ENTITY_DEVICE;

                CDeviceTreeNode * pDeviceNode = new CDeviceTreeNode(pDevice, pRigNode);
                pRigNode->AppendChild(pDeviceNode);
            }
        }
    }

    m_pTreeDevice->reset();
    m_pTreeDeviceModel->SetupData(root);
    m_pTreeDevice->repaint();
}


