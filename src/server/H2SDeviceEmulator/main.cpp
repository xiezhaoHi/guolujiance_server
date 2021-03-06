
#include <QtCore/QCoreApplication>

#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include <stdlib.h>
#include <boost/asio.hpp>
#include "tcp_client.h"
#include "protocol/protocol_cmd.h"
#include "message/message_login.h"
#include "message/message_device_realtime_data.h"
#include "message/message_device_id.h"
#include "message/message_get_devices_info_req.h"
#include "message/message_set_device_param.h"
#include "message/message_get_device_data_count_req.h"
#include "message/message_get_device_data_req.h"
#include "message/message_body_factory.h"
#include "message/message_device_realtime_batched_data.h"
#include <QDebug>
#include <QString>
#include "QThread"
#include <QDateTime>

using namespace boost::asio;
using boost::asio::ip::tcp;

void Device(CTcpClient * pClient, QString & deviceCode);
void PCClient(CTcpClient * pClient);


int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cerr << "Usage: H2SDeviceEmulator <host> <port> <deviceCode>\n";
        //system("pause");
        //return 1;
    }

    //qDebug() << argv[0];
    //qDebug() << argv[1];
    //qDebug() << argv[2];
    //qDebug() << argv[3];

    QCoreApplication a(argc, argv);

    boost::asio::io_service io_service;

    tcp::resolver resolver(io_service);
    tcp::resolver::query query("127.0.0.1", "17024");
    auto endpoint_iterator = resolver.resolve(query);
    CTcpClient c(io_service, endpoint_iterator);

    std::thread t([&io_service](){ io_service.run(); });

    QString qstrDeviceCode;
    if (argc < 4) {
        qstrDeviceCode = QStringLiteral("1001");// 无输入设备编码时，默认以设备编码 510124000
    } else if (argc == 4) {
        qstrDeviceCode = QString::fromLocal8Bit(argv[3]);
    }

    CMessageBodyFactory::GetInstance();
    Device(&c, qstrDeviceCode);
    //PCClient(&c);

    return a.exec();
}

