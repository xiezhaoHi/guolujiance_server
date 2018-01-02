#ifndef H2SMONITORCLIENT_H
#define H2SMONITORCLIENT_H

#include <QtWidgets/QMainWindow>
#include <QTabWidget>
#include <QPushButton>
#include "form_login.h"
#include "form_device_data_curve.h"
#include "form_device_data_curve_container.h"
#include "form_device_data_table_view.h"
#include "qcustombutton.h"
#include "qstatebutton.h"
#include "qinnerwidget.h"
#include <QVector>
#include "qdatashowwidget.h"
#include "qphotoshowwidget.h"
#include "connection\qmessagethread.h"
#include "message/message_get_device_list_rep.h"
#include "qall_pannel_widget.h"
#include "qshow_struct.h"
#include "qparamsetwidget.h"
#include "qshowwarndlg.h"
#include "qwarning_widget.h"
#include "qrunstateinfo_widget.h"
#include "qcheckrecvdataqueuethread.h"
#include "qdialwidget.h"
#include <QTableWidget>
#include "qallpaneltablewidget.h"
#include "qexportwidget.h"
#include <QSqlDatabase>
#include "qallwarninfowidget.h"
#include "qremovedatatobasethread.h"
#include "qinit_configuration.h"
#include "util/global_util.h"
#include <errno.h>
#include <io.h>
#include <direct.h>
#include <QMediaPlayer>
#include <QMediaPlaylist>

class H2SMonitorClient : public QMainWindow
{
    Q_OBJECT

public:
    H2SMonitorClient(QWidget *parent = 0);
    ~H2SMonitorClient();

    void BindLoginForm(CFormLogin * formLogin);
    void setMessageThread(QMessageThread *messageThread);
private:
    // 构建主界面主按钮
    void BuildUI();

    // 美化界面
    void BeautifyUI();

    // 翻译界面元素
    void TranslateUI();

    // 连接与本界面相关的信号和槽
    void BindSlots();

    //创建左侧按钮界面
    void BuildBtnWidget();

    void paintEvent(QPaintEvent *p);

    //获取设备列表信息
    void GetDeviceListInfo();


    //创建所有的面板
    void BuildAllPannel();

    //创建所有的曲线
    void BuildAllSpline();


    void GetLoginInfo();

    void resizeEvent(QResizeEvent *e);

    void setBtnBackGround(int index);

    void WarnTestValue(double, int index, int id);

    void AveCalculate(double,double, int index, int probe ,DATA_INFO* st);

    void InitDir();

    // 更新服务器时间
    void UpdateServerTime(const QString & serverTime, const QString & weekday);

	void batteryValue(unsigned char &value);

private slots:

    void HandleLoginResult(bool loginSucceed);
    void PanelShowSlot();
    void ParamSetSlot();
    void ImageShowSlot();
    void ExitSlot();
    void DataShowSlot();
    //void infoViewWidgetMouseKeyDown(QPoint);
    //void infoViewWidgetMouseKeyUp(QPoint);
    //void infoViewWidgetMouseMove(QPoint);
    void ServerRepInfoSlot(CMessage*);
    void ServerRepDataSlot(const CMessage *);
    void TimeoutSlot();
    void InstrumentClickSlot();
    void ChangeDeviceSlot();
    /*
    void AllPannelMouseKeyDown(QPoint);
    void AllPannelMouseKeyUp(QPoint);
    void AllPannelMouseKeyMove(QPoint);*/
    void UpdateButtonState();
    void SaveParamSlot(QString msg);
    void SaveWarnParamSlot(int, int);
    void ShowWarning1st();
    void ShowWarning2nd();
    void CleanWarningInfo();
    void UpdateWarnState();
    void GetUserNameSlot(QString user,QString pwd );
    void ServerDisconnectSlot();
	void ServerConnectSlot();
    void ExportShowSlot();
    void AllWarnInfoShow();
    void QueryWarnInfoSlot();
    void DataToExcelBegin(QString str);
    void DataToExcelEnd(int);
    void SaveDataCommpeleteSlot();
	void HeartBeatSlot();


private:
    QLabel *m_lb_user;
    QTabWidget                    * m_pTabWidget;               // 主tab选项区域
  //  CFormDeviceDataCurveContainer * m_pDeviceDataCurveContainer;// 曲线图tab
  //  CFormDeviceDataTableView      * m_pDeviceDataTableView;     // 表格视图tab
    QWidget *m_pDeviceDataCurveContainer;// 曲线图tab
    QWidget *m_pDeviceDataTableView;// 曲线图tab

