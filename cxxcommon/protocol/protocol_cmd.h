/*****************************************************
** 文件名：  protocol_cmd.h
** 版  权：  XXX 版权所有
** 版  本：  
** 内容简述：通信命令码定义，包括（PC客户端-PC服务端、终端采集设备-PC服务端）
** 创建日期：2015/07/15
** 创建人：  唐桥
** 修改记录：
   日期        版本      修改人    修改内容 
    
*****************************************************/
#ifndef _PROTOCOL_CMD_H_
#define _PROTOCOL_CMD_H_

#include "protocol_data_type.h"

//   31        23          15        7       0  
//   |         |           |         |       |  
// +-------------------------------------------—+
// | 0000 0000 0000 00 0 0 0000 0000 0000 0000 |
// |               |  | | |                    |
// |               |  | | |                    |
// +---------------+--+-+-+---------------------+
//         |        |  | |         |            
//      保留12位     |  | |         |            
//         客户端类型2位 | |         |            
//             回复标识1位 |         |            
//                   通知标识1位     |            
//                             通用命令码16位     
// 命令由32位组成，
// 第0~15位，占16位，通用命令码
// 第16位，占1位，通知标识位（由服务器主动发出，非回复客户端时置1，默认置为0）
// 第17位，占1位，回复标识位（由服务器回复客户端时置1，默认置为0）
// 第18~19位，占2位，客户端类型位（PC服务端：0 PC客户端：1 采集设备：2）
// 第20~31位，占12位，保留备用


//////////////////////////////////////////////////////////////////////////
// 命令偏移位定义
#define CMD_NOTIFY_BIT_OFFSET       16 /* 服务器主动通知标识位偏移 */
#define CMD_REP_BIT_OFFSET          17 /* 服务器回复标识位偏移 */
#define CMD_CLIENT_TYPE_BIT_OFFSET  18 /* 客户端类型标识位偏移 */


//////////////////////////////////////////////////////////////////////////
// 客户端类型定义
#define CLIENT_TYPE_PC_SERVER       0x00 /* PC服务器 */
#define CLIENT_TYPE_PC_CLIENT       0x01 /* PC客户端 */
#define CLIENT_TYPE_TERMINAL_DEVICE 0x02 /* 终端采集设备 */


//////////////////////////////////////////////////////////////////////////
// 命令实用宏定义
#define SET_CLIENT_TYPE(cmd, type)  (((U32)cmd & (0x03FFFF)) | ((type & 0x03) << CMD_CLIENT_TYPE_BIT_OFFSET)) /* 设置客户端类型 */
#define SET_PC_CLIENT_CMD_BIT(cmd) (SET_CLIENT_TYPE(cmd, CLIENT_TYPE_PC_CLIENT))
#define SET_PC_SERVER_CMD_BIT(cmd)  (SET_CLIENT_TYPE(cmd, CLIENT_TYPE_PC_SERVER))
#define SET_DEVICE_BIT(cmd)         (SET_CLIENT_TYPE(cmd, CLIENT_TYPE_TERMINAL_DEVICE))


#define SET_NOTIFY_BIT(cmd) (((U32)cmd | (0x01 << CMD_NOTIFY_BIT_OFFSET)))      /* 设置通知标识位为1，PC服务端才设置此位*/
#define SET_REP_BIT(cmd)    (((U32)cmd | (0x01 << CMD_REP_BIT_OFFSET)))         /* 设置回复标识位为1，PC服务端才设置此位*/
#define IS_NOTIFY_CMD(cmd)  ((U32)cmd & (0x01 << CMD_NOTIFY_BIT_OFFSET))        /* 命令为通知命令时结果为非0，否则为0 */
#define IS_REP_CMD(cmd)     ((U32)cmd & (0x01 << CMD_REP_BIT_OFFSET))           /* 命令为回复命令时结果为非0，否则为0 */
#define GET_COMMON_CMD(cmd) ((U32)cmd & 0x0FFFF)                                /* 获取通用命令码 */
#define GET_CLIENT_TYPE(cmd) (((U32)cmd >> CMD_CLIENT_TYPE_BIT_OFFSET) & 0x03)  /* 获取客户端类型 */

#define IS_FROM_PC_SERVER(cmd) (CLIENT_TYPE_PC_SERVER == GET_CLIENT_TYPE(cmd)) /* 是否为PC服务器 */
#define IS_FROM_PC_CLIENT(cmd) (CLIENT_TYPE_PC_CLIENT == GET_CLIENT_TYPE(cmd)) /* 是否为PC客户端 */
#define IS_FROM_DEVICE(cmd)    (CLIENT_TYPE_TERMINAL_DEVICE == GET_CLIENT_TYPE(cmd)) /* 是否为终端采集设备 */


