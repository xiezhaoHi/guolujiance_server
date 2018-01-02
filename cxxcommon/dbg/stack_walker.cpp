#include "stack_walker.h"


#ifdef WIN32 
#include "state_collector/path_collector.h"
#include "service/log_service/log_service.h"
#include <QDir>

#pragma comment(lib, "Dbghelp.lib")

LONG WINAPI H2SMonitorServerUnhandledExceptionFilter(
    __in          struct _EXCEPTION_POINTERS* ExceptionInfo
    )
{
    // 创建dmp目录
    QString strDmpDirName(CPathCollector::m_strAppDirPath + QStringLiteral("./dmp"));
    QDir dmpDir(strDmpDirName);
    if (!dmpDir.exists()) {
        if (!dmpDir.mkdir(strDmpDirName)) {
            LOG_WARING() << QStringLiteral("创建dmp目录失败,error:%1")
                .arg(GetLastError());
            return EXCEPTION_CONTINUE_SEARCH;
        }
    }

    wchar_t dmpFileName[MAX_PATH] = {0};
    SYSTEMTIME st;
    GetLocalTime(&st);
    swprintf(dmpFileName, L"dmp/%04d%02d%02d_%02d%02d%02d_%03d.dmp",
        st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

    // 创建dmp文件
    HANDLE hDmpFile = CreateFile(dmpFileName, FILE_ALL_ACCESS, 
        0, NULL, CREATE_ALWAYS, 0, NULL);
    if (INVALID_HANDLE_VALUE == hDmpFile) {
        return EXCEPTION_CONTINUE_SEARCH;
    } else {
        MINIDUMP_EXCEPTION_INFORMATION eInfo;
        eInfo.ThreadId = GetCurrentThreadId();
        eInfo.ExceptionPointers = ExceptionInfo;
        eInfo.ClientPointers = FALSE;

        BOOL ret = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),
            hDmpFile,  MiniDumpNormal, &eInfo, NULL, NULL);
        if (!ret) {
            LOG_WARING() << QStringLiteral("写dmp文件失败,error:%1")
                .arg(GetLastError());
        } else {
            LOG_WARING() << QStringLiteral("写dmp文件[%1]完成")
                .arg(QString().toWCharArray(dmpFileName));
        }
    }
    CloseHandle(hDmpFile); 
    return EXCEPTION_CONTINUE_SEARCH;
}

#endif // WIN32


StackWalkerInterface::StackWalkerInterface()
{
    m_hDbhHelp = NULL;
    m_hToolhelp = NULL;

    m_fpCreateToolhelp32Snapshot = NULL;
    m_fpModule32First = NULL;
    m_fpModule32Next = NULL;
    m_fpThread32First = NULL;
    m_fpThread32Next = NULL;

    m_fpSymInitialize = NULL;
    m_fpSymCleanup = NULL;

    m_fpStackWalk64 = NULL;
    m_fpSymSetContext = NULL;
    m_fpSymEnumSymbols = NULL;

    m_fpSymGetOptions = NULL;
    m_fpSymSetOptions = NULL;

    m_fpSymGetModuleBase64 = NULL;

    m_fpSymFunctionTableAccess64 = NULL;

    m_fpSymGetLineFromAddr64 = NULL;

    m_fpSymGetSymFromAddr64 = NULL;

    m_fpSymLoadModule64 = NULL;

    m_fpUnDecorateSymbolName = NULL;

    m_bCanUse = false;
}

StackWalkerInterface::~StackWalkerInterface()
{
    if (m_hDbhHelp != NULL)
    {
        FreeLibrary(m_hDbhHelp);
        m_hDbhHelp = NULL;
    }
    if (m_hToolhelp != NULL)
    {
        FreeLibrary(m_hToolhelp);
        m_hToolhelp = NULL;
    }
}