    QCustomButton * m_ButtonPanelShow;   //面板显示按钮
    QCustomButton * m_pButtonParamsSet;  //参数设置按钮
    QCustomButton * m_pButtonDataShow;   //数据显示按钮
    QCustomButton * m_pButtonImageShow;  //图像显示按钮
    QCustomButton * m_pButtonExport;     //数据导出
    QCustomButton * m_pButtonWarningInfo;//报警信息
    QCustomButton * m_pButtonExit;       //退出按钮

    QWidget *m_tabWidget;
    QDataShowWidget *m_dataShow;         //数据显示对话框
    QPhotoShowWidget *m_photoShow;       //图像显示对话框
    QPhotoShowWidget *m_allSplineShow;   //显示所有曲线
    QExportWidget    *m_exportWidget;    // 导出对话框
    QAllWarnInfoWidget *m_allWarnInfoWidget; //显示所有报警信息
    //QInnerWidget *m_innerwidget;  
    QAllPanelTableWidget  *m_innerwidget;
    QAll_pannel_widget *m_allPannelInnerwidget;    //显示所有面板内框
    QInnerWidget *m_allSplineInnerwidget;           //显示所有曲线内框
    QWidget *m_outerwidget;
    QParamSetWidget  *m_paramSetWidget;  //参数设置
    //QCustomButton  *m_warnline_1st;
    //QCustomButton  *m_warnline_2nd;
    QWarning_Widget  *m_warnline_1st;    //一级预警
    QWarning_Widget  *m_warnline_2nd;    //二级预警
    QStateButton    *m_warn_clean;       //清除预警
    QLabel           *m_warnline_1str_count;
    QLabel           *m_warnline_2nd_count;
    int               m_1stcount;
    int               m_2ndcount;

    QShowWarnDlg     *m_warnline_1st_dlg;   //一级预警信息显示
    QShowWarnDlg     *m_warnline_2nd_dlg;   //二级预警信息显示 
    QRunStateInfo_Widget *m_state_widget;   //运行状态信息
    QString m_userName;                     //用户名
    QString m_userPwd;
    int m_leftBtnHight;                     //左侧按钮数量
    QVector<BUTTON_INSTRUMENT> m_btn_instrument;     //按钮信息
    QVector<DATA_SHOW_INFO> m_dataShowInfo;      //数据显示信息

    QVector<PANNEL_DATA> m_allPannelData;        //所有面板显示数据信息

    QVector<T_MSG_DEVICES_LIST_ITEM> m_deviceListInfo;  //设备列表信息

    
    QWaitWidget *m_wait_dlg;          //等待对话框
    QTimer *m_timer;
    QTimer *m_tm_UpdateTimer;
	QTimer *m_tm_HeartBeat;                        //心跳包发送点时器
    QMessageThread *m_messageThread;               //消息线程
    QRemoveDataToBaseThread *m_saveDataThread;      //数据库线程
    QVector<DATA_INFO*> m_vtbasedata;
    QVector<DATA_INFO> m_vt_avebasedata;


    QLabel *m_lb_title1, *m_lb_title2,*m_lb_show, *m_lb_set, *m_lb_data, *m_lb_image, *m_lb_exit;
    QLabel *m_lb_wel, *m_lb_panel, *m_lb_monit, *m_lb_ver;
    QLabel *m_lb_export, *m_lb_warnInfo;
    QSqlDatabase m_msgdb;                     //报警消息数据库
    QSqlDatabase m_datadb;                    //测量数据数据库
    QSqlDatabase m_aveDatadb;                 //平均数据数据库
    QMutex     * m_pMutexForMsgDB;
    QMutex m_mutex;
    WARN_TYPE m_warnType;
    int m_aveTime;
    int m_calculateCount;
    int m_connectCount;          //正在发送数据的设备数

private:
    QPoint m_beginPos;
    bool m_isDownd;
	bool m_isLogin;                 //是否成功登陆。
    int m_currentDeviceNo;
    QVector< XY_VALUE > m_recvXYData;    //接收到的数据和时间
    QVector<XY_VALUE > m_recvAveData;    //平均值
    QVector<XY_VALUE> m_recvCaculate;      //用来计算平均值的容器

    QString             m_qstrInstDir;  // 实例安装目录
    QString             m_qstrDBDir;    // 数据库文件存放目录
    QString             m_qstrLogDir;   // 日志文件存放目录
    QString             m_qstrExportdir;// 导出文件存放目录
	QMediaPlayer *m_WarnPlayer;
	QMediaPlaylist *m_mediaList;
	bool m_isFisrtPlayer;
	bool m_isSecondPlayer;

private:
    CFormLogin        * m_pFormLogin;// 登录页
    void SetDataShowDlg(int index);
    void SetPhotoShowDlg( int index );
    void SetParamSetDlg( int index );
    void InitDatabase();
signals:
     void UpdateWarnSignal();
};

#endif // H2SMONITORCLIENT_H
