#ifndef _WINDOWS_SERVICE_ENTRY_H_

#include <windows.h>
#include <stdio.h>


#define SVCNAME TEXT("GASMonitorSvc")
//#define SVCNAME TEXT("H2SMonitorSvc")

VOID WINAPI SvcMain(DWORD dwArgc, LPTSTR *lpszArgv);
VOID WINAPI SvcCtrlHandler(DWORD);
VOID        ReportSvcStatus(DWORD, DWORD, DWORD);
VOID        SvcInit(DWORD, LPTSTR *);
void        LogEvent(LPCTSTR pFormat, ...);


#endif//_WINDOWS_SERVICE_ENTRY_H_