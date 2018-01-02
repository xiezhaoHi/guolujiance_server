#ifndef QMESSAGETHREAD_H
#define QMESSAGETHREAD_H

#include <QThread>

#include <boost/asio.hpp>
#include "tcp_client.h"
#include "protocol/protocol_cmd.h"
#include "message/message.h"
#include "message/message_login.h"
#include "message_queue.h"
#include "util/global_util.h"
#include "util/sync_cond_queue.h"
#include <QString>
#include <QQueue>
#include <QSettings>
#include <QFile>
#include <QtWidgets/QApplication>
#include <QTimer>
#include "qioservicethread.h"

class QMessageThread : public QThread
{
    Q_OBJECT

public:
    QMessageThread(QObject *parent);
    QMessageThread();
    ~QMessageThread();
    void run();
    boost::asio::io_service io_service;
    
public:
    //static QMessageThread* getInstance();
    bool init();
    void StartTcpServer();
    void SetThreadOver();
    bool SendMessage(CMessage *message);
    CMessage* SysQueuePopFront();
    CMessage* DataQueuePopFront();
	CMessage *LoadDataQueuePopFront();
    bool DataQueueIsEmpty();
    bool SysQueueIsEmpty();
	bool LoadQueueIsEmpty();
    bool SysWait(unsigned long time);
    bool DataWait(unsigned long timeout);
	bool LoadDataWait(unsigned long timeout);
    void setTcpClient(CTcpClient *tcpClient );
    void setReConnectInfo(QString str, QString pwd);
    public slots:
        void ConnectSlot();
		void ReconnectServer();

private:
    bool m_isConnect;
    bool m_isquit;
    bool m_isSendMsg;
    CTcpClient *m_pTcpClient;
    QString serverIP;
    QString serverPort;
    session_message_queue       sys_msgs_queue;
    session_message_queue       data_msgs_queue;
	session_message_queue       load_data_queue;
    QMutex m_mutex;
    QTimer *m_connectTimer;
    QIOServiceThread *m_ioserviceThread;

    bool isInit;
    QString struser;
    QString strPwd;
    int m_icount;    
    std::thread m_t;
    
signals:
    void ServerRepMsg(QString );
    void ServerDisconnect();
	void ServierConnect();
	void ServerRepDeviceDataCount(CMessage* );
};
extern QMessageThread *g_messageThread;
#endif // QMESSAGETHREAD_H
