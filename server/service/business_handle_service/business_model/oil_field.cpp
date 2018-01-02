#include "oil_field.h"
#include "rig.h"
#include <QMutexLocker>



COilField::COilField()
{
    type = ENTITY_OIL_FIELD;
    m_pMutexForLstRig = new QMutex();
}

COilField::~COilField()
{
    delete m_pMutexForLstRig;
}

rig_ptr COilField::FindRigByID(const QString & id)
{
    QMutexLocker lock(m_pMutexForLstRig);
    for (rig_ptr pRig : m_lstRig) {
        if (pRig && pRig->id.compare(id) == 0) {
            return pRig;
        }
    }
    return NULL;
}

rig_ptr COilField::GetOrCreateRig(const QString & id)
{
    QMutexLocker lock(m_pMutexForLstRig);
    for (rig_ptr pRig : m_lstRig) {
        if (pRig && pRig->id.compare(id) == 0) {
            return pRig;
        }
    }

    rig_ptr pRig = std::make_shared<CRig>();
    pRig->id = id;
    pRig->SetOilField(this->shared_from_this());

    m_lstRig.push_back(pRig);

    return pRig;
}

void COilField::GetRigs(QList<rig_ptr> & lst)
{
    QMutexLocker lock(m_pMutexForLstRig);
    lst.append(m_lstRig);
}
