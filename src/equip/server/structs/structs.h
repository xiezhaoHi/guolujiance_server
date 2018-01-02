#ifndef _STRUCTS_H_
#define _STRUCTS_H_

#include <memory>
#include <QString>
#include <QObject>
#include "service/socket_service/tcp_session.h"


typedef enum _E_ENTITY_TYPE
{
    ENTITY_UNKNOW = 0,
    ENTITY_PENDING = 1,// ���� �������ڽ�����
    ENTITY_ROOT = 2,   // �����
    ENTITY_OIL_FIELD,  // ����
    ENTITY_RIG,        // ����
    ENTITY_USER,       // �û�
    ENTITY_DEVICE      // �豸
}E_ENTITY_TYPE;

class CBaseEntity;
typedef std::shared_ptr<CBaseEntity> base_entity_ptr;

class CBaseEntity : public QObject
{
    Q_OBJECT

public:
    CBaseEntity() {
        this->type = ENTITY_UNKNOW;
    } 

    CBaseEntity(const CBaseEntity & val)
    {
        this->id = val.id;
        this->name = val.name;
        this->type = val.type;
    }

    virtual ~CBaseEntity() {}

    CBaseEntity & operator= (const CBaseEntity & val)
    {
        this->id = val.id;
        this->name = val.name;
        this->type = val.type;
        return *this;
    }

    void CopyFrom(const CBaseEntity & val)
    {
        this->id = val.id;
        this->name = val.name;
        this->type = val.type;
    }

    virtual void SetSession(tcp_session_ptr session) {};

    virtual QString GetDisplayText() 
    {
        return QStringLiteral("type:%3 id:%1 name:%2").arg(type).arg(id).arg(name);
    }

signals:
    void entityInfoUpdated();

public:
    QString         id;
    QString         name;
    E_ENTITY_TYPE   type;// ʵ������
};

Q_DECLARE_METATYPE(CBaseEntity)
Q_DECLARE_METATYPE(base_entity_ptr)

// ������Ϣ
class COilFieldInfo : public CBaseEntity
{
    // TODO
};


// ������Ϣ
class CRigInfo : public CBaseEntity
{
    // TODO

};

typedef enum _E_DEVICE_TYPE
{
    DEVICE_TYPE_UNKNOWN = 0,           // δ֪�����豸
    DEVICE_TYPE_PORTABLE_H2S_DETECTOR, // ��ЯʽH2S�����
    DEVICE_TYPE_STATIC_H2S_DETECTOR,   // �̶�ʽH2S�����
}E_DEVICE_TYPE;

// �豸��Ϣ
class CDeviceInfo : public CBaseEntity
{
public:
    void CopyFrom(CDeviceInfo & val)
    {
        CBaseEntity::CopyFrom(val);
        this->code = val.code;
        this->type = ENTITY_DEVICE;
    }

public:
    QString         code;       // �豸����
    E_DEVICE_TYPE   deviceType; // �豸����
    QString         warningLine1st;// һ������ֵ
    QString         warningLine2nd;// ��������ֵ
    QString         maxRange;      // ���ֵ
    QString         sampleRate;    // ������
};

typedef std::shared_ptr<CDeviceInfo> device_info_ptr;

// ��Ա��Ϣ
class CUserInfo : public CBaseEntity
{
    CDeviceInfo   deviceInfo;

    // TODO
};

#endif//!_STRUCTS_H_