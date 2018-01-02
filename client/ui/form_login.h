/*****************************************************
**  �� �� ����form_login.h
**  �� Ȩ��XXX ��Ȩ����
**  ��    ����Ver.1.0
**  ���ݼ�������¼������ͷ�ļ�
**
**  �������ڣ�2015/07/22 
**  �����ˣ����
**  �޸ļ�¼��
    ����        �汾      �޸���    �޸�����   
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

    // �ж��Ƿ��¼�ɹ�
    bool LoginSucceed();

    // ��ȡ��¼�˺�
    QString GetAccount() const { return m_strAccount; }

    // ���õ�¼�˺�
    void SetAccount(QString val) { m_strAccount = val; }

    // ��ȡ��¼����
    QString GetPassword() const { return m_strPassword; }

    // ���õ�¼����
    void SetPassword(QString val) { m_strPassword = val; }


    QMessageThread *m_messageThread;
signals:

    void LoginCompleted(bool loginSucceed);

private:

    // ������¼ҳ����
    void BuildUI();

    // ��������
    void BeautifyUI();

    // �������Ԫ��
    void TranslateUI();

    // �󶨱�������ص��ź����
    void BindSlots();
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *);
    bool isRegist();
    //void InitDir();
private slots:
    // ��¼
    void Login();
    // ȡ��
    void LoginCancel();
    void ServerRepInfoSlot(CMessage*);
    void WaitTimeOut();
    void ServerRepMsgSlot(QString);
    void ResgitLocalHost();
    void DeviceCollectSlot();
private:
    QLineEdit     * m_pEditAccount;         // �˺������
    QLineEdit     * m_pEditPassword;        // ���������
    QCheckBox     * m_pCheckSavePassword;   // ��������ѡ���
    QCheckBox     * m_pCheckAutoLogin;      // �Զ���¼ѡ���
    QPushButton   * m_pButtonServerSetting; // �������豸�Ի��򴥷���ť
    QCustomButton   * m_pButtonLogin;         // ��¼��ť
    QCustomButton   * m_pButtonCancel;        // ȡ����ť
    QCustomButton   *m_pButton_Collect;       //�豸У׼

    QPasswd_dlg    *m_passwd_dlg;             //��׼�û���
    QDevice_Collect_dlg *m_collect_dlg;       //У������

    QWaitWidget    *m_wait_dlg;
    QTimer *m_timer;
    QString m_commid;

private:
    bool            m_bLoginSucceed;        // �Ƿ��¼�ɹ�
    QString         m_strAccount;           // ��¼�˺�
    QString         m_strPassword;          // ��¼����
    QString         m_diskid;               //ע����
signals:
    void SendUserName(QString,QString);
};

#endif // FORM_LOGIN_H
