#include "business_model.h"
#include <QMutexLocker>



CBusinessModel * CBusinessModel::m_pInstance = NULL;

CBusinessModel::CBusinessModel()
{
    m_pMutexForOilField = new QMutex(QMutex::Recursive);
}

CBusinessModel::~CBusinessModel()
{
    if (m_pMutexForOilField) {
        delete m_pMutexForOilField;
    }
}

CBusinessModel * CBusinessModel::GetInstance()
{
    if (!m_pInstance) {
        m_pInstance = new CBusinessModel();
    }
    return m_pInstance;
}

void CBusinessModel::DestroyInstance()
{
    if (m_pInstance) {
        delete m_pInstance;
        m_pInstance = NULL;
    }
}

oil_field_ptr CBusinessModel::FindOilFieldByID(const QString & id)
{
    QMutexLocker lock(m_pMutexForOilField);
    for (oil_field_ptr pOilField : m_lstOilField) {
        if (pOilField && pOilField->id.compare(id) == 0) {
            return pOilField;
        }
    }
    return NULL;
}

oil_field_ptr CBusinessModel::GetOrCreateOilField(const QString & id)
{
    QMutexLocker lock(m_pMutexForOilField);
    for (oil_field_ptr pOilField : m_lstOilField) {
        if (pOilField && pOilField->id.compare(id) == 0) {
            return pOilField;
        }
    }

    oil_field_ptr pOilField = std::make_shared<COilField>();
    pOilField->id = id;

    m_lstOilField.push_back(pOilField);

    return pOilField;
}

bool CBusinessModel::GetRigList(QList<rig_ptr> & lst)
{
    QMutexLocker lock(m_pMutexForOilField);

    for (oil_field_ptr ptr : m_lstOilField)
    {
        ptr->GetRigs(lst);
    }

    return true;
}
