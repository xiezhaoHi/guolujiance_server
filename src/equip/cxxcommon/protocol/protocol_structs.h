/*****************************************************
** 文件名：  protocol_structs.h
** 版  权：  XXX 版权所有
** 版  本：  
** 内容简述：通信结构的定义
** 创建日期：2015/07/15
** 创建人：  唐桥
** 修改记录：
   日期        版本      修改人    修改内容 
    
*****************************************************/

#ifndef _PROTOCOL_STRUCTS_H_
#define _PROTOCOL_STRUCTS_H_

#include "protocol_data_type.h"

// 协议版本号定义 1.0.0.0
#define PROTOCOL_VERSION_1ST (0x1)
#define PROTOCOL_VERSION_2ND (0x0)
#define PROTOCOL_VERSION_3RD (0x0)
#define PROTOCOL_VERSION_4TH (0x0)

// 当前协议版本 
#define PROTOCOL_VERSION    ((PROTOCOL_VERSION_1ST << 31)\
    | (PROTOCOL_VERSION_2ND << 23)\
    | (PROTOCOL_VERSION_3RD << 15)\
    | (PROTOCOL_VERSION_4TH <<  7)) 

// 协议校验位
#define PROTOCOL_VERIFY_BITS (~((U32)PROTOCOL_VERSION))


// 各通信使用的结构体采用4字节对齐

// 消息头 (协议版本与协议校验位作异或,结果为全F则认为有效)
typedef struct _T_MESSAGE_HEADER
{
    U32     cmd;                // 命令码
    U32     protocolVersion;    // 协议版本
    U32     protocolVerifyBits; // 协议校验位
    U32     messageNo;          // 消息流水号
    U32     cmdResult;          // 命令执行结果(发送请求的一方如果需要在命令执行失败时获取错误信息，则请求时将此值置为1，否则置为0)
                                //            服务器返回非0值表示执行失败，如客户端请求时要求回传错误信息，则Body部分为T_STRUCT_ERROR_INFO
    U32     bodyLength;         // 消息体长度
}T_MESSAGE_HEADER;


// 命令执行失败的错误信息
typedef struct _T_MSG_ERROR_INFO
{
    U32     infoBufLen;
    // U8 * infoBuf;
}T_MSG_ERROR_INFO;


// 登录消息
typedef struct _T_MSG_LOGIN
{
    union
    {
        U16 accountLen;         // 账号长度
        U16 deviceNoLen;        // 设备号长度
    };
    U16     passwordLen;        // 密码长度
    // U8 * accountBuf;         // 账号或设备号
    // u8 * passwordBuf;        // 密码
}T_MSG_LOGIN;


// 设备列表回复
typedef struct _T_MSG_DEVICES_LIST_REP
{
    U16      deviceCount;        // 设备计数，最多支持返回256
    // T_MSG_DEVICES_LIST_ITEM * pItems;// 连续的多个设备结点
}T_MSG_DEVICES_LIST_REP;

// 设备列表结点
typedef struct _T_MSG_DEVICES_LIST_ITEM
{
    U8      deviceId[36];       // 设备在系统中的编号，在数据库中为uniqueidentifier类型，16字节
    U8      deviceNo[24];       // 设备编号，在数据库中为不超过20个字节
    U8      deviceName[52];     // 设备名称，在数据库中为不超过50个字节
    U8      warningLine1st[50]; // 一级报警值
    U8      warningLine2nd[50]; // 二级报警值
    U8      maxRange[50];       // 最大值
    U8      devType;            // 设备类型 1:便携式H2S检测仪 2:固定式H2S检测仪
    U8      reserved;           // 保留
    U32     sampleRateLen;      // 采样率字符串长度
    // U8 * sampleRate;
}T_MSG_DEVICES_LIST_ITEM;

// 设备ID
typedef struct _T_MSG_DEVICE_ID
{
    U8      deviceID[36];
}T_MSG_DEVICE_ID;

// 设备列表详情请求
typedef struct _T_MSG_DEVICES_INFO_LIST_REQ
{
    U8      deviceCount;        // 本次请求的设备计数
    //T_MSG_DEVICE_ID * deviceIds;// 连续的多个设备唯一标识
}T_MSG_DEVICES_INFO_LIST_REQ;

