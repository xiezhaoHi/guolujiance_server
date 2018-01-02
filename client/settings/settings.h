#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include <QString>


class CClientSetting
{
public:
    static bool EnableQss();

private:
    QString     ip;
    int         port;
    bool        login;
};



#endif//!_SETTINGS_H_