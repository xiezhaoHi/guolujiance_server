/*****************************************************
**  �� �� ����tcp_logic.cpp
**  �� Ȩ��XXX ��Ȩ����
**  ��    ����Ver.1.0
**  ���ݼ�����ͨѶ�߼���ʵ���ļ�
**
**  �������ڣ�2015/07/22 
**  �����ˣ����
**  �޸ļ�¼��
    ����        �汾      �޸���    �޸�����   
*****************************************************/

#include "global_util.h"

//�������󷽷�
static CGlobalUtil *_sharedObject = nullptr;
CGlobalUtil* CGlobalUtil::getInstance()
{
    if (! _sharedObject)
    {
        _sharedObject = new CGlobalUtil();
        _sharedObject->init();
    }

    return _sharedObject;
}

//��ʼ��
bool CGlobalUtil::init()
{
    //��ȡ�����ļ�
    ReadIniFile();
    return true;
}

//����
CGlobalUtil::CGlobalUtil(void)
{
}

//����
CGlobalUtil::~CGlobalUtil(void)
{
}

//��ʼTcp����
QString CGlobalUtil::GetServerIP()
{
    return serverIP;
}
QString CGlobalUtil::GetServerPort()
{
    return serverPort;
}
QString CGlobalUtil::GetIniFilePath()
{
    QString filePath;  
    filePath=QCoreApplication::applicationDirPath();
    filePath+="/app.ini";
    return filePath;
}
void CGlobalUtil::CreateIniFile()
{
    QString filePath=GetIniFilePath();
    if(!QFile::exists(filePath))
    {
        //Qt��ʹ��QSettings���дini�ļ�   
        //QSettings���캯���ĵ�һ��������ini�ļ���·��,�ڶ���������ʾ���ini�ļ�,��������������ȱʡ   
        QSettings *configIniWrite = new QSettings(filePath, QSettings::IniFormat);  
        //��ini�ļ���д������,setValue���������������Ǽ�ֵ��   
        //��ini�ļ��ĵ�һ����д�������������Ϣ,IP��ַ
        configIniWrite->setValue("/server/ip", "127.0.0.1");  
        //��ini�ļ��ĵ�һ����д�������������Ϣ,IP��ַ
        configIniWrite->setValue("server/port", "17024");  
        //д����ɺ�ɾ��ָ��   
        delete configIniWrite;  
    }
}

void CGlobalUtil::ReadIniFile()
{
    QString filePath=GetIniFilePath();
    if(QFile::exists(filePath))
    {
        //���������ļ���Ϣ
        QSettings *configIniRead = new QSettings(filePath, QSettings::IniFormat);  
        //����ȡ����ini�ļ�������QString�У���ȡֵ��Ȼ��ͨ��toString()����ת����QString����   
        serverIP = configIniRead->value("/server/ip").toString();
        serverPort = configIniRead->value("/server/port").toString();
        //��������ɺ�ɾ��ָ��   
        delete configIniRead;  
    }
}

QString CGlobalUtil::MD5(QString pwd)
{
    QString pwdmd5;  
    QByteArray bb;  
    bb = QCryptographicHash::hash ( pwd.toLocal8Bit(), QCryptographicHash::Md5 );
    pwdmd5.append(bb.toHex());  
    return pwdmd5;
}


int Power(int weight, int time)
{
    int ret = 1;
    for (int i = 1; i < time; i++) {
        ret *= weight;
    }
    return ret;
}


int BCD2INT(unsigned char * bcd, int bcdLen)
{
    int dec = 0;
    for (int i = 0; i < bcdLen; i++) {
        int tmp = ((bcd[i] >> 4) & 0x0F) * 10 + (bcd[i] & 0x0F);
        dec += (tmp * Power(100, (bcdLen - 1 - i)));
    }
    return dec;
}

