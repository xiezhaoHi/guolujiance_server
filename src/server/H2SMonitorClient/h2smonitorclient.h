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
    // ��������������ť
    void BuildUI();

    // ��������
    void BeautifyUI();

    // �������Ԫ��
    void TranslateUI();

    // �����뱾������ص��źźͲ�
    void BindSlots();

    //������ఴť����
    void BuildBtnWidget();

    void paintEvent(QPaintEvent *p);

    //��ȡ�豸�б���Ϣ
    void GetDeviceListInfo();


    //�������е����
    void BuildAllPannel();

    //�������е�����
    void BuildAllSpline();


    void GetLoginInfo();

    void resizeEvent(QResizeEvent *e);

    void setBtnBackGround(int index);

    void WarnTestValue(double, int index, int id);

    void AveCalculate(double,double, int index, int probe ,DATA_INFO* st);

    void InitDir();

    // ���·�����ʱ��
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
    QTabWidget                    * m_pTabWidget;               // ��tabѡ������
  //  CFormDeviceDataCurveContainer * m_pDeviceDataCurveContainer;// ����ͼtab
  //  CFormDeviceDataTableView      * m_pDeviceDataTableView;     // �����ͼtab
    QWidget *m_pDeviceDataCurveContainer;// ����ͼtab
    QWidget *m_pDeviceDataTableView;// ����ͼtab

    QCustomButton * m_ButtonPanelShow;   //�����ʾ��ť
    QCustomButton * m_pButtonParamsSet;  //�������ð�ť
    QCustomButton * m_pButtonDataShow;   //������ʾ��ť
    QCustomButton * m_pButtonImageShow;  //ͼ����ʾ��ť
    QCustomButton * m_pButtonExport;     //���ݵ���
    QCustomButton * m_pButtonWarningInfo;//������Ϣ
    QCustomButton * m_pButtonExit;       //�˳���ť

    QWidget *m_tabWidget;
    QDataShowWidget *m_dataShow;         //������ʾ�Ի���
    QPhotoShowWidget *m_photoShow;       //ͼ����ʾ�Ի���
    QPhotoShowWidget *m_allSplineShow;   //��ʾ��������
    QExportWidget    *m_exportWidget;    // �����Ի���
    QAllWarnInfoWidget *m_allWarnInfoWidget; //��ʾ���б�����Ϣ
    //QInnerWidget *m_innerwidget;  
    QAllPanelTableWidget  *m_innerwidget;
    QAll_pannel_widget *m_allPannelInnerwidget;    //��ʾ��������ڿ�
    QInnerWidget *m_allSplineInnerwidget;           //��ʾ���������ڿ�
    QWidget *m_outerwidget;
    QParamSetWidget  *m_paramSetWidget;  //��������
    //QCustomButton  *m_warnline_1st;
    //QCustomButton  *m_warnline_2nd;
    QWarning_Widget  *m_warnline_1st;    //һ��Ԥ��
    QWarning_Widget  *m_warnline_2nd;    //����Ԥ��
    QStateButton    *m_warn_clean;       //���Ԥ��
    QLabel           *m_warnline_1str_count;
    QLabel           *m_warnline_2nd_count;
    int               m_1stcount;
    int               m_2ndcount;

    QShowWarnDlg     *m_warnline_1st_dlg;   //һ��Ԥ����Ϣ��ʾ
    QShowWarnDlg     *m_warnline_2nd_dlg;   //����Ԥ����Ϣ��ʾ 
    QRunStateInfo_Widget *m_state_widget;   //����״̬��Ϣ
    QString m_userName;                     //�û���
    QString m_userPwd;
    int m_leftBtnHight;                     //��ఴť����
    QVector<BUTTON_INSTRUMENT> m_btn_instrument;     //��ť��Ϣ
    QVector<DATA_SHOW_INFO> m_dataShowInfo;      //������ʾ��Ϣ

    QVector<PANNEL_DATA> m_allPannelData;        //���������ʾ������Ϣ

    QVector<T_MSG_DEVICES_LIST_ITEM> m_deviceListInfo;  //�豸�б���Ϣ

    
    QWaitWidget *m_wait_dlg;          //�ȴ��Ի���
    QTimer *m_timer;
    QTimer *m_tm_UpdateTimer;
	QTimer *m_tm_HeartBeat;                        //���������͵�ʱ��
    QMessageThread *m_messageThread;               //��Ϣ�߳�
    QRemoveDataToBaseThread *m_saveDataThread;      //���ݿ��߳�
    QVector<DATA_INFO*> m_vtbasedata;
    QVector<DATA_INFO> m_vt_avebasedata;


    QLabel *m_lb_title1, *m_lb_title2,*m_lb_show, *m_lb_set, *m_lb_data, *m_lb_image, *m_lb_exit;
    QLabel *m_lb_wel, *m_lb_panel, *m_lb_monit, *m_lb_ver;
    QLabel *m_lb_export, *m_lb_warnInfo;
    QSqlDatabase m_msgdb;                     //������Ϣ���ݿ�
    QSqlDatabase m_datadb;                    //�����������ݿ�
    QSqlDatabase m_aveDatadb;                 //ƽ���������ݿ�
    QMutex     * m_pMutexForMsgDB;
    QMutex m_mutex;
    WARN_TYPE m_warnType;
    int m_aveTime;
    int m_calculateCount;
    int m_connectCount;          //���ڷ������ݵ��豸��

private:
    QPoint m_beginPos;
    bool m_isDownd;
	bool m_isLogin;                 //�Ƿ�ɹ���½��
    int m_currentDeviceNo;
    QVector< XY_VALUE > m_recvXYData;    //���յ������ݺ�ʱ��
    QVector<XY_VALUE > m_recvAveData;    //ƽ��ֵ
    QVector<XY_VALUE> m_recvCaculate;      //��������ƽ��ֵ������

    QString             m_qstrInstDir;  // ʵ����װĿ¼
    QString             m_qstrDBDir;    // ���ݿ��ļ����Ŀ¼
    QString             m_qstrLogDir;   // ��־�ļ����Ŀ¼
    QString             m_qstrExportdir;// �����ļ����Ŀ¼
	QMediaPlayer *m_WarnPlayer;
	QMediaPlaylist *m_mediaList;
	bool m_isFisrtPlayer;
	bool m_isSecondPlayer;

private:
    CFormLogin        * m_pFormLogin;// ��¼ҳ
    void SetDataShowDlg(int index);
    void SetPhotoShowDlg( int index );
    void SetParamSetDlg( int index );
    void InitDatabase();
signals:
     void UpdateWarnSignal();
};

#endif // H2SMONITORCLIENT_H
