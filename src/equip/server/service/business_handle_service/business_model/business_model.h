#ifndef _BUSINESS_MODEL_H_
#define _BUSINESS_MODEL_H_

#include <QString>
#include <QList>
#include <QMutex>
#include "oil_field.h"
#include "rig.h"
#include "device.h"
#include "user.h"

class COilField;
class CRig;

typedef std::shared_ptr<CRig> rig_ptr;

class CBusinessModel
{
public:

    static CBusinessModel * GetInstance();

    static void DestroyInstance();

    // ��ȡָ��ID���������
    oil_field_ptr FindOilFieldByID(const QString & id);

    // ����ָ��ID���������ʱ�����ظö��󣻷��򣬴���ָ��ID���������󷵻�
    oil_field_ptr GetOrCreateOilField(const QString & id);

    // ��ȡ���о����б�
    bool GetRigList(QList<rig_ptr> & lst);

private:

    CBusinessModel();

    ~CBusinessModel();

private:
    static CBusinessModel * m_pInstance;
    QList<oil_field_ptr>    m_lstOilField;
    QMutex                * m_pMutexForOilField;
};


#endif//!_BUSINESS_MODEL_H_