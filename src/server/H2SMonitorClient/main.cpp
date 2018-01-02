#include "h2smonitorclient.h"
#include "ui/form_login.h"
#include <QtWidgets/QApplication>
#include <QFile>
#include <QString>
#include <QDesktopWidget>
#include <QString>
#include <QSettings>
#include "util/global_util.h"
#include <QTranslator>
#include "connection\tcp_manager.h"
#include <QDebug>
/*
#ifdef _DEBUG 
#include "vld.h" 
#endif
*/
#ifdef WIN32
#include <Windows.h>
#include <DbgHelp.h>
LONG WINAPI H2SMonitorServerUnhandledExceptionFilter(
struct _EXCEPTION_POINTERS* ExceptionInfo
    );
#endif

int main(int argc, char *argv[])
{
    /*
#ifdef _DEBUG 
    _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif*/
    QApplication a(argc, argv);
    QString strLibPath(QDir::toNativeSeparators(QApplication::applicationDirPath())+QDir::separator()+"plugins");
    a.addLibraryPath(strLibPath);
    CGlobalUtil::getInstance()->CreateIniFile();

    QTranslator translator;
    translator.load("h2smonitorclient_zh.qm");
    a.installTranslator(&translator);
    // 设置程序的整体样式
    /*QFile qssFile(":/qss/application_common_style.qss");
    if (qssFile.open(QFile::ReadOnly)) {
    QString stylesheet = QLatin1String(qssFile.readAll());
    a.setStyleSheet(stylesheet);
    qssFile.close();
    }*/
#ifdef WIN32
    // 记录程序异常
    SetUnhandledExceptionFilter(H2SMonitorServerUnhandledExceptionFilter);
#endif
    // 硫化氢监控主画面创建
    H2SMonitorClient * mainWindow = new H2SMonitorClient();
    // 登录画面创建
    CFormLogin * loginWindow = new CFormLogin();

    // 主画面绑定登录画面
    mainWindow->setMessageThread( loginWindow->m_messageThread );
    mainWindow->BindLoginForm(loginWindow);
    // 登录画面显示
    loginWindow->show();
    QDesktopWidget* desktop = QApplication::desktop();
    loginWindow->move((desktop->width() - loginWindow->width())/2, (desktop->height() - loginWindow->height())/2);

   // CWidgetDeviceSetup dlg;
    //dlg.show();

    return a.exec();
}


#ifdef WIN32 
#include <QDir>

#pragma comment(lib, "Dbghelp.lib")

LONG WINAPI H2SMonitorServerUnhandledExceptionFilter(
    __in          struct _EXCEPTION_POINTERS* ExceptionInfo
    )
{
    // 创建dmp目录
    QString strDmpDirName(QStringLiteral("./dmp"));
    QDir dmpDir(strDmpDirName);
    if (!dmpDir.exists()) {
        if (!dmpDir.mkdir(strDmpDirName)) {
            qDebug() << QStringLiteral("创建dmp目录失败,error:%1")
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
            qDebug() << QStringLiteral("写dmp文件失败,error:%1")
                .arg(GetLastError());
        } else {
            qDebug() << QStringLiteral("写dmp文件[%1]完成")
                .arg(QString().toWCharArray(dmpFileName));
        }
    }
    CloseHandle(hDmpFile); 
    return EXCEPTION_CONTINUE_SEARCH;
}

#endif // WIN32