#ifndef _OIL_FIELD_H_
#define _OIL_FIELD_H_

#include <QList>
#include <QMutex>
#include "structs/structs.h"
#include "service/business_handle_service/business_model/rig.h"

class CRig;
class COilField;
typedef std::shared_ptr<COilField> oil_field_ptr;

// ����
class COilField : public COilFieldInfo, public std::enable_shared_from_this<COilField>
{
public:

    COilField();

    ~COilField();

    // ����ID��ȡָ���ľ���
    rig_ptr FindRigByID(const QString & id);

    // ����ָ��ID�ľ��Ӷ���ʱ�����ظö��󣻷��򣬴���ָ��ID�ľ��Ӷ���󷵻�
    rig_ptr GetOrCreateRig(const QString & id);

    // ��ȡ�����µ����о���
    void GetRigs(QList<rig_ptr> & lst);
private:
                                 // ������Ϣ
    QList<rig_ptr>      m_lstRig;
    QMutex            * m_pMutexForLstRig;
};

#endif//!_OIL_FIELD_H_