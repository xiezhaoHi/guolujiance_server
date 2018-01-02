#include "registration.h"
#include <QSettings>

bool Regist(const QString & code)
{
    bool ret = false;
    QString diskID(getDiskInfo().serialNumber);
    diskID.trimmed();
    yxyDES2 des;
    des.InitializeKey(H2S_MONITOR_SERVER_PRODUCT_ID, 0);
    QByteArray ba = diskID.toLatin1();
    des.EncryptData(ba.data(), 0);
    QString code_(des.GetCiphertextInHex());

    if (ret = (code_ == code)) {
        WriteRegistrationCode(code);
    }

    return ret;
}

void ReadRigistrationCode(QString & code)
{
    QSettings config("h2smonitorserver.lic", QSettings::IniFormat);
    code = config.value("code", QStringLiteral("")).toString();
}

void WriteRegistrationCode(const QString & code)
{
    QSettings config("h2smonitorserver.lic", QSettings::IniFormat);
    config.setValue("code", code);
}
