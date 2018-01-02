#ifndef H2SMONITORSERVER_H
#define H2SMONITORSERVER_H

#include <QtWidgets/QMainWindow>
#include <QEvent>
#include <QSplitter>
#include "ui/form_devices_container.h"
#include "ui/form_rig_device_detail.h"
#include "ui/form_server_control_panel.h"


class QMenu;
class QAction;
class CFormLogViewer;

class H2SMonitorServer : public QMainWindow
{
    Q_OBJECT

public:

    H2SMonitorServer(QWidget *parent = 0);

    ~H2SMonitorServer();


public slots:
    // 程序启动运行
    void runServices();

    // 停止程序内部各种服务
    void destroyServices();

    // 窗口关闭事件，停止各种已经启动的内部服务
    void closeEvent(QCloseEvent * event);

private:
    // 
    void BuildUI();

    // 
    void BeautifyUI();

    // 
    void TranslateUI();

    // 绑定信号
    void BindSlots();

private slots:

    void menuLogViewClicked(bool checked);

    void switchServiceStatus(bool startService);

private:
    QSplitter              * m_pSplitter;
    CFormDevicesContainer  * m_pFormDevicesContainer;
    CFormRigDeviceDetail   * m_pFormDeviceDetail;
    CFormServerControlPanel* m_pFormServerControlPanel;
    QMenu                  * m_pMenuViews;
    QAction                * m_pActionLogView;
    CFormLogViewer         * m_pLogViewer;
};




#endif // H2SMONITORSERVER_H