// 设备列表详情回复
typedef struct _T_MSG_DEVICES_INFO_LIST_REP
{
    U8      deviceCount;        // 本次回复中包含的设备计数
    // T_MSG_DEVICE_INFO_LIST_ITEM * pItems;// 连续的多个设备结点详情
}T_MSG_DEVICES_INFO_LIST_REP;

// 设备列表节点详情
typedef struct _T_MSG_DEVICE_INFO_ITEM
{
    U8      deviceId[36];               // 设备在系统中的编号，在数据库中为uniqueidentifier类型，36个字符
    U8      deviceNo[24];               // 设备码，在数据库中为不超过20个字节
    U8      deviceName[52];             // 设备名称，在数据库中为不超过50个字节
    U8      deviceModel[20];            // 设备名称，在数据库中为不超过50个字节
    U8      deviceState[1];             // 设备名称，在数据库中为不超过50个字节
    U8      deviceManufacturers[255];   // 设备名称，在数据库中为不超过50个字节
    U8      deviceMainParams[255];      // 设备名称，在数据库中为不超过50个字节
    U8      deviceRigName[50];          // 设备名称，在数据库中为不超过50个字节
    U8      deviceCompany[200];         // 设备名称，在数据库中为不超过50个字节
    U8      deviceAddress[200];         // 设备名称，在数据库中为不超过50个字节
}T_MSG_DEVICE_INFO_LIST_ITEM;


// 采集设备打包数据中的单个数据
typedef struct _T_MSG_DEVICE_REALTIME_DATA_ITEM
{
    U16     channelNo;          // 通道号
    U16     datatype;           // 数据类型
    U8      data[7];            // 设备上传的原始数据
    U8      reserved;           // 保留1字节,现在里面存放着设备电量情况，
}T_MSG_DEVICE_REALTIME_DATA_ITEM;


// 采集设备实时打包数据[设备上报]
typedef struct _T_MSG_DEVICE_REALTIME_BATCHED_DATA
{
    U64     timestamp;          // 时间
    D64     longitude;          // 经度
    D64     latitude;           // 纬度
    U32     dataItemCount;      // 包含的数据个数
    // T_MSG_DEVICE_REALTIME_DATAITEM * pDataItems;// 具体的数据列表
}T_MSG_DEVICE_REALTIME_BATCHED_DATA;


// 采集设备实时数据[设备上报]
typedef struct _T_MSG_DEVICE_REALTIME_DATA
{
    U16     channelNo;          // 通道号
    U16     datatype;           // 数据类型
    U64     timestamp;          // 时间
    D64     longitude;          // 经度
    D64     latitude;           // 纬度
    //D64     data;             // 数据(厂家的设备会传7字节数据，下位机原样上报，首字节传空)
    U8      reserved;           // 原为保留字节，现在用它表示设备电量情况，最高位为1时表示设备正在充电，剩下的7位表示电量的百分比
    U8      data_[7];           // 下位机上报的数据
}T_MSG_DEVICE_REALTIME_DATA;


// 广播设备采集数据[服务端发往客户端]
typedef struct _T_MSG_BROADCAST_DEVICE_REALTIME_DATA
{
    U8      rigId[36];          // 井队ID
    U8      deviceId[36];       // 设备号
    D64     data;               // 
    T_MSG_DEVICE_REALTIME_DATA realtimeData;// 设备采集数据
}T_MSG_BROADCAST_DEVICE_REALTIME_DATA;


// 服务器时间
typedef struct _T_MSG_SERVER_TIME
{
    U64     serverTime;         // 服务器时间，精确到毫秒，自1970-01-01 00:00:00.000
}T_MSG_SERVER_TIME;

// 保留备用（扩展消息, 以便将来处理不同版本软件之间的功能兼容）
typedef struct _T_MSG_EXTENSION
{
    U16     extensionID;        // 具体扩展功能标识
    U16     extensionInfoLen;   // 扩展消息参数长度
    // U8 * extensionInfo;      // 具体的扩展消息参数
}T_MSG_EXTENSION;


// 设备参数（PC客户端设置）
typedef struct _T_MSG_DEVICE_PARAM
{
    U8      deviceID[36];
    U8      warningLine1st[50]; // 一级报警值
    U8      warningLine2nd[50]; // 二级报警值
    U8      maxRange[50];       // 最大值
    U8      reserved[2];        // 保留
    U32     deviceNameLen;      // 设备名长度
    U32     sampleRateLen;      // 采样率字符串长度
    // U8 * deviceName;         // 设备名字符串
    // U8 * sampleRate;         // 采样率字符串
}T_MSG_DEVICE_PARAM;


