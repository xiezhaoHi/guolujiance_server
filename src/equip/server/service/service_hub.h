#ifndef _SERVICE_HUB_H_
#define _SERVICE_HUB_H_

#include "dbg/dbg_logger.h"

typedef enum _E_SERVICE_START_ORDER
{
    NO_SERVICE = 0,
    LOG_SERVICE,
    CONFIGURATION_SERVICE,
    DB_SERVICE,
    TASK_SERVICE,
    BUSINESS_HANDLE_SERVICE,
    SOCKET_SERVICE,
}E_SERVICE_START_ORDER;

class CServiceHub
{
public:

    static CServiceHub * GetInstance();

    static void DestroyInstance();

    void CreateServicesOnlyOnce();

    void DestroyServicesForEver();

    bool StartServices();

    void StopServices();

    bool IsServicesRunning();

private:

    CServiceHub();

    ~CServiceHub();

private:
    bool                 m_bIsServicesRunning;
    static CServiceHub * m_pInstance;
};

#endif//!_SERVICE_HUB_H_