#include "h2smonitorserver.h"
#include "windows_service_entry.h"
#include "state_collector/path_collector.h"
#include <QtWidgets/QApplication>
#include <QFile>
#include <QDir>
#include <QString>
#include "util/global_util.h"


int main(int argc, char *argv[])
{
	QString strTemp = QString("%1").arg(argc);
	
    if (argc == 2 
        && strcmp(argv[1], "gui") == 0)// 命令行参数指定以GUI的方式运行程序
	{
		
        QApplication a(argc, argv);
        CPathCollector::m_strAppDirPath = a.applicationDirPath();

        QFile qssFile(":/qss/application_common_style.qss");
        if (qssFile.open(QFile::ReadOnly)) {
            QString stylesheet = QLatin1String(qssFile.readAll());
            a.setStyleSheet(stylesheet);
            qssFile.close();
        }

        H2SMonitorServer w;
        w.show();

        return a.exec();
    }
    else// 默认以windows服务的方式启动运行
    {
        // 获取程序文件所在目录
        wchar_t strFilePath[MAX_PATH + 1] = {0};
        GetModuleFileName(NULL, strFilePath, sizeof(strFilePath));
        wchar_t * pStrDir = wcsrchr(strFilePath, '\\');
        *pStrDir = '\0';

        // 切换程序运行目录为exe所在目录
        CPathCollector::m_strAppDirPath = QString::fromStdWString(std::wstring(strFilePath));
        QDir::setCurrent(CPathCollector::m_strAppDirPath);
        QCoreApplication::addLibraryPath(CPathCollector::m_strAppDirPath);

        SERVICE_TABLE_ENTRY ServiceTable[2];
        ServiceTable[0].lpServiceName = SVCNAME;
        ServiceTable[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTION)SvcMain;
        ServiceTable[1].lpServiceName = NULL;  
        ServiceTable[1].lpServiceProc = NULL;
        StartServiceCtrlDispatcher(ServiceTable);
        return 0;
    }
}