//////////////////////////////////////////////////////////////////////////
// 通用命令码定义
#define COMMON_CMD_EXTENSION                        0x0000 /* 含义:扩展命令 发送端:服务端 客户端 采集设备 发送结构:T_MSG_EXTENSION 回复结构:根据具体的扩展功能而定 */
#define COMMON_CMD_LOGIN                            0x0001 /* 含义:登录命令 发送端:客户端 发送结构:T_MSG_LOGIN 回复结构:T_MSG_SERVER_TIME */
#define COMMON_CMD_GET_DEVICE_LIST                  0x0002 /* 含义:获取设备列表 发送端:客户端 发送结构:无 回复结构:T_MSG_DEVICES_LIST_REP */
#define COMMON_CMD_GET_DEVICE_INFO                  0x0003 /* 含义:获取设备信息 发送端:客户端 发送结构: 回复结构: */
#define COMMON_CMD_REALTIME_DATA                    0x0004 /* 含义:服务器广播设备采集到的实时数据 发送端: PC服务端、采集设备 发送结构:T_MSG_DEVICE_REALTIME_DATA或T_MSG_BROADCAST_DEVICE_REALTIME_DATA 回复结构:无 */
#define COMMON_CMD_EXIT                             0x0005 /* 含义:退出 发送端: PC服务端或客户端 发送结构:无 回复结构:无 */
#define COMMON_CMD_GET_DEVICE_STATUS                0x0006 /* 含义:查询设备在线状态 发送端: PC客户端 发送结构:无 回复结构:无 */
#define COMMON_CMD_SET_DEVICE_PARAM                 0x0007 /* 含义:设置设备参数（一级报警值、二级报警值、最大值） 发送端: PC客户端 发送结构: T_MSG_DEVICE_PARAM 回复结构:无 */
#define COMMON_CMD_SEND_HEARTBEAT                   0x0008 /* 含义:发送心跳 发送端: PC客户端 发送结构:无 回复结构:T_MSG_HEARTBEAT */
#define COMMON_CMD_GET_HISTORY_DEVICE_DATA_COUNT    0x0009 /* 含义:获取历史设备数据条数 发送端: PC客户端 发送结构:T_MSG_GET_DEVICE_DATA_COUNT_REQ 回复结构:T_MSG_GET_DEVICE_DATA_COUNT_REP */
#define COMMON_CMD_GET_HISTORY_DEVICE_DATA          0x000A /* 含义:获取历史设备数据 发送端: PC客户端 发送结构:T_MSG_GET_DEVICE_DATA_REQ 回复结构:T_MSG_GET_DEVICE_DATA_REP */
#define COMMON_CMD_REALTIME_BATCHED_DATA            0x000B /* 含义:批量发送 发送端: PC客户端 发送结构:T_MSG_GET_DEVICE_DATA_REQ 回复结构:T_MSG_GET_DEVICE_DATA_REP */
//20180122 新增
#define COMMON_CMD_REALTIME_BATCHED_DATA_NEW           0x000C /* 含义:批量发送 */

//////////////////////////////////////////////////////////////////////////
// PC客户端主动发出的请求命令
#define PC_CLIENT_CMD_LOGIN                         (SET_PC_CLIENT_CMD_BIT(COMMON_CMD_LOGIN))           /*PC客户端登录*/
#define PC_CLIENT_CMD_GET_DEVICE_LIST               (SET_PC_CLIENT_CMD_BIT(COMMON_CMD_GET_DEVICE_LIST)) /*获取设备列表*/
#define PC_CLIENT_CMD_GET_DEVICE_INFO               (SET_PC_CLIENT_CMD_BIT(COMMON_CMD_GET_DEVICE_INFO)) /*获取设备详情*/
#define PC_CLIENT_CMD_EXIT                          (SET_PC_CLIENT_CMD_BIT(COMMON_CMD_EXIT))            /*PC客户端退出*/
#define PC_CLIENT_CMD_GET_DEVICE_STATUS             (SET_PC_CLIENT_CMD_BIT(COMMON_CMD_GET_DEVICE_STATUS))/*PC客户端获取指定设备的在线状态*/
#define PC_CLIENT_CMD_SET_DEVICE_PARAM              (SET_PC_CLIENT_CMD_BIT(COMMON_CMD_SET_DEVICE_PARAM)) /*设置设备参数*/
#define PC_CLIENT_CMD_SEND_HEARTBEAT                (SET_PC_CLIENT_CMD_BIT(COMMON_CMD_SEND_HEARTBEAT))   /* 发送心跳 */
#define PC_CLIENT_CMD_GET_HISTORY_DEVICE_DATA_COUNT (SET_PC_CLIENT_CMD_BIT(COMMON_CMD_GET_HISTORY_DEVICE_DATA_COUNT))/*获取历史设备数据条数*/
#define PC_CLIENT_CMD_GET_HISTORY_DEVICE_DATA       (SET_PC_CLIENT_CMD_BIT(COMMON_CMD_GET_HISTORY_DEVICE_DATA)) /*获取历史设备数据*/

