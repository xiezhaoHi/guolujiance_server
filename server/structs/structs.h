#ifndef _STRUCTS_H_
#define _STRUCTS_H_

#include <memory>
#include <QString>
#include <QObject>
#include "service/socket_service/tcp_session.h"


typedef enum _E_ENTITY_TYPE
{
    ENTITY_UNKNOW = 0,
    ENTITY_PENDING = 1,// 代表 操作正在进行中
    ENTITY_ROOT = 2,   // 根结点
    ENTITY_OIL_FIELD,  // 油田
    ENTITY_RIG,        // 井队
    ENTITY_USER,       // 用户
    ENTITY_DEVICE      // 设备
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
    E_ENTITY_TYPE   type;// 实体类型
};

Q_DECLARE_METATYPE(CBaseEntity)
Q_DECLARE_METATYPE(base_entity_ptr)

// 油田信息
class COilFieldInfo : public CBaseEntity
{
    // TODO
};


// 井队信息
class CRigInfo : public CBaseEntity
{
    // TODO

};

typedef enum _E_DEVICE_TYPE
{
    DEVICE_TYPE_UNKNOWN = 0,           // 未知类型设备
    DEVICE_TYPE_PORTABLE_H2S_DETECTOR, // 便携式H2S检测仪
    DEVICE_TYPE_STATIC_H2S_DETECTOR,   // 固定式H2S检测仪
}E_DEVICE_TYPE;

// 设备信息
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
    QString         code;       // 设备编码
    E_DEVICE_TYPE   deviceType; // 设备类型
    QString         warningLine1st;// 一级报警值
    QString         warningLine2nd;// 二级报警值
    QString         maxRange;      // 最大值
    QString         sampleRate;    // 采样率
};

typedef std::shared_ptr<CDeviceInfo> device_info_ptr;

// 人员信息
class CUserInfo : public CBaseEntity
{
    CDeviceInfo   deviceInfo;

    // TODO
};

#endif//!_STRUCTS_H_