// 获取设备在线状态请求
typedef struct _T_MSG_DEVICE_ONLINE_STATE_REQ
{
    U16     deviceCount;            // 本次请求获取在线状态的设备个数
    // T_MSG_DEVICE_ID * pDeviceIDs;// 设备ID列表
}T_MSG_DEVICE_ONLINE_STATE_REQ;


// 获取设备在线状态请求回复
#define DEVICE_ONLINE_STATUS_ONLINE          0x01 /*设备在线 */
#define DEVICE_ONLINE_STATUS_OFFLINE         0x02 /*设备离线*/
#define DEVICE_ONLINE_STATUS_INVALID_DEVICE  0x03 /*请求的设备ID无效*/

typedef struct _T_MSG_DEVICE_ONLINE_STATUS_REP_ITEM
{
    U8      deviceID[36];       // 设备ID
    U8      deviceOnlineStatus; // 设备在线状态
}T_MSG_DEVICE_ONLINE_STATUS_REP_ITEM;

typedef struct _T_MSG_DEVICE_ONLINE_STATUS_REP
{
    U16             itemCount;                      // 返回的状态节点数
    //T_MSG_DEVICE_ONLINE_STATUS_REP_ITEM * pItems;  // 设备状态节点列表
}T_MSG_DEVICE_ONLINE_STATUS_REP;


typedef struct _T_MSG_HEARTBEAT
{
    U64     heartbeatTime;      // 服务器的当前时间
}T_MSG_HEARTBEAT;


typedef struct _T_MSG_SERVER_TIME_BEIJING
{
    U16     year;               // 年(公元0~65535年)
    U8      month;              // 月(1~12)
    U8      day;                // 日(1~31)
    U8      hour;               // 时(0~23)
    U8      minute;             // 分(0~59)
    U8      second;             // 秒(0~59)
    U8      reserved;           // 增加此成员,用于4字节对齐
}T_MSG_SERVER_TIME_BEIJING;// 服务器的北京时间表示


typedef struct _T_MSG_GET_DEVICE_DATA_COUNT_REQ
{
    U64     begin;              // 开始时间
    U64     end;                // 结束时间(必须与开始时间在同一天内)
    U32     deviceIDLen;        // 设备ID长度
    U32     reserved;           // 保留
    // U8 * deviceID;           // 设备ID，为空时表示本用户所在井队的所有设备
}T_MSG_GET_DEVICE_DATA_COUNT_REQ;// 获取设备数据的总条数


typedef struct _T_MSG_GET_DEVICE_DATA_COUNT_REP
{
    U32     deivceDataCount;    // 设备数据的条数
}T_MSG_GET_DEVICE_DATA_COUNT_REP;// 回复设备数据的条数


typedef struct _T_MSG_GET_DEVICE_DATA_REQ
{
    U64     begin;              // 开始时间
    U64     end;                // 结束时间(必须与开始时间在同一天内)
    U32     pageIndex;          // 分页号,从1开始计数
    U32     rowsOfPerPage;      // 每一页的数据条数，比如20、50、100
    U32     deviceIDLen;        // 设备ID长度
    U32     reserved;           // 保留
    // U8 * deviceID;           // 设备ID，为空时表示本用户所在井队的所有设备
}T_MSG_GET_DEVICE_DATA_REQ;// 获取设备数据的请求


typedef struct _T_MSG_DEVICE_DATA_ITEM
{
    D64     data;                   // 设备采集到的数据
    U32     deviceIDLen;            // 设备ID号长度
    // T_MSG_DEVICE_REALTIME_DATA detailData;// 详细数据
    // U8 * deviceID;               // 设备ID
}T_MSG_DEVICE_DATA_ITEM;// 设备数据详情


typedef struct _T_MSG_GET_DEVICE_DATA_REP
{
    U32     pageIndex;              // 当前数据的分页号
    U32     rowsOfCurrentPage;      // 当前页的数据条数
    //T_MSG_DEVICE_DATA_ITEM * deviceDataArray;// 设备采集到的数据
}T_MSG_GET_DEVICE_DATA_REP;// 回复


#endif//!_PROTOCOL_STRUCTS_H_