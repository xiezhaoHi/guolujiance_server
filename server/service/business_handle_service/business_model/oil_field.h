#ifndef _OIL_FIELD_H_
#define _OIL_FIELD_H_

#include <QList>
#include <QMutex>
#include "structs/structs.h"
#include "service/business_handle_service/business_model/rig.h"

class CRig;
class COilField;
typedef std::shared_ptr<COilField> oil_field_ptr;

// 油田
class COilField : public COilFieldInfo, public std::enable_shared_from_this<COilField>
{
public:

    COilField();

    ~COilField();

    // 根据ID获取指定的井队
    rig_ptr FindRigByID(const QString & id);

    // 存在指定ID的井队对象时，返回该对象；否则，创建指定ID的井队对象后返回
    rig_ptr GetOrCreateRig(const QString & id);

    // 获取油田下的所有井队
    void GetRigs(QList<rig_ptr> & lst);
private:
                                 // 油田信息
    QList<rig_ptr>      m_lstRig;
    QMutex            * m_pMutexForLstRig;
};

#endif//!_OIL_FIELD_H_