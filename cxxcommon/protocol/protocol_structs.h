/*****************************************************
** �ļ�����  protocol_structs.h
** ��  Ȩ��  XXX ��Ȩ����
** ��  ����  
** ���ݼ�����ͨ�Žṹ�Ķ���
** �������ڣ�2015/07/15
** �����ˣ�  ����
** �޸ļ�¼��
   ����        �汾      �޸���    �޸����� 
    
*****************************************************/

#ifndef _PROTOCOL_STRUCTS_H_
#define _PROTOCOL_STRUCTS_H_

#include "protocol_data_type.h"

// Э��汾�Ŷ��� 1.0.0.0
#define PROTOCOL_VERSION_1ST (0x1)
#define PROTOCOL_VERSION_2ND (0x0)
#define PROTOCOL_VERSION_3RD (0x0)
#define PROTOCOL_VERSION_4TH (0x0)

// ��ǰЭ��汾 
#define PROTOCOL_VERSION    ((PROTOCOL_VERSION_1ST << 31)\
    | (PROTOCOL_VERSION_2ND << 23)\
    | (PROTOCOL_VERSION_3RD << 15)\
    | (PROTOCOL_VERSION_4TH <<  7)) 

// Э��У��λ
#define PROTOCOL_VERIFY_BITS (~((U32)PROTOCOL_VERSION))


// ��ͨ��ʹ�õĽṹ�����4�ֽڶ���

// ��Ϣͷ (Э��汾��Э��У��λ�����,���ΪȫF����Ϊ��Ч)
typedef struct _T_MESSAGE_HEADER
{
    U32     cmd;                // ������
    U32     protocolVersion;    // Э��汾
    U32     protocolVerifyBits; // Э��У��λ
    U32     messageNo;          // ��Ϣ��ˮ��
    U32     cmdResult;          // ����ִ�н��(���������һ�������Ҫ������ִ��ʧ��ʱ��ȡ������Ϣ��������ʱ����ֵ��Ϊ1��������Ϊ0)
                                //            ���������ط�0ֵ��ʾִ��ʧ�ܣ���ͻ�������ʱҪ��ش�������Ϣ����Body����ΪT_STRUCT_ERROR_INFO
    U32     bodyLength;         // ��Ϣ�峤��
}T_MESSAGE_HEADER;


// ����ִ��ʧ�ܵĴ�����Ϣ
typedef struct _T_MSG_ERROR_INFO
{
    U32     infoBufLen;
    // U8 * infoBuf;
}T_MSG_ERROR_INFO;


// ��¼��Ϣ
typedef struct _T_MSG_LOGIN
{
    union
    {
        U16 accountLen;         // �˺ų���
        U16 deviceNoLen;        // �豸�ų���
    };
    U16     passwordLen;        // ���볤��
    // U8 * accountBuf;         // �˺Ż��豸��
    // u8 * passwordBuf;        // ����
}T_MSG_LOGIN;


// �豸�б�ظ�
typedef struct _T_MSG_DEVICES_LIST_REP
{
    U16      deviceCount;        // �豸���������֧�ַ���256
    // T_MSG_DEVICES_LIST_ITEM * pItems;// �����Ķ���豸���
}T_MSG_DEVICES_LIST_REP;

// �豸�б���
typedef struct _T_MSG_DEVICES_LIST_ITEM
{
    U8      deviceId[36];       // �豸��ϵͳ�еı�ţ������ݿ���Ϊuniqueidentifier���ͣ�16�ֽ�
    U8      deviceNo[24];       // �豸��ţ������ݿ���Ϊ������20���ֽ�
    U8      deviceName[52];     // �豸���ƣ������ݿ���Ϊ������50���ֽ�
    U8      warningLine1st[50]; // һ������ֵ
    U8      warningLine2nd[50]; // ��������ֵ
    U8      maxRange[50];       // ���ֵ
    U8      devType;            // �豸���� 1:��ЯʽH2S����� 2:�̶�ʽH2S�����
    U8      reserved;           // ����
    U32     sampleRateLen;      // �������ַ�������
    // U8 * sampleRate;
}T_MSG_DEVICES_LIST_ITEM;

// �豸ID
typedef struct _T_MSG_DEVICE_ID
{
    U8      deviceID[36];
}T_MSG_DEVICE_ID;

// �豸�б���������
typedef struct _T_MSG_DEVICES_INFO_LIST_REQ
{
    U8      deviceCount;        // ����������豸����
    //T_MSG_DEVICE_ID * deviceIds;// �����Ķ���豸Ψһ��ʶ
}T_MSG_DEVICES_INFO_LIST_REQ;

