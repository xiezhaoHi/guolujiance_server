#ifndef _STACK_WALKER_H_
#define _STACK_WALKER_H_

#include <windows.h>

#include <map>
#include <vector>

#include <tchar.h>

#include <DbgHelp.h>
#include <tlhelp32.h>

#undef MODULEENTRY32
#undef PMODULEENTRY32
#undef LPMODULEENTRY32

class StackWalkerInterface
{
public:
    StackWalkerInterface();

    ~StackWalkerInterface();

    // SymInitialize()
    typedef BOOL (__stdcall *fpSymInitialize)(
        IN HANDLE hProcess,
        IN PSTR UserSearchPath, 
        IN BOOL fInvadeProcess
        );
    fpSymInitialize m_fpSymInitialize;

    // SymCleanup()
    typedef BOOL (__stdcall *fpSymCleanup)(
        IN HANDLE hProcess
        );
    fpSymCleanup m_fpSymCleanup;

    // StackWalk64()
    typedef BOOL (__stdcall *fpStackWalk64)(
        DWORD MachineType, 
        HANDLE hProcess,
        HANDLE hThread, 
        LPSTACKFRAME64 StackFrame, 
        PVOID ContextRecord,
        PREAD_PROCESS_MEMORY_ROUTINE64 ReadMemoryRoutine,
        PFUNCTION_TABLE_ACCESS_ROUTINE64 FunctionTableAccessRoutine,
        PGET_MODULE_BASE_ROUTINE64 GetModuleBaseRoutine,
        PTRANSLATE_ADDRESS_ROUTINE64 TranslateAddress
        );
    fpStackWalk64 m_fpStackWalk64;

    // SymSetContext()
    typedef BOOL (__stdcall *fpSymSetContext)(
        HANDLE hProcess,
        PIMAGEHLP_STACK_FRAME StackFrame,
        PIMAGEHLP_CONTEXT Context
        );
    fpSymSetContext m_fpSymSetContext;

    // SymEnumSymbols()
    typedef BOOL (__stdcall *fpSymEnumSymbols)(
        HANDLE hProcess,
        ULONG64 BaseOfDll,
        PCTSTR Mask,
        PSYM_ENUMERATESYMBOLS_CALLBACK EnumSymbolsCallback,
        PVOID UserContext
        );
    fpSymEnumSymbols m_fpSymEnumSymbols;

    // SymGetOptions()
    typedef DWORD (__stdcall *fpSymGetOptions)();
    fpSymGetOptions m_fpSymGetOptions;

    // SymSetOptions()
    typedef DWORD (__stdcall *fpSymSetOptions)(IN DWORD SymOptions);
    fpSymSetOptions m_fpSymSetOptions;

    // SymFunctionTableAccess64()
    typedef PVOID (__stdcall *fpSymFunctionTableAccess64)(
        HANDLE hProcess,
        DWORD64 AddrBase
        );
    fpSymFunctionTableAccess64 m_fpSymFunctionTableAccess64;

    // SymGetLineFromAddr64()
    typedef BOOL (__stdcall *fpSymGetLineFromAddr64)(
        IN HANDLE hProcess,
        IN DWORD64 dwAddr,
        OUT PDWORD pdwDisplacement,
        OUT PIMAGEHLP_LINE64 Line
        );
    fpSymGetLineFromAddr64 m_fpSymGetLineFromAddr64;

    // SymGetModuleBase64()
    typedef DWORD64 (__stdcall *fpSymGetModuleBase64)(
        IN HANDLE hProcess,
        IN DWORD64 dwAddr
        );
    fpSymGetModuleBase64 m_fpSymGetModuleBase64;

    // SymGetSymFromAddr64()
    typedef BOOL (__stdcall *fpSymGetSymFromAddr64)(
        IN HANDLE hProcess,
        IN DWORD64 dwAddr,
        OUT PDWORD64 pdwDisplacement,
        OUT PIMAGEHLP_SYMBOL64 Symbol );
    fpSymGetSymFromAddr64 m_fpSymGetSymFromAddr64;

    // UnDecorateSymbolName()
    typedef DWORD (__stdcall WINAPI *fpUnDecorateSymbolName)(
        PCSTR DecoratedName,
        PSTR UnDecoratedName,
        DWORD UndecoratedLength,
        DWORD Flags );
    fpUnDecorateSymbolName m_fpUnDecorateSymbolName;

    // SymLoadModule64()
    typedef DWORD64 (__stdcall *fpSymLoadModule64)(
        IN HANDLE hProcess,
        IN HANDLE hFile,
        IN PSTR ImageName,
        IN PSTR ModuleName,
        IN DWORD64 BaseOfDll,
        IN DWORD SizeOfDll
        );
    fpSymLoadModule64 m_fpSymLoadModule64;



    // CreateToolhelp32Snapshot()
    typedef HANDLE (__stdcall *fpCreateToolhelp32Snapshot)(
        DWORD dwFlags, 
        DWORD th32ProcessID
        );
    fpCreateToolhelp32Snapshot m_fpCreateToolhelp32Snapshot;

    // Module32First()
    typedef BOOL (__stdcall *fpModule32First)(
        HANDLE hSnapshot, 
        LPMODULEENTRY32 lpme
        );
    fpModule32First m_fpModule32First;

    // Module32Next()
    typedef BOOL (__stdcall *fpModule32Next)(
        HANDLE hSnapshot, 
        LPMODULEENTRY32 lpme
        );
    fpModule32Next m_fpModule32Next;

    // Thread32First()
    typedef BOOL (__stdcall *fpThread32First)(
        HANDLE hSnapshot, 
        LPTHREADENTRY32 lpme
        );
    fpThread32First m_fpThread32First;

    // Thread32Next()
    typedef BOOL (__stdcall *fpThread32Next)(
        HANDLE hSnapshot, 
        LPTHREADENTRY32 lpme
        );
    fpThread32Next m_fpThread32Next;

    bool Init();
    bool CanUse();

private:
#ifdef UNICODE
    HMODULE LoadDll(const WCHAR *pDllName);
#else
    HMODULE LoadDll(const char *pDllName);
#endif	
    FARPROC LoadAPI(HMODULE dllHandle, const char *pAPIName);

    HMODULE m_hDbhHelp;
    HMODULE m_hToolhelp;

    bool m_bCanUse;
};


const DWORD STACKWALK_MAX_NAMELEN = 1024;

class CStackWalker
{
public:
    CStackWalker(void);
    ~CStackWalker(void);

    BOOL GetCallstack(std::vector<std::string>& callStacks, DWORD dwThreadId);
    BOOL GetCallstack(std::vector<std::string>& callStacks);

private:
    StackWalkerInterface m_swi;

    HANDLE m_hProcess;
    DWORD m_dwProcessId;
    HANDLE m_hThread;
    DWORD m_dwThreadId;

    //当前堆栈信息
    char m_StackInfo[STACKWALK_MAX_NAMELEN];

    //模块基址-名称映射表
    std::map<DWORD64, std::string> m_ModuleList;
    BOOL GetModuleList();
};

extern CStackWalker g_StackWalker;

#ifdef WIN32
#include <Windows.h>
#include <DbgHelp.h>
LONG WINAPI H2SMonitorServerUnhandledExceptionFilter(
struct _EXCEPTION_POINTERS* ExceptionInfo
    );
#endif

#endif//!_STACK_WALKER_H_