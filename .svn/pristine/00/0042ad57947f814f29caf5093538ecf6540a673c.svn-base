#include "windows_service_entry.h"
#include "service/service_hub.h"
#include "service/log_service/log_service.h"
#include "state_collector/path_collector.h"
#include "dbg/dbg_logger.h"



SERVICE_STATUS          g_SvcStatus;
SERVICE_STATUS_HANDLE   g_SvcStatusHandle;
HANDLE                  g_hSvcStopEvent = NULL;



VOID WINAPI SvcMain(DWORD dwArgc, LPTSTR *lpszArgv)
{
    // Register the handler function for the service.
    g_SvcStatusHandle = RegisterServiceCtrlHandler( 
        SVCNAME, 
        SvcCtrlHandler);

    if( !g_SvcStatusHandle )
    {
        return; 
    }

    // These SERVICE_STATUS members remain as set here.
    g_SvcStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS; 
    g_SvcStatus.dwServiceSpecificExitCode = 0;    

    // Report initial status to the SCM.
    ReportSvcStatus( SERVICE_START_PENDING, NO_ERROR, 3000 );
    
    // Perform service-specific initialization and work.
    SvcInit( dwArgc, lpszArgv );
}

VOID SvcInit(DWORD dwArgc, LPTSTR *lpszArgv)
{
    g_hSvcStopEvent = CreateEvent(
        NULL,    // default security attributes
        TRUE,    // manual reset event
        FALSE,   // not signaled
        NULL);   // no name

    if ( g_hSvcStopEvent == NULL)
    {
        ReportSvcStatus( SERVICE_STOPPED, NO_ERROR, 0 );
        return;
    }

    // Report running status when initialization is complete.
    ReportSvcStatus( SERVICE_RUNNING, NO_ERROR, 0 );



    CDbgLogger::LogMessageHandler(LEVEL_DEBUG, __FILE__, __LINE__, "ÉèÖÃ³ÌÐòÂ·¾¶");

    CServiceHub::GetInstance()->CreateServicesOnlyOnce();
    bool ret = CServiceHub::GetInstance()->StartServices();
    if (!ret) {
        LogEvent(TEXT("%s:%d"), __FILE__, __LINE__);
        CloseHandle(g_hSvcStopEvent);
        ReportSvcStatus( SERVICE_STOPPED, NO_ERROR, 0 );
        return;
    }

    // Perform work until service stops.
    while(1)
    {
        LogEvent(TEXT("%s:%d"), __FILE__, __LINE__);
        // Check whether to stop the service.
        WaitForSingleObject(g_hSvcStopEvent, INFINITE);
        LogEvent(TEXT("%s:%d"), __FILE__, __LINE__);
        CloseHandle(g_hSvcStopEvent);
        ReportSvcStatus( SERVICE_STOPPED, NO_ERROR, 0 );
        return;
    }
}

VOID WINAPI SvcCtrlHandler(DWORD dwControl)
{
    switch(dwControl)
    {
    case SERVICE_CONTROL_PAUSE:
        ReportSvcStatus(SERVICE_STOP_PENDING, NO_ERROR, 0);
        CServiceHub::GetInstance()->StopServices();
        ReportSvcStatus(SERVICE_PAUSED, NO_ERROR, 0);

        break;

    case SERVICE_CONTROL_STOP:
        ReportSvcStatus(SERVICE_STOP_PENDING, NO_ERROR, 0);
        CServiceHub::GetInstance()->StopServices();
        ReportSvcStatus(SERVICE_STOPPED, NO_ERROR, 0);
        SetEvent(g_hSvcStopEvent);
        break;

    case SERVICE_CONTROL_SHUTDOWN:
        ReportSvcStatus(SERVICE_STOP_PENDING, NO_ERROR, 0);
        CServiceHub::GetInstance()->StopServices();
        ReportSvcStatus(SERVICE_STOPPED, NO_ERROR, 0);
        SetEvent(g_hSvcStopEvent);
        break;

    case SERVICE_CONTROL_CONTINUE:
        {
            ReportSvcStatus(SERVICE_CONTINUE_PENDING, NO_ERROR, 0);
            bool ret = CServiceHub::GetInstance()->StartServices();
            if (!ret) {
                ReportSvcStatus( SERVICE_STOPPED, NO_ERROR, 0 );
            }
            else 
            {
                ReportSvcStatus(SERVICE_RUNNING, NO_ERROR, 0);
            }
        }
        break;

    default:
        break;
    }
}

VOID ReportSvcStatus(DWORD dwCurrentState,
                     DWORD dwWin32ExitCode,
                     DWORD dwWaitHint)
{
    static DWORD dwCheckPoint = 1;

    // Fill in the SERVICE_STATUS structure.
    g_SvcStatus.dwCurrentState = dwCurrentState;
    g_SvcStatus.dwWin32ExitCode = dwWin32ExitCode;
    g_SvcStatus.dwWaitHint = dwWaitHint;

    if (dwCurrentState == SERVICE_START_PENDING)
        g_SvcStatus.dwControlsAccepted = 0;
    else g_SvcStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;

    if ( (dwCurrentState == SERVICE_RUNNING) ||
        (dwCurrentState == SERVICE_STOPPED) )
        g_SvcStatus.dwCheckPoint = 0;
    else g_SvcStatus.dwCheckPoint = dwCheckPoint++;

    // Report the status of the service to the SCM.
    SetServiceStatus( g_SvcStatusHandle, &g_SvcStatus );
}

void LogEvent(LPCTSTR pFormat, ...)
{  
    TCHAR    chMsg[256];  
    HANDLE  hEventSource;  
    LPTSTR  lpszStrings[1];  
    va_list pArg;  

    va_start(pArg, pFormat);  
    vswprintf(chMsg, pFormat, pArg);  
    va_end(pArg);  

    lpszStrings[0] = chMsg;  

    hEventSource = RegisterEventSource(NULL, SVCNAME);  
    if (hEventSource != NULL)  
    {  
        ReportEvent(hEventSource, EVENTLOG_INFORMATION_TYPE, 0, 0, NULL, 1, 0, (LPCTSTR*) &lpszStrings[0], NULL);  
        DeregisterEventSource(hEventSource);  
    }  
}  