// �豸�б�����ظ�
typedef struct _T_MSG_DEVICES_INFO_LIST_REP
{
    U8      deviceCount;        // ���λظ��а������豸����
    // T_MSG_DEVICE_INFO_LIST_ITEM * pItems;// �����Ķ���豸�������
}T_MSG_DEVICES_INFO_LIST_REP;

// �豸�б�ڵ�����
typedef struct _T_MSG_DEVICE_INFO_ITEM
{
    U8      deviceId[36];               // �豸��ϵͳ�еı�ţ������ݿ���Ϊuniqueidentifier���ͣ�36���ַ�
    U8      deviceNo[24];               // �豸�룬�����ݿ���Ϊ������20���ֽ�
    U8      deviceName[52];             // �豸���ƣ������ݿ���Ϊ������50���ֽ�
    U8      deviceModel[20];            // �豸���ƣ������ݿ���Ϊ������50���ֽ�
    U8      deviceState[1];             // �豸���ƣ������ݿ���Ϊ������50���ֽ�
    U8      deviceManufacturers[255];   // �豸���ƣ������ݿ���Ϊ������50���ֽ�
    U8      deviceMainParams[255];      // �豸���ƣ������ݿ���Ϊ������50���ֽ�
    U8      deviceRigName[50];          // �豸���ƣ������ݿ���Ϊ������50���ֽ�
    U8      deviceCompany[200];         // �豸���ƣ������ݿ���Ϊ������50���ֽ�
    U8      deviceAddress[200];         // �豸���ƣ������ݿ���Ϊ������50���ֽ�
}T_MSG_DEVICE_INFO_LIST_ITEM;


// �ɼ��豸��������еĵ�������
typedef struct _T_MSG_DEVICE_REALTIME_DATA_ITEM
{
    U16     channelNo;          // ͨ����
    U16     datatype;           // ��������
    U8      data[7];            // �豸�ϴ���ԭʼ����
    U8      reserved;           // ����1�ֽ�,�������������豸���������
}T_MSG_DEVICE_REALTIME_DATA_ITEM;


// �ɼ��豸ʵʱ�������[�豸�ϱ�]
typedef struct _T_MSG_DEVICE_REALTIME_BATCHED_DATA
{
    U64     timestamp;          // ʱ��
    D64     longitude;          // ����
    D64     latitude;           // γ��
    U32     dataItemCount;      // ���������ݸ���
    // T_MSG_DEVICE_REALTIME_DATAITEM * pDataItems;// ����������б�
}T_MSG_DEVICE_REALTIME_BATCHED_DATA;


// �ɼ��豸ʵʱ����[�豸�ϱ�]
typedef struct _T_MSG_DEVICE_REALTIME_DATA
{
    U16     channelNo;          // ͨ����
    U16     datatype;           // ��������
    U64     timestamp;          // ʱ��
    D64     longitude;          // ����
    D64     latitude;           // γ��
    //D64     data;             // ����(���ҵ��豸�ᴫ7�ֽ����ݣ���λ��ԭ���ϱ������ֽڴ���)
    U8      reserved;           // ԭΪ�����ֽڣ�����������ʾ�豸������������λΪ1ʱ��ʾ�豸���ڳ�磬ʣ�µ�7λ��ʾ�����İٷֱ�
    U8      data_[7];           // ��λ���ϱ�������
}T_MSG_DEVICE_REALTIME_DATA;


// �㲥�豸�ɼ�����[����˷����ͻ���]
typedef struct _T_MSG_BROADCAST_DEVICE_REALTIME_DATA
{
    U8      rigId[36];          // ����ID
    U8      deviceId[36];       // �豸��
    D64     data;               // 
    T_MSG_DEVICE_REALTIME_DATA realtimeData;// �豸�ɼ�����
}T_MSG_BROADCAST_DEVICE_REALTIME_DATA;


// ������ʱ��
typedef struct _T_MSG_SERVER_TIME
{
    U64     serverTime;         // ������ʱ�䣬��ȷ�����룬��1970-01-01 00:00:00.000
}T_MSG_SERVER_TIME;

// �������ã���չ��Ϣ, �Ա㽫������ͬ�汾���֮��Ĺ��ܼ��ݣ�
typedef struct _T_MSG_EXTENSION
{
    U16     extensionID;        // ������չ���ܱ�ʶ
    U16     extensionInfoLen;   // ��չ��Ϣ��������
    // U8 * extensionInfo;      // �������չ��Ϣ����
}T_MSG_EXTENSION;


// �豸������PC�ͻ������ã�
typedef struct _T_MSG_DEVICE_PARAM
{
    U8      deviceID[36];
    U8      warningLine1st[50]; // һ������ֵ
    U8      warningLine2nd[50]; // ��������ֵ
    U8      maxRange[50];       // ���ֵ
    U8      reserved[2];        // ����
    U32     deviceNameLen;      // �豸������
    U32     sampleRateLen;      // �������ַ�������
    // U8 * deviceName;         // �豸���ַ���
    // U8 * sampleRate;         // �������ַ���
}T_MSG_DEVICE_PARAM;