#ifdef UNICODE
HMODULE StackWalkerInterface::LoadDll(const WCHAR *pDllName)
#else
HMODULE StackWalkerInterface::LoadDll(const char *pDllName)
#endif
{
    HMODULE dllHandle = LoadLibrary(pDllName);
    if (NULL != dllHandle)
    {
        return dllHandle;
    }
    else
    {
        printf("StackWalkerInterface LoadLibrary \"%s\" Error code 0x%08x!\n", pDllName, GetLastError());
        return NULL;
    }
}

FARPROC StackWalkerInterface::LoadAPI(HMODULE dllHandle, const char *pAPIName)
{
    FARPROC apiAddr = GetProcAddress(dllHandle, pAPIName);
    if (NULL != apiAddr)
    {
        return apiAddr;
    }
    else
    {
        printf("StackWalkerInterface GetProcAddress \"%s\" Error code 0x%08x!\n", pAPIName, GetLastError());
        return NULL;
    }
}

bool StackWalkerInterface::Init()
{
    bool res = false;

    // 加载dbghelp.dll >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    m_hDbhHelp = LoadDll(_T("dbghelp.dll"));
    if (NULL == m_hDbhHelp)
    {
        return false;
    }

    m_fpSymInitialize = (fpSymInitialize) LoadAPI(m_hDbhHelp, "SymInitialize" );
    if (NULL == m_fpSymInitialize)
    {
        return false;
    }

    m_fpSymCleanup = (fpSymCleanup) LoadAPI(m_hDbhHelp, "SymCleanup" );
    if (NULL == m_fpSymCleanup)
    {
        return false;
    }

    m_fpStackWalk64 = (fpStackWalk64) LoadAPI(m_hDbhHelp, "StackWalk64" );
    if (NULL == m_fpStackWalk64)
    {
        return false;
    }

    m_fpSymSetContext = (fpSymSetContext) LoadAPI(m_hDbhHelp, "SymSetContext" );
    if (NULL == m_fpSymSetContext)
    {
        return false;
    }

    m_fpSymEnumSymbols = (fpSymEnumSymbols) LoadAPI(m_hDbhHelp, "SymEnumSymbols" );
    if (NULL == m_fpSymEnumSymbols)
    {
        return false;
    }

    m_fpSymGetOptions = (fpSymGetOptions) LoadAPI(m_hDbhHelp, "SymGetOptions" );
    if (NULL == m_fpSymGetOptions)
    {
        return false;
    }

    m_fpSymSetOptions = (fpSymSetOptions) LoadAPI(m_hDbhHelp, "SymSetOptions" );
    if (NULL == m_fpSymSetOptions)
    {
        return false;
    }

    m_fpSymFunctionTableAccess64 = (fpSymFunctionTableAccess64) LoadAPI(m_hDbhHelp, "SymFunctionTableAccess64" );
    if (NULL == m_fpSymFunctionTableAccess64)
    {
        return false;
    }

    m_fpSymGetLineFromAddr64 = (fpSymGetLineFromAddr64) LoadAPI(m_hDbhHelp, "SymGetLineFromAddr64" );
    if (NULL == m_fpSymGetLineFromAddr64)
    {
        return false;
    }

    m_fpSymGetModuleBase64 = (fpSymGetModuleBase64) LoadAPI(m_hDbhHelp, "SymGetModuleBase64" );
    if (NULL == m_fpSymGetModuleBase64)
    {
        return false;
    }

    m_fpSymGetSymFromAddr64 = (fpSymGetSymFromAddr64) LoadAPI(m_hDbhHelp, "SymGetSymFromAddr64" );
    if (NULL == m_fpSymGetSymFromAddr64)
    {
        return false;
    }

    m_fpUnDecorateSymbolName = (fpUnDecorateSymbolName) LoadAPI(m_hDbhHelp, "UnDecorateSymbolName" );
    if (NULL == m_fpUnDecorateSymbolName)
    {
        return false;
    }

    m_fpSymLoadModule64 = (fpSymLoadModule64) LoadAPI(m_hDbhHelp, "SymLoadModule64" );
    if (NULL == m_fpSymLoadModule64)
    {
        return false;
    }

    // 加载kernel32.dll >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    m_hToolhelp = LoadDll(_T("kernel32.dll"));
    if (NULL == m_hToolhelp)
    {
        return false;
    }

    m_fpCreateToolhelp32Snapshot = (fpCreateToolhelp32Snapshot)LoadAPI(m_hToolhelp, "CreateToolhelp32Snapshot");
    if (NULL == m_fpCreateToolhelp32Snapshot)
    {
        return false;
    }

    m_fpModule32First = (fpModule32First) LoadAPI(m_hToolhelp, "Module32First");
    if (NULL == m_fpModule32First)
    {
        return false;
    }

    m_fpModule32Next = (fpModule32Next) LoadAPI(m_hToolhelp, "Module32Next");
    if (NULL == m_fpModule32Next)
    {
        return false;
    }

    m_fpThread32First = (fpThread32First) LoadAPI(m_hToolhelp, "Thread32First");
    if (NULL == m_fpThread32First)
    {
        return false;
    }

    m_fpThread32Next = (fpThread32Next) LoadAPI(m_hToolhelp, "Thread32Next");
    if (NULL == m_fpThread32Next)
    {
        return false;
    }

    m_bCanUse = true;
    return true;
}

