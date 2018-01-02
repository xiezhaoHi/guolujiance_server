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



// ���԰汾�ĳ��򣬲���Ҫ����ע����
#ifdef NEED_LICENSE
// δע��ʱһֱ��ʾ�Ի���ֱ���û�������ȷ��ע���룬���߹ر�ע�ᴰ��
// ע����֤δͨ��ʱ������retVal
#define IS_REGISTED(retFailedVal) {\
    if (IS_REGISTED_()) {\
    } else {\
    CFormRegistration form;\
    form.ShowRegistDialog(QString(getDiskInfo().serialNumber).trimmed());\
    if (IS_REGISTED_()) {\
        LOG_INFO() << QStringLiteral("ע������֤�ɹ�");\
    } else {\
        LOG_WARING() << QStringLiteral("ע������֤ʧ��");\
    return retFailedVal;\
    }\
    }\
}
#else // 
#define IS_REGISTED(retFailedVal) ;
#endif//!NEED_LICENSE


// δ��֤ʱ����������ע����ĶԻ���ע�������ʱ����ʧ�ܣ����򷵻سɹ�
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


// ע��
bool Regist(const QString & code);

// ��ȡע����
void ReadRigistrationCode(QString & code);

// ��¼ע����
void WriteRegistrationCode(const QString & code);


#endif//!_H2S_MONITOR_SERVER_GENUINE_VERIFY_H_