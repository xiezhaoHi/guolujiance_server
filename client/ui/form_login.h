/*****************************************************
**  文 件 名：form_login.h
**  版 权：XXX 版权所有
**  版    本：Ver.1.0
**  内容简述：登录画面类头文件
**
**  创建日期：2015/07/22 
**  创建人：杨恒
**  修改记录：
    日期        版本      修改人    修改内容   
*****************************************************/

#ifndef FORM_LOGIN_H
#define FORM_LOGIN_H

#include <QWidget>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QDir>
#include "qcustombutton.h"
#include "connection\tcp_manager.h"
#include "connection\qmessagethread.h"
#include "qwaitwidget.h"
#include <QKeyEvent>
#include "qcheckrecvsysqueuethread.h"
#include "diskid32.h"
#pragma  comment(lib,"diskid32.lib")
#include "yxyDES2.h"
#include "qpasswd_dlg.h"
#include "qdevice_collect_dlg.h"
#include <QtSerialPort/QSerialPort>
#include "qinit_configuration.h"

#include <QtSerialPort/QSerialPortInfo>


class CFormLogin : public QWidget
{
    Q_OBJECT

public:
    CFormLogin(QWidget *parent = 0);
    ~CFormLogin();

    // 判断是否登录成功
    bool LoginSucceed();

    // 获取登录账号
    QString GetAccount() const { return m_strAccount; }

    // 设置登录账号
    void SetAccount(QString val) { m_strAccount = val; }

    // 获取登录密码
    QString GetPassword() const { return m_strPassword; }

    // 设置登录密码
    void SetPassword(QString val) { m_strPassword = val; }


    QMessageThread *m_messageThread;
signals:

    void LoginCompleted(bool loginSucceed);

private:

    // 构建登录页界面
    void BuildUI();

    // 美化界面
    void BeautifyUI();

    // 翻译界面元素
    void TranslateUI();

    // 绑定本界面相关的信号与槽
    void BindSlots();
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *);
    bool isRegist();
    //void InitDir();
private slots:
    // 登录
    void Login();
    // 取消
    void LoginCancel();
    void ServerRepInfoSlot(CMessage*);
    void WaitTimeOut();
    void ServerRepMsgSlot(QString);
    void ResgitLocalHost();
    void DeviceCollectSlot();
private:
    QLineEdit     * m_pEditAccount;         // 账号输入框
    QLineEdit     * m_pEditPassword;        // 密码输入框
    QCheckBox     * m_pCheckSavePassword;   // 保存密码选择框
    QCheckBox     * m_pCheckAutoLogin;      // 自动登录选择框
    QPushButton   * m_pButtonServerSetting; // 服务器设备对话框触发按钮
    QCustomButton   * m_pButtonLogin;         // 登录按钮
    QCustomButton   * m_pButtonCancel;        // 取消按钮
    QCustomButton   *m_pButton_Collect;       //设备校准

    QPasswd_dlg    *m_passwd_dlg;             //较准用户名
    QDevice_Collect_dlg *m_collect_dlg;       //校正界面

    QWaitWidget    *m_wait_dlg;
    QTimer *m_timer;
    QString m_commid;

private:
    bool            m_bLoginSucceed;        // 是否登录成功
    QString         m_strAccount;           // 登录账号
    QString         m_strPassword;          // 登录密码
    QString         m_diskid;               //注册码
signals:
    void SendUserName(QString,QString);
};

#endif // FORM_LOGIN_H