bool StackWalkerInterface::CanUse()
{
    return m_bCanUse;
}

CStackWalker::CStackWalker(void)
{
#ifdef WIN32
    // 记录程序异常
    SetUnhandledExceptionFilter(H2SMonitorServerUnhandledExceptionFilter);
#endif

    m_hProcess = GetCurrentProcess();
    m_dwProcessId = GetCurrentProcessId();

    BOOL res = m_swi.Init();
    if (!res)
    {
        printf("StackWalkerInterface Init() Error!\n");
        return;
    }

    res = m_swi.m_fpSymInitialize(m_hProcess, NULL, TRUE);
    if (!res)
    {
        printf("SymInitialize() Error code 0x%08x!\n", GetLastError());
        return;
    }

    DWORD symOptions = m_swi.m_fpSymGetOptions();
    symOptions |= SYMOPT_LOAD_LINES;
    symOptions |= SYMOPT_FAIL_CRITICAL_ERRORS;
    symOptions = m_swi.m_fpSymSetOptions(symOptions);

    GetModuleList();
}

CStackWalker::~CStackWalker(void)
{
    m_swi.m_fpSymCleanup(m_hProcess);
    m_ModuleList.clear();
}

BOOL CStackWalker::GetModuleList()
{
    m_ModuleList.clear();

    HANDLE hSnap = INVALID_HANDLE_VALUE;
    hSnap = m_swi.m_fpCreateToolhelp32Snapshot(TH32CS_SNAPMODULE, m_dwProcessId);
    if (hSnap == INVALID_HANDLE_VALUE)
    {
        printf("CreateToolhelp32Snapshot() Error code 0x%08x!\n", GetLastError());
        return FALSE;
    }

    MODULEENTRY32 me;
    memset(&me, 0, sizeof(me));
    me.dwSize = sizeof(me);

    BOOL res = FALSE;
    int cnt = 0;
    res = m_swi.m_fpModule32First(hSnap, &me);
    while (res)
    {
        m_swi.m_fpSymLoadModule64(m_hProcess, 0, me.szExePath, me.szModule, 
            (DWORD64) me.modBaseAddr, me.modBaseSize);

        std::string name = me.szModule;
        m_ModuleList[(DWORD64)me.modBaseAddr] = name;

        cnt++;
        res = m_swi.m_fpModule32Next( hSnap, &me );
    }

    CloseHandle(hSnap);

    if (cnt > 0)
    {
        res = TRUE;
    }

    return res;
}