void Device(CTcpClient * pClient, QString & deviceCode)
{
    QThread::currentThread()->sleep(3);

    //-----------------------------------测试下位机命令   Start---------------------//

    //下位机登录命令
    CMessageLogin *deviceLogin = new CMessageLogin();
    deviceLogin->m_qstrAccount = deviceCode;// QStringLiteral("X14440935");
    deviceLogin->m_qstrPassword = QStringLiteral("123456");

    CMessage * message = new CMessage();
    message->m_header.m_struct.cmd = DEVICE_CMD_LOGIN;
    message->m_header.m_struct.messageNo = 1;
    message->m_header.m_struct.cmdResult = 1;// 非0表示 需要获取错误信息
    message->SetMessageBody(deviceLogin);

    qDebug() << "DEVICE LOGIN" << message->ToString();
    pClient->write(message);
    QThread::currentThread()->sleep(2);

    unsigned char buf[7];
    {
        memset(buf, 0x00, sizeof(buf));
        buf[0] = 0x00;// 工作状态：正常
        buf[1] = 0x00, buf[2] = 0x00, buf[3] = 0x12;// 实时浓度BCD码
        buf[4] = 0x02;// 两位小数
        buf[5] = 0x09;// 气体：H2S
        buf[6] = 0x02;// 单位：PPM
    }

    int tempData= 10;
    int i = 0;
	while (true)
	{
		i++;

		qDebug() << "Sleep 1 second, then send realtime data." << i;
		QThread::currentThread()->sleep(2);

		if (tempData > 100) tempData = 10;//tempData=0;
		buf[3] = tempData;
		
		CMessageDeviceRealtimeData *deviceData = new CMessageDeviceRealtimeData();

		for (int index = 0; index < 6; ++index)
		{


			//下位机发送实时采集数据命令
			deviceData = new CMessageDeviceRealtimeData();
			deviceData->m_struct.channelNo = index;
			deviceData->m_struct.datatype = 2;
			deviceData->m_struct.longitude = 0.1f;
			deviceData->m_struct.latitude = 0.2f;
			deviceData->m_struct.timestamp = QDateTime::currentMSecsSinceEpoch();
			memcpy(deviceData->m_struct.data_, buf, 7);
			//deviceData->m_struct.data = tempData;

			message = new CMessage();
			message->m_header.m_struct.cmd = DEVICE_CMD_REALTIME_DATA; //524292  
			message->m_header.m_struct.messageNo = 1;
			message->m_header.m_struct.cmdResult = 1;// 非0表示 需要获取错误信息
			message->SetMessageBody(deviceData);
			tempData+=1;
			qDebug() << tempData << QDateTime::fromMSecsSinceEpoch( deviceData->m_struct.timestamp).toString(" ___ hh:mm:ss");
			pClient->test_write(message);

		}
		
        CMessageDeviceRealtimeBatchedData * batchedData = new CMessageDeviceRealtimeBatchedData();
        batchedData->m_struct.dataItemCount = 0;

        message = new CMessage();
        message->m_header.m_struct.cmd = DEVICE_CMD_REALTIME_BATCHED_DATA;
        message->m_header.m_struct.messageNo = 1;
        message->m_header.m_struct.cmdResult = 1;// 非0表示 需要获取错误信息
        message->SetMessageBody(batchedData);
        tempData+=1;
        qDebug() << tempData << QDateTime::fromMSecsSinceEpoch( deviceData->m_struct.timestamp).toString(" ___ hh:mm:ss");
		pClient->write(message);
        message = new CMessage();
        message->m_header.m_struct.cmd = DEVICE_CMD_SEND_HEARTBEAT;
        message->m_header.m_struct.messageNo = 1;
        message->m_header.m_struct.cmdResult = 1;// 非0表示 需要获取错误信息
        message->SetMessageBody(deviceData);
        tempData+=1;
        qDebug() << tempData << QDateTime::fromMSecsSinceEpoch( deviceData->m_struct.timestamp).toString(" ___ hh:mm:ss");
       

		continue;
        if (i > 30)
        {
            qDebug() << "sleep 120 seconds";
            QThread::currentThread()->sleep(120);
            qDebug() << "wakeup after 120 seconds";
        }
    }

    //下位机退出命令
    message = new CMessage();
    message->m_header.m_struct.cmd = DEVICE_CMD_EXIT;
    message->m_header.m_struct.messageNo = 3;
    message->m_header.m_struct.cmdResult = 0;// 非0表示 需要获取错误信息
    pClient->write(message);
    qDebug() << QStringLiteral("DEVICE EXIT");
    QThread::currentThread()->sleep(30);
    pClient->close();
    qDebug() << QStringLiteral("DEVICE CLOSE SESSION");
}

void PCClient(CTcpClient * pClient)
{
    QThread::currentThread()->sleep(3);

    CMessageLogin *login = new CMessageLogin();
    login->m_qstrAccount = QStringLiteral("admin");
    login->m_qstrPassword = QStringLiteral("123456");

    CMessage * message = new CMessage();
    message->m_header.m_struct.cmd = PC_CLIENT_CMD_LOGIN;
    message->m_header.m_struct.messageNo = 1;
    message->m_header.m_struct.cmdResult = 1;// 非0表示 需要获取错误信息
    message->SetMessageBody(login);

    qDebug() << "PC CLIENT LOGIN " << message->ToString();
    pClient->write(message);
    QThread::currentThread()->sleep(3);

    // 取得设备列表
    message = new CMessage();
    message->m_header.m_struct.cmd = PC_CLIENT_CMD_GET_DEVICE_LIST;
    message->m_header.m_struct.messageNo = 2;
    message->m_header.m_struct.cmdResult = 1;// 不需要获取错误信息
    message->SetMessageBody(NULL);
    qDebug() << "PC CLIENT CMD GET DEVICE LIST " << message->ToString();
    pClient->write(message);
    QThread::currentThread()->sleep(3);

    // 查询设备信息
    message = new CMessage();
    message->m_header.m_struct.cmd = PC_CLIENT_CMD_GET_DEVICE_INFO;
    message->m_header.m_struct.messageNo = 3;
    message->m_header.m_struct.cmdResult = 1;// 非0表示 需要获取错误信息

    CMessageGetDevicesInfoReq * pReq = new CMessageGetDevicesInfoReq();
    CMessageDeviceID * pDeviceID = new CMessageDeviceID();
    char * strDeviceID = "4522AB01-4972-4A31-BEF4-21D3EB2CFE77";
    memcpy(pDeviceID->m_struct.deviceID, strDeviceID, 36);
    pReq->m_lstDeviceID.push_back(pDeviceID);
    message->SetMessageBody(pReq);

    qDebug() << "PC CLIENT CMD GET DEVICE INFO" << message->ToString();
    pClient->write(message);
    QThread::currentThread()->sleep(3);

    // 查询历史数据条数
    message = new CMessage();
    message->m_header.m_struct.cmd = PC_CLIENT_CMD_GET_HISTORY_DEVICE_DATA_COUNT;
    message->m_header.m_struct.messageNo = 7;
    message->m_header.m_struct.cmdResult = 1;// 非0表示 需要获取错误信息

    CMessageGetDeviceDataCountReq * pGetDeviceDataCountReq = new CMessageGetDeviceDataCountReq();
    QDateTime begin = QDateTime::fromString("2015-12-31 00:00:00.000", "yyyy-MM-dd HH:mm:ss.zzz");
    QDateTime end = QDateTime::fromString("2015-12-31 23:59:59.999", "yyyy-MM-dd HH:mm:ss.zzz");
    pGetDeviceDataCountReq->m_struct.begin = begin.toMSecsSinceEpoch();
    pGetDeviceDataCountReq->m_struct.end =end.toMSecsSinceEpoch();
    message->SetMessageBody(pGetDeviceDataCountReq);
    qDebug() << "PC_CLIENT_CMD_GET_HISTORY_DEVICE_DATA_COUNT" << message->ToString();
    pClient->write(message);
    QThread::currentThread()->sleep(3);

    // 查询历史数据
    for (int i = 0; i < 10; i++)
    {
        message = new CMessage();
        message->m_header.m_struct.cmd = PC_CLIENT_CMD_GET_HISTORY_DEVICE_DATA;
        message->m_header.m_struct.messageNo = 8;
        message->m_header.m_struct.cmdResult = 1;// 非0表示 需要获取错误信息

        CMessageGetDeviceDataReq * pGetDeviceDataReq = new CMessageGetDeviceDataReq();
        pGetDeviceDataReq->m_struct.begin = begin.toMSecsSinceEpoch();
        pGetDeviceDataReq->m_struct.end =end.toMSecsSinceEpoch();
        pGetDeviceDataReq->m_struct.pageIndex = i + 1;
        pGetDeviceDataReq->m_struct.rowsOfPerPage = 100;
        message->SetMessageBody(pGetDeviceDataReq);
        qDebug() << "PC_CLIENT_CMD_GET_HISTORY_DEVICE_DATA" << message->ToString();
        pClient->write(message);
        QThread::currentThread()->sleep(3);
    }
    

    // 设置设备参数
    message = new CMessage();
    message->m_header.m_struct.cmd = PC_CLIENT_CMD_SET_DEVICE_PARAM;
    message->m_header.m_struct.messageNo = 3;
    message->m_header.m_struct.cmdResult = 1;// 非0表示 需要获取错误信息

    CMessageSetDeviceParam * pParam = new CMessageSetDeviceParam();
    memcpy(pParam->m_struct.deviceID, strDeviceID, 36);
    memcpy(pParam->m_struct.warningLine1st, "15.36##aa", 9);
    memcpy(pParam->m_struct.warningLine2nd, "20.43##bb", 9);
    memcpy(pParam->m_struct.maxRange, "35.6##cc", 9);
    message->SetMessageBody(pParam);

    qDebug() << "PC CLIENT CMD SET DEVICE PARAM" << message->ToString();
    //TODO: pClient->write(message);
    QThread::currentThread()->sleep(3);

    while (false)
    {
        // 查询设备信息
        message = new CMessage();
        message->m_header.m_struct.cmd = PC_CLIENT_CMD_GET_DEVICE_INFO;
        message->m_header.m_struct.messageNo = 3;
        message->m_header.m_struct.cmdResult = 1;// 非0表示 需要获取错误信息

        CMessageGetDevicesInfoReq * pReq = new CMessageGetDevicesInfoReq();
        CMessageDeviceID * pDeviceID = new CMessageDeviceID();
        char * strDeviceID = "4522AB01-4972-4A31-BEF4-21D3EB2CFE77";
        memcpy(pDeviceID->m_struct.deviceID, strDeviceID, 36);
        pReq->m_lstDeviceID.push_back(pDeviceID);
        message->SetMessageBody(pReq);

        qDebug() << "PC CLIENT CMD GET DEVICE INFO" << message->ToString();
        pClient->write(message);
        QThread::currentThread()->sleep(3);
    }

    QThread::currentThread()->sleep(30000);
}
