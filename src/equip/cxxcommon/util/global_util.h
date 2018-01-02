#ifndef GLOBAL_UTIL_H
#define GLOBAL_UTIL_H
/*****************************************************
**  �� �� ����tcp_logic.h
**  �� Ȩ��XXX ��Ȩ����
**  ��    ����Ver.1.0
**  ���ݼ�����ͨѶ�߼���ͷ�ļ�
**
**  �������ڣ�2015/07/22 
**  �����ˣ����
**  �޸ļ�¼��
    ����        �汾      �޸���    �޸�����   
*****************************************************/
#include <thread>
#include <boost/asio.hpp>
#include <QString>
#include <QSettings>
#include <QFile>
#include <QtWidgets/QApplication>
#include <QCryptographicHash>
#include <QVector>

// H2S�ͻ��˳�������Ŀ���������� USED_BY_H2SMONITORCLIENT_ONLY ���������õĹ��� tangqiao 2015/11/20
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


// ѹ��BCD��ת10����
int BCD2INT(unsigned char * bcd, int bcdLen);

double AveCalculatefromQVector(QVector<double> vt);
// ѹ��BCD��ת������
double BCD2Double(unsigned char * bcd, int bcdLen, int decimalLen);

// ���豸�ϴ���7�ֽ�ԭʼ�����еľ�������ֵת��Ϊ�����ͱ�ʾ
double Convert7ByteDataToD64(unsigned char buf[7]);

// ��U8תΪBCD���ʾ,����ʾ��99
unsigned char U8toBCD(unsigned char u8);

// ��U16תΪBCD���ʾ,����ʾ��9999
unsigned short U16toBCD(unsigned short u16);


#ifdef USED_BY_H2SMONITORCLIENT_ONLY
double Max(QVector<double> &vec);
void AveCalculator(XY_VALUE ProbeValue, int aveTime, XY_VALUE &out);
#endif


#endif