BOOL CStackWalker::GetCallstack(std::vector<std::string>& callStacks, DWORD dwThreadId)
{
    if (!m_swi.CanUse())
    {
        return FALSE;
    }

    HANDLE hSnap = m_swi.m_fpCreateToolhelp32Snapshot(TH32CS_SNAPALL, m_dwProcessId);
    if (hSnap == INVALID_HANDLE_VALUE)
    {
        printf("CreateToolhelp32Snapshot() Error code 0x%08x!\n", GetLastError());
        return FALSE;
    }

    HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, dwThreadId);
    DWORD hCurrThreadId = ::GetCurrentThreadId();
    if (hThread != NULL)
    {
        CONTEXT context;
        if (dwThreadId != hCurrThreadId)
        {
            SuspendThread(hThread);
            memset(&context, 0, sizeof(CONTEXT));
            context.ContextFlags = CONTEXT_FULL;
            if (!GetThreadContext(hThread, &context))
            {
                ResumeThread(hThread);
                return FALSE;
            }
        }
        else
        {
            memset(&context, 0, sizeof(CONTEXT));
            context.ContextFlags = CONTEXT_FULL;
            __asm    call x 
            __asm x: pop eax 
            __asm    mov context.Eip, eax 
            __asm    mov context.Ebp, ebp 
            __asm    mov context.Esp, esp 
        }

        STACKFRAME64 stackframe;
        memset(&stackframe, 0, sizeof(stackframe));
        DWORD imageType;

#ifdef _M_IX86
        imageType = IMAGE_FILE_MACHINE_I386;
        stackframe.AddrPC.Offset = context.Eip;
        stackframe.AddrPC.Mode = AddrModeFlat;
        stackframe.AddrFrame.Offset = context.Ebp;
        stackframe.AddrFrame.Mode = AddrModeFlat;
        stackframe.AddrStack.Offset = context.Esp;
        stackframe.AddrStack.Mode = AddrModeFlat;
#elif _M_X64
        imageType = IMAGE_FILE_MACHINE_AMD64;
        stackframe.AddrPC.Offset = context.Rip;
        stackframe.AddrPC.Mode = AddrModeFlat;
        stackframe.AddrFrame.Offset = context.Rsp;
        stackframe.AddrFrame.Mode = AddrModeFlat;
        stackframe.AddrStack.Offset = context.Rsp;
        stackframe.AddrStack.Mode = AddrModeFlat;
#else
#error "Not Supported!"
#endif

        char sysbuff[sizeof(IMAGEHLP_SYMBOL64) + STACKWALK_MAX_NAMELEN];
        IMAGEHLP_SYMBOL64 *pSym = (IMAGEHLP_SYMBOL64 *)sysbuff;
        memset(pSym, 0x00, sizeof(IMAGEHLP_SYMBOL64) + STACKWALK_MAX_NAMELEN);
        pSym->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL64);
        pSym->MaxNameLength = STACKWALK_MAX_NAMELEN;

        IMAGEHLP_LINE64 line;
        memset(&line, 0x00, sizeof(line));
        line.SizeOfStruct = sizeof(line);

        while (true)
        {
            if (!m_swi.m_fpStackWalk64(imageType, m_hProcess, hThread, &stackframe, &context, 
                NULL, m_swi.m_fpSymFunctionTableAccess64, m_swi.m_fpSymGetModuleBase64, NULL) )
            {
                printf("StackWalk64() Error code 0x%08x!\n", GetLastError());
                return FALSE;
            }

            if (stackframe.AddrPC.Offset == stackframe.AddrReturn.Offset)
            {
                break;
            }

            if (stackframe.AddrPC.Offset != 0)
            {
                memset(m_StackInfo, 0x00, sizeof(char) * STACKWALK_MAX_NAMELEN);

                //函数对应的模块名 查m_ModuleList表
                DWORD64 moduleBase = m_swi.m_fpSymGetModuleBase64(m_hProcess, stackframe.AddrPC.Offset);
                std::map<DWORD64, std::string>::iterator module = m_ModuleList.find(moduleBase);
                if (module != m_ModuleList.end())
                {
                    const char *name = module->second.c_str();
                    strcat_s(m_StackInfo, STACKWALK_MAX_NAMELEN, name);
                }
                else
                {
                    strcat_s(m_StackInfo, STACKWALK_MAX_NAMELEN, "unknow module");
                }
                strcat_s(m_StackInfo, STACKWALK_MAX_NAMELEN, ": ");

                //函数对应的代码文件 以及行号
                DWORD dwDisplacementFromLine;
                if (m_swi.m_fpSymGetLineFromAddr64(m_hProcess, stackframe.AddrPC.Offset, &dwDisplacementFromLine, &line))
                {
                    char name[50] = {0};
                    _itoa_s(line.LineNumber, name, 50, 10);
                    strcat_s(m_StackInfo, STACKWALK_MAX_NAMELEN, line.FileName);
                    strcat_s(m_StackInfo, STACKWALK_MAX_NAMELEN, "(");
                    strcat_s(m_StackInfo, STACKWALK_MAX_NAMELEN, name);
                    strcat_s(m_StackInfo, STACKWALK_MAX_NAMELEN, ")");
                }
                else
                {
                    strcat_s(m_StackInfo, STACKWALK_MAX_NAMELEN, "unknow file");
                }
                strcat_s(m_StackInfo, STACKWALK_MAX_NAMELEN, ": ");

                //函数名
                DWORD64 dwDisplacementFromFunc;
                if (m_swi.m_fpSymGetSymFromAddr64(m_hProcess, stackframe.AddrPC.Offset, &dwDisplacementFromFunc, pSym))
                {
                    if(pSym->Name[0] == '?')
                    {
                        //修饰名 -> 函数签名
                        char name[STACKWALK_MAX_NAMELEN] = {0};
                        m_swi.m_fpUnDecorateSymbolName(pSym->Name, name, STACKWALK_MAX_NAMELEN, UNDNAME_NAME_ONLY);
                        strcat_s(m_StackInfo, STACKWALK_MAX_NAMELEN, name);
                    }
                    else
                    {
                        strcat_s(m_StackInfo, STACKWALK_MAX_NAMELEN, pSym->Name);
                    }
                }
                else
                {
                    strcat_s(m_StackInfo, STACKWALK_MAX_NAMELEN, "unknow func");
                }
                strcat_s(m_StackInfo, STACKWALK_MAX_NAMELEN, ": ");

                std::string oneCallStack = m_StackInfo;
                callStacks.push_back(oneCallStack);
            }

            if (stackframe.AddrReturn.Offset == 0)
            {
                break;
            }
        }

        if (dwThreadId != hCurrThreadId)
        {
            ResumeThread(hThread);
        }
        CloseHandle(hThread);

        callStacks.push_back("\n");
    }

    CloseHandle(hSnap);
    return TRUE;
}

BOOL CStackWalker::GetCallstack(std::vector<std::string>& callStacks)
{
    if (!m_swi.CanUse())
    {
        return FALSE;
    }

    HANDLE hSnap = m_swi.m_fpCreateToolhelp32Snapshot(TH32CS_SNAPALL, m_dwProcessId);
    if (hSnap == INVALID_HANDLE_VALUE)
    {
        return FALSE;
    }

    THREADENTRY32 te;
    memset(&te, 0, sizeof(te));
    te.dwSize = sizeof(te);
    BOOL res = m_swi.m_fpThread32First(hSnap, &te);
    while (res)
    {
        if (te.th32OwnerProcessID == m_dwProcessId)
        {
            GetCallstack(callStacks, te.th32ThreadID);
        }
        res = m_swi.m_fpThread32Next(hSnap, &te);
    }

    CloseHandle(hSnap);

    return TRUE;
}

CStackWalker g_StackWalker;