//////////////////////////////////////////////////////////////////////////
// 采集设备主动发出的请求命令
#define DEVICE_CMD_LOGIN                                (SET_DEVICE_BIT(COMMON_CMD_LOGIN))        /*设备登录*/
#define DEVICE_CMD_EXIT                                 (SET_DEVICE_BIT(COMMON_CMD_EXIT))         /*设备退出*/
#define DEVICE_CMD_REALTIME_DATA                        (SET_DEVICE_BIT(COMMON_CMD_REALTIME_DATA))/*设备上传实时采集数据*/
#define DEVICE_CMD_SEND_HEARTBEAT                       (SET_DEVICE_BIT(COMMON_CMD_SEND_HEARTBEAT))/*设备发送心跳，服务器返回服务器时间*/
#define DEVICE_CMD_REALTIME_BATCHED_DATA                (SET_DEVICE_BIT(COMMON_CMD_REALTIME_BATCHED_DATA))/*设备批量上传采集数据*/
//20180122 新增
#define DEVICE_CMD_REALTIME_BATCHED_DATA_NEW               (SET_DEVICE_BIT(COMMON_CMD_REALTIME_BATCHED_DATA_NEW))/*新协议设备批量上传采集数据*/
//////////////////////////////////////////////////////////////////////////
// PC服务端发出的命令
#define PC_SERVER_CMD_LOGIN_REP                         (SET_PC_SERVER_CMD_BIT(SET_REP_BIT(COMMON_CMD_LOGIN)))              /*登录回复*/
#define PC_SERVER_CMD_GET_DEVICE_LIST_REP               (SET_PC_SERVER_CMD_BIT(SET_REP_BIT(COMMON_CMD_GET_DEVICE_LIST)))    /*获取设备列表回复*/
#define PC_SERVER_CMD_GET_DEVICES_INFO_REP              (SET_PC_SERVER_CMD_BIT(SET_REP_BIT(COMMON_CMD_GET_DEVICE_INFO)))    /*获取设备详情回复*/
#define PC_SERVER_CMD_GET_DEVICE_STATUS_REP             (SET_PC_SERVER_CMD_BIT(SET_REP_BIT(COMMON_CMD_GET_DEVICE_STATUS)))  /*获取设备在线状态回复*/
#define PC_SERVER_CMD_SET_DEVICE_PARAM_REP              (SET_PC_SERVER_CMD_BIT(SET_REP_BIT(COMMON_CMD_SET_DEVICE_PARAM)))   /*设置设备参数回复*/
#define PC_SERVER_CMD_SEND_HEARTBEAT_REP                (SET_PC_SERVER_CMD_BIT(SET_REP_BIT(COMMON_CMD_SEND_HEARTBEAT)))     /*发送心跳回复 */
#define PC_SERVER_CMD_GET_HISTORY_DEVICE_DATA_COUNT_REP (SET_PC_SERVER_CMD_BIT(SET_REP_BIT(COMMON_CMD_GET_HISTORY_DEVICE_DATA_COUNT)))/*获取历史设备数据条数回复*/
#define PC_SERVER_CMD_GET_HISTORY_DEVICE_DATA_REP       (SET_PC_SERVER_CMD_BIT(SET_REP_BIT(COMMON_CMD_GET_HISTORY_DEVICE_DATA)))/*获取历史设备数据回复*/
#define PC_SERVER_CMD_REALTIME_BATCHED_DATA_REP         (SET_PC_SERVER_CMD_BIT(SET_REP_BIT(COMMON_CMD_REALTIME_BATCHED_DATA)))   /*回复设备批量上传实时采集数据的结果*/
// PC服务端广播命令
#define PC_SERVER_CMD_BROADCAST_REALTIME_DATA           (SET_PC_SERVER_CMD_BIT(SET_NOTIFY_BIT(COMMON_CMD_REALTIME_DATA)))   /*广播设备实时采集数据到客户端*/
#define PC_SERVER_CMD_BROADCAST_DEVICE_STATUS           (SET_PC_SERVER_CMD_BIT(SET_NOTIFY_BIT(COMMON_CMD_GET_DEVICE_STATUS)))/*广播设备在线状态变化*/


#endif//!_PROTOCOL_CMD_H_