double BCD2Double(unsigned char * bcd, int bcdLen, int decimalLen)
{
    if (decimalLen > bcdLen || decimalLen < 0) {
        return 0;
    }

    int val = BCD2INT(bcd, bcdLen);

    double power = 1;
    for (int i = 0; i < decimalLen; i++) {
        power *= 0.1;
    }

    return val * power;
}
double AveCalculatefromQVector(QVector<double> vt)
{
    double value = 0.0;
    for( int i = 0; i < vt.count(); ++i )
    {
        value += vt[i];
    }
    double ave = 0.0;
    if( vt.count() != 0 )
        ave = value / vt.count(); ////1451036981774.0000

    // H2Sȡ1λС����SO2��2λС����ƽ��ֵֻȡ2λС�� tangqiao 2015/12/25
    long long val= (long long)(ave * 100);   //-21474836.480000000
    ave = (double)val / 100;
    return ave;
}

double Convert7ByteDataToD64(unsigned char buf[7])
{
    double data = 0.0;
    
    if (buf[4] == 0x02)// 2λС��
    {
        data = ((buf[1] >>4) * 10 + (buf[1] & 0x0F)) * 100 + ((buf[2] >>4) * 10 + (buf[2] & 0x0F))  + ((buf[3] >>4) *10 + (buf[3] & 0x0F))*0.01;
    }
    else if (buf[4] == 0x01)// 1λС��
    {
        data = (buf[1] >>4)* 10000 + ( (buf[1] & 0x0F)*10 + (buf[2]>>4)) * 100 + ((buf[2] & 0x0F) * 10 + (buf[3] >>4)) + (buf[3] & 0x0F) * 0.1;
    }

    return data;
}


#ifdef USED_BY_H2SMONITORCLIENT_ONLY

double Max(QVector<double> &vec)
{
    double ret = vec.at(0);
    for (double data : vec)
    {
        if (data > ret)
        {
            ret = data;
        }
    }
 
    return ret;
}

void AveCalculator(XY_VALUE ProbeValue, int aveTime, XY_VALUE &out)
{
    QVector<double> vt_tm;
    QVector<double> vt_val;
    while( ProbeValue.x_value.count() > 0 )
    {
        double startTm = ProbeValue.x_value[0];
        double endTm = startTm + aveTime*60*1000;
        int icount = 0;
        vt_tm.clear();
        vt_val.clear();
        int k = 0;
        while( ProbeValue.x_value.count() > k )
        {
            if( ProbeValue.x_value[k] <= endTm )
            {
                vt_tm.push_back( ProbeValue.x_value[k] );
                vt_val.push_back(ProbeValue.y_value[k]);
                ++k;
            }
            else
            {
                //������һ������
                ProbeValue.x_value.remove(0, k);
                ProbeValue.y_value.remove(0, k);
                out.x_value.push_back( Max(vt_tm ));
                out.y_value.push_back( AveCalculatefromQVector(vt_val ));
                vt_tm.clear();
                vt_val.clear();
                k = 0;
                if( ProbeValue.x_value.count()!= 0 )
                {
                    startTm = ProbeValue.x_value[0];
                    endTm = startTm + aveTime*60*1000;
                }

            }
        }
        ProbeValue.x_value.clear();
        ProbeValue.y_value.clear();
        out.x_value.push_back( Max(vt_tm ));
        out.y_value.push_back( AveCalculatefromQVector(vt_val ));
    }
}




#endif

unsigned char U8toBCD(unsigned char u8)
{
    unsigned char ret = 0;
    // ȡʮλ����ͬʱ���Ե����ܴ��ڵİ�λ��
    unsigned char tens = (u8 / 10) % 10;
    // ȡ��λ��
    unsigned char singles = u8 % 10;
    ret = (tens << 4) | singles;
    return ret;
}

unsigned short U16toBCD(unsigned short u16)
{
    unsigned short ret = 0;
    unsigned short thousands, hundreds, tens, singles;

    // ȡǧλ��
    thousands = u16 / 1000;
    // ȡ��λ��
    hundreds = (u16 / 100) % 10;
    // ȡʮλ��
    tens = (u16 / 10) % 10;
    // ȡ��λ��
    singles = u16 % 10;

    ret = (thousands << 12) | (hundreds << 8) | (tens << 4) | singles;
    return ret;
}