// ��ȡ�豸����״̬����
typedef struct _T_MSG_DEVICE_ONLINE_STATE_REQ
{
    U16     deviceCount;            // ���������ȡ����״̬���豸����
    // T_MSG_DEVICE_ID * pDeviceIDs;// �豸ID�б�
}T_MSG_DEVICE_ONLINE_STATE_REQ;


// ��ȡ�豸����״̬����ظ�
#define DEVICE_ONLINE_STATUS_ONLINE          0x01 /*�豸���� */
#define DEVICE_ONLINE_STATUS_OFFLINE         0x02 /*�豸����*/
#define DEVICE_ONLINE_STATUS_INVALID_DEVICE  0x03 /*������豸ID��Ч*/

typedef struct _T_MSG_DEVICE_ONLINE_STATUS_REP_ITEM
{
    U8      deviceID[36];       // �豸ID
    U8      deviceOnlineStatus; // �豸����״̬
}T_MSG_DEVICE_ONLINE_STATUS_REP_ITEM;

typedef struct _T_MSG_DEVICE_ONLINE_STATUS_REP
{
    U16             itemCount;                      // ���ص�״̬�ڵ���
    //T_MSG_DEVICE_ONLINE_STATUS_REP_ITEM * pItems;  // �豸״̬�ڵ��б�
}T_MSG_DEVICE_ONLINE_STATUS_REP;


typedef struct _T_MSG_HEARTBEAT
{
    U64     heartbeatTime;      // �������ĵ�ǰʱ��
}T_MSG_HEARTBEAT;


typedef struct _T_MSG_SERVER_TIME_BEIJING
{
    U16     year;               // ��(��Ԫ0~65535��)
    U8      month;              // ��(1~12)
    U8      day;                // ��(1~31)
    U8      hour;               // ʱ(0~23)
    U8      minute;             // ��(0~59)
    U8      second;             // ��(0~59)
    U8      reserved;           // ���Ӵ˳�Ա,����4�ֽڶ���
}T_MSG_SERVER_TIME_BEIJING;// �������ı���ʱ���ʾ


typedef struct _T_MSG_GET_DEVICE_DATA_COUNT_REQ
{
    U64     begin;              // ��ʼʱ��
    U64     end;                // ����ʱ��(�����뿪ʼʱ����ͬһ����)
    U32     deviceIDLen;        // �豸ID����
    U32     reserved;           // ����
    // U8 * deviceID;           // �豸ID��Ϊ��ʱ��ʾ���û����ھ��ӵ������豸
}T_MSG_GET_DEVICE_DATA_COUNT_REQ;// ��ȡ�豸���ݵ�������


typedef struct _T_MSG_GET_DEVICE_DATA_COUNT_REP
{
    U32     deivceDataCount;    // �豸���ݵ�����
}T_MSG_GET_DEVICE_DATA_COUNT_REP;// �ظ��豸���ݵ�����


typedef struct _T_MSG_GET_DEVICE_DATA_REQ
{
    U64     begin;              // ��ʼʱ��
    U64     end;                // ����ʱ��(�����뿪ʼʱ����ͬһ����)
    U32     pageIndex;          // ��ҳ��,��1��ʼ����
    U32     rowsOfPerPage;      // ÿһҳ����������������20��50��100
    U32     deviceIDLen;        // �豸ID����
    U32     reserved;           // ����
    // U8 * deviceID;           // �豸ID��Ϊ��ʱ��ʾ���û����ھ��ӵ������豸
}T_MSG_GET_DEVICE_DATA_REQ;// ��ȡ�豸���ݵ�����


typedef struct _T_MSG_DEVICE_DATA_ITEM
{
    D64     data;                   // �豸�ɼ���������
    U32     deviceIDLen;            // �豸ID�ų���
    // T_MSG_DEVICE_REALTIME_DATA detailData;// ��ϸ����
    // U8 * deviceID;               // �豸ID
}T_MSG_DEVICE_DATA_ITEM;// �豸��������


typedef struct _T_MSG_GET_DEVICE_DATA_REP
{
    U32     pageIndex;              // ��ǰ���ݵķ�ҳ��
    U32     rowsOfCurrentPage;      // ��ǰҳ����������
    //T_MSG_DEVICE_DATA_ITEM * deviceDataArray;// �豸�ɼ���������
}T_MSG_GET_DEVICE_DATA_REP;// �ظ�


#endif//!_PROTOCOL_STRUCTS_H_