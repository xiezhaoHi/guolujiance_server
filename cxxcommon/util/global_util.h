#ifndef GLOBAL_UTIL_H
#define GLOBAL_UTIL_H
/*****************************************************
**  文 件 名：tcp_logic.h
**  版 权：XXX 版权所有
**  版    本：Ver.1.0
**  内容简述：通讯逻辑类头文件
**
**  创建日期：2015/07/22 
**  创建人：杨恒
**  修改记录：
    日期        版本      修改人    修改内容   
*****************************************************/
#include <thread>
#include <boost/asio.hpp>
#include <QString>
#include <QSettings>
#include <QFile>
#include <QtWidgets/QApplication>
#include <QCryptographicHash>
#include <QVector>

// H2S客户端程序在项目属性里设置 USED_BY_H2SMONITORCLIENT_ONLY 启用其引用的功能 tangqiao 2015/11/20
#ifdef USED_BY_H2SMONITORCLIENT_ONLY
#include "qshow_struct.h"
#endif

class CGlobalUtil
{
public:
    static CGlobalUtil* getInstance();
    bool init();
    CGlobalUtil(void);
    ~CGlobalUtil(void);
    void CreateIniFile();
    void ReadIniFile();
    QString GetServerIP();
    QString GetServerPort();
    QString MD5(QString pwd);
private:
    QString GetIniFilePath();
    
private:
    QString serverIP;
    QString serverPort;
};


// 压缩BCD码转10进制
int BCD2INT(unsigned char * bcd, int bcdLen);

double AveCalculatefromQVector(QVector<double> vt);
// 压缩BCD码转浮点数
double BCD2Double(unsigned char * bcd, int bcdLen, int decimalLen);

// 将设备上传的7字节原始数据中的具体数据值转换为浮点型表示
double Convert7ByteDataToD64(unsigned char buf[7]);

// 将U8转为BCD码表示,最多表示到99
unsigned char U8toBCD(unsigned char u8);

// 将U16转为BCD码表示,最多表示到9999
unsigned short U16toBCD(unsigned short u16);


#ifdef USED_BY_H2SMONITORCLIENT_ONLY
double Max(QVector<double> &vec);
void AveCalculator(XY_VALUE ProbeValue, int aveTime, XY_VALUE &out);
#endif


#endif