#ifndef _H2S_MONITOR_SERVER_GENUINE_VERIFY_H_
#define _H2S_MONITOR_SERVER_GENUINE_VERIFY_H_

#include "diskid32.h"
#include "yxyDES2.h"
#include "version/version.h"
#include "ui/form_registration.h"
#include "service/log_service/log_service.h"
#include <QString>
#include <QByteArray>


#ifndef _DEBUG
//#define NEED_LICENSE
#endif // !_DEBUG



// 调试版本的程序，不需要输入注册码
#ifdef NEED_LICENSE
// 未注册时一直显示对话框，直到用户输入正确的注册码，或者关闭注册窗口
// 注册验证未通过时，返回retVal
#define IS_REGISTED(retFailedVal) {\
    if (IS_REGISTED_()) {\
    } else {\
    CFormRegistration form;\
    form.ShowRegistDialog(QString(getDiskInfo().serialNumber).trimmed());\
    if (IS_REGISTED_()) {\
        LOG_INFO() << QStringLiteral("注册码验证成功");\
    } else {\
        LOG_WARING() << QStringLiteral("注册码验证失败");\
    return retFailedVal;\
    }\
    }\
}
#else // 
#define IS_REGISTED(retFailedVal) ;
#endif//!NEED_LICENSE


// 未验证时，弹出输入注册码的对话框，注册码错误时返回失败，否则返回成功
#define IS_REGISTED_ [](){\
    QString iniCode;\
    ReadRigistrationCode(iniCode);\
    QString diskID(getDiskInfo().serialNumber);\
    diskID.trimmed();\
    yxyDES2 des;\
    des.InitializeKey(H2S_MONITOR_SERVER_PRODUCT_ID, 0);\
    QByteArray ba = diskID.toLatin1();\
    des.EncryptData(ba.data(), 0);\
    QString code(des.GetCiphertextInHex());\
    return (iniCode == code);\
}\


// 注册
bool Regist(const QString & code);

// 读取注册码
void ReadRigistrationCode(QString & code);

// 记录注册码
void WriteRegistrationCode(const QString & code);


#endif//!_H2S_MONITOR_SERVER_GENUINE_VERIFY_H_