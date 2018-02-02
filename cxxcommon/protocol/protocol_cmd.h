/*****************************************************
** �ļ�����  protocol_cmd.h
** ��  Ȩ��  XXX ��Ȩ����
** ��  ����  
** ���ݼ�����ͨ�������붨�壬������PC�ͻ���-PC����ˡ��ն˲ɼ��豸-PC����ˣ�
** �������ڣ�2015/07/15
** �����ˣ�  ����
** �޸ļ�¼��
   ����        �汾      �޸���    �޸����� 
    
*****************************************************/
#ifndef _PROTOCOL_CMD_H_
#define _PROTOCOL_CMD_H_

#include "protocol_data_type.h"

//   31        23          15        7       0  
//   |         |           |         |       |  
// +-------------------------------------------��+
// | 0000 0000 0000 00 0 0 0000 0000 0000 0000 |
// |               |  | | |                    |
// |               |  | | |                    |
// +---------------+--+-+-+---------------------+
//         |        |  | |         |            
//      ����12λ     |  | |         |            
//         �ͻ�������2λ | |         |            
//             �ظ���ʶ1λ |         |            
//                   ֪ͨ��ʶ1λ     |            
//                             ͨ��������16λ     
// ������32λ��ɣ�
// ��0~15λ��ռ16λ��ͨ��������
// ��16λ��ռ1λ��֪ͨ��ʶλ���ɷ����������������ǻظ��ͻ���ʱ��1��Ĭ����Ϊ0��
// ��17λ��ռ1λ���ظ���ʶλ���ɷ������ظ��ͻ���ʱ��1��Ĭ����Ϊ0��
// ��18~19λ��ռ2λ���ͻ�������λ��PC����ˣ�0 PC�ͻ��ˣ�1 �ɼ��豸��2��
// ��20~31λ��ռ12λ����������


//////////////////////////////////////////////////////////////////////////
// ����ƫ��λ����
#define CMD_NOTIFY_BIT_OFFSET       16 /* ����������֪ͨ��ʶλƫ�� */
#define CMD_REP_BIT_OFFSET          17 /* �������ظ���ʶλƫ�� */
#define CMD_CLIENT_TYPE_BIT_OFFSET  18 /* �ͻ������ͱ�ʶλƫ�� */


//////////////////////////////////////////////////////////////////////////
// �ͻ������Ͷ���
#define CLIENT_TYPE_PC_SERVER       0x00 /* PC������ */
#define CLIENT_TYPE_PC_CLIENT       0x01 /* PC�ͻ��� */
#define CLIENT_TYPE_TERMINAL_DEVICE 0x02 /* �ն˲ɼ��豸 */


//////////////////////////////////////////////////////////////////////////
// ����ʵ�ú궨��
#define SET_CLIENT_TYPE(cmd, type)  (((U32)cmd & (0x03FFFF)) | ((type & 0x03) << CMD_CLIENT_TYPE_BIT_OFFSET)) /* ���ÿͻ������� */
#define SET_PC_CLIENT_CMD_BIT(cmd) (SET_CLIENT_TYPE(cmd, CLIENT_TYPE_PC_CLIENT))
#define SET_PC_SERVER_CMD_BIT(cmd)  (SET_CLIENT_TYPE(cmd, CLIENT_TYPE_PC_SERVER))
#define SET_DEVICE_BIT(cmd)         (SET_CLIENT_TYPE(cmd, CLIENT_TYPE_TERMINAL_DEVICE))


#define SET_NOTIFY_BIT(cmd) (((U32)cmd | (0x01 << CMD_NOTIFY_BIT_OFFSET)))      /* ����֪ͨ��ʶλΪ1��PC����˲����ô�λ*/
#define SET_REP_BIT(cmd)    (((U32)cmd | (0x01 << CMD_REP_BIT_OFFSET)))         /* ���ûظ���ʶλΪ1��PC����˲����ô�λ*/
#define IS_NOTIFY_CMD(cmd)  ((U32)cmd & (0x01 << CMD_NOTIFY_BIT_OFFSET))        /* ����Ϊ֪ͨ����ʱ���Ϊ��0������Ϊ0 */
#define IS_REP_CMD(cmd)     ((U32)cmd & (0x01 << CMD_REP_BIT_OFFSET))           /* ����Ϊ�ظ�����ʱ���Ϊ��0������Ϊ0 */
#define GET_COMMON_CMD(cmd) ((U32)cmd & 0x0FFFF)                                /* ��ȡͨ�������� */
#define GET_CLIENT_TYPE(cmd) (((U32)cmd >> CMD_CLIENT_TYPE_BIT_OFFSET) & 0x03)  /* ��ȡ�ͻ������� */

#define IS_FROM_PC_SERVER(cmd) (CLIENT_TYPE_PC_SERVER == GET_CLIENT_TYPE(cmd)) /* �Ƿ�ΪPC������ */
#define IS_FROM_PC_CLIENT(cmd) (CLIENT_TYPE_PC_CLIENT == GET_CLIENT_TYPE(cmd)) /* �Ƿ�ΪPC�ͻ��� */
#define IS_FROM_DEVICE(cmd)    (CLIENT_TYPE_TERMINAL_DEVICE == GET_CLIENT_TYPE(cmd)) /* �Ƿ�Ϊ�ն˲ɼ��豸 */


//////////////////////////////////////////////////////////////////////////
// ͨ�������붨��
#define COMMON_CMD_EXTENSION                        0x0000 /* ����:��չ���� ���Ͷ�:����� �ͻ��� �ɼ��豸 ���ͽṹ:T_MSG_EXTENSION �ظ��ṹ:���ݾ������չ���ܶ��� */
#define COMMON_CMD_LOGIN                            0x0001 /* ����:��¼���� ���Ͷ�:�ͻ��� ���ͽṹ:T_MSG_LOGIN �ظ��ṹ:T_MSG_SERVER_TIME */
#define COMMON_CMD_GET_DEVICE_LIST                  0x0002 /* ����:��ȡ�豸�б� ���Ͷ�:�ͻ��� ���ͽṹ:�� �ظ��ṹ:T_MSG_DEVICES_LIST_REP */
#define COMMON_CMD_GET_DEVICE_INFO                  0x0003 /* ����:��ȡ�豸��Ϣ ���Ͷ�:�ͻ��� ���ͽṹ: �ظ��ṹ: */
#define COMMON_CMD_REALTIME_DATA                    0x0004 /* ����:�������㲥�豸�ɼ�����ʵʱ���� ���Ͷ�: PC����ˡ��ɼ��豸 ���ͽṹ:T_MSG_DEVICE_REALTIME_DATA��T_MSG_BROADCAST_DEVICE_REALTIME_DATA �ظ��ṹ:�� */
#define COMMON_CMD_EXIT                             0x0005 /* ����:�˳� ���Ͷ�: PC����˻�ͻ��� ���ͽṹ:�� �ظ��ṹ:�� */
#define COMMON_CMD_GET_DEVICE_STATUS                0x0006 /* ����:��ѯ�豸����״̬ ���Ͷ�: PC�ͻ��� ���ͽṹ:�� �ظ��ṹ:�� */
#define COMMON_CMD_SET_DEVICE_PARAM                 0x0007 /* ����:�����豸������һ������ֵ����������ֵ�����ֵ�� ���Ͷ�: PC�ͻ��� ���ͽṹ: T_MSG_DEVICE_PARAM �ظ��ṹ:�� */
#define COMMON_CMD_SEND_HEARTBEAT                   0x0008 /* ����:�������� ���Ͷ�: PC�ͻ��� ���ͽṹ:�� �ظ��ṹ:T_MSG_HEARTBEAT */
#define COMMON_CMD_GET_HISTORY_DEVICE_DATA_COUNT    0x0009 /* ����:��ȡ��ʷ�豸�������� ���Ͷ�: PC�ͻ��� ���ͽṹ:T_MSG_GET_DEVICE_DATA_COUNT_REQ �ظ��ṹ:T_MSG_GET_DEVICE_DATA_COUNT_REP */
#define COMMON_CMD_GET_HISTORY_DEVICE_DATA          0x000A /* ����:��ȡ��ʷ�豸���� ���Ͷ�: PC�ͻ��� ���ͽṹ:T_MSG_GET_DEVICE_DATA_REQ �ظ��ṹ:T_MSG_GET_DEVICE_DATA_REP */
#define COMMON_CMD_REALTIME_BATCHED_DATA            0x000B /* ����:�������� ���Ͷ�: PC�ͻ��� ���ͽṹ:T_MSG_GET_DEVICE_DATA_REQ �ظ��ṹ:T_MSG_GET_DEVICE_DATA_REP */
//20180122 ����
#define COMMON_CMD_REALTIME_BATCHED_DATA_NEW           0x000C /* ����:�������� */

//////////////////////////////////////////////////////////////////////////
// PC�ͻ���������������������
#define PC_CLIENT_CMD_LOGIN                         (SET_PC_CLIENT_CMD_BIT(COMMON_CMD_LOGIN))           /*PC�ͻ��˵�¼*/
#define PC_CLIENT_CMD_GET_DEVICE_LIST               (SET_PC_CLIENT_CMD_BIT(COMMON_CMD_GET_DEVICE_LIST)) /*��ȡ�豸�б�*/
#define PC_CLIENT_CMD_GET_DEVICE_INFO               (SET_PC_CLIENT_CMD_BIT(COMMON_CMD_GET_DEVICE_INFO)) /*��ȡ�豸����*/
#define PC_CLIENT_CMD_EXIT                          (SET_PC_CLIENT_CMD_BIT(COMMON_CMD_EXIT))            /*PC�ͻ����˳�*/
#define PC_CLIENT_CMD_GET_DEVICE_STATUS             (SET_PC_CLIENT_CMD_BIT(COMMON_CMD_GET_DEVICE_STATUS))/*PC�ͻ��˻�ȡָ���豸������״̬*/
#define PC_CLIENT_CMD_SET_DEVICE_PARAM              (SET_PC_CLIENT_CMD_BIT(COMMON_CMD_SET_DEVICE_PARAM)) /*�����豸����*/
#define PC_CLIENT_CMD_SEND_HEARTBEAT                (SET_PC_CLIENT_CMD_BIT(COMMON_CMD_SEND_HEARTBEAT))   /* �������� */
#define PC_CLIENT_CMD_GET_HISTORY_DEVICE_DATA_COUNT (SET_PC_CLIENT_CMD_BIT(COMMON_CMD_GET_HISTORY_DEVICE_DATA_COUNT))/*��ȡ��ʷ�豸��������*/
#define PC_CLIENT_CMD_GET_HISTORY_DEVICE_DATA       (SET_PC_CLIENT_CMD_BIT(COMMON_CMD_GET_HISTORY_DEVICE_DATA)) /*��ȡ��ʷ�豸����*/

//////////////////////////////////////////////////////////////////////////
// �ɼ��豸������������������
#define DEVICE_CMD_LOGIN                                (SET_DEVICE_BIT(COMMON_CMD_LOGIN))        /*�豸��¼*/
#define DEVICE_CMD_EXIT                                 (SET_DEVICE_BIT(COMMON_CMD_EXIT))         /*�豸�˳�*/
#define DEVICE_CMD_REALTIME_DATA                        (SET_DEVICE_BIT(COMMON_CMD_REALTIME_DATA))/*�豸�ϴ�ʵʱ�ɼ�����*/
#define DEVICE_CMD_SEND_HEARTBEAT                       (SET_DEVICE_BIT(COMMON_CMD_SEND_HEARTBEAT))/*�豸�������������������ط�����ʱ��*/
#define DEVICE_CMD_REALTIME_BATCHED_DATA                (SET_DEVICE_BIT(COMMON_CMD_REALTIME_BATCHED_DATA))/*�豸�����ϴ��ɼ�����*/
//20180122 ����
#define DEVICE_CMD_REALTIME_BATCHED_DATA_NEW               (SET_DEVICE_BIT(COMMON_CMD_REALTIME_BATCHED_DATA_NEW))/*��Э���豸�����ϴ��ɼ�����*/
//////////////////////////////////////////////////////////////////////////
// PC����˷���������
#define PC_SERVER_CMD_LOGIN_REP                         (SET_PC_SERVER_CMD_BIT(SET_REP_BIT(COMMON_CMD_LOGIN)))              /*��¼�ظ�*/
#define PC_SERVER_CMD_GET_DEVICE_LIST_REP               (SET_PC_SERVER_CMD_BIT(SET_REP_BIT(COMMON_CMD_GET_DEVICE_LIST)))    /*��ȡ�豸�б�ظ�*/
#define PC_SERVER_CMD_GET_DEVICES_INFO_REP              (SET_PC_SERVER_CMD_BIT(SET_REP_BIT(COMMON_CMD_GET_DEVICE_INFO)))    /*��ȡ�豸����ظ�*/
#define PC_SERVER_CMD_GET_DEVICE_STATUS_REP             (SET_PC_SERVER_CMD_BIT(SET_REP_BIT(COMMON_CMD_GET_DEVICE_STATUS)))  /*��ȡ�豸����״̬�ظ�*/
#define PC_SERVER_CMD_SET_DEVICE_PARAM_REP              (SET_PC_SERVER_CMD_BIT(SET_REP_BIT(COMMON_CMD_SET_DEVICE_PARAM)))   /*�����豸�����ظ�*/
#define PC_SERVER_CMD_SEND_HEARTBEAT_REP                (SET_PC_SERVER_CMD_BIT(SET_REP_BIT(COMMON_CMD_SEND_HEARTBEAT)))     /*���������ظ� */
#define PC_SERVER_CMD_GET_HISTORY_DEVICE_DATA_COUNT_REP (SET_PC_SERVER_CMD_BIT(SET_REP_BIT(COMMON_CMD_GET_HISTORY_DEVICE_DATA_COUNT)))/*��ȡ��ʷ�豸���������ظ�*/
#define PC_SERVER_CMD_GET_HISTORY_DEVICE_DATA_REP       (SET_PC_SERVER_CMD_BIT(SET_REP_BIT(COMMON_CMD_GET_HISTORY_DEVICE_DATA)))/*��ȡ��ʷ�豸���ݻظ�*/
#define PC_SERVER_CMD_REALTIME_BATCHED_DATA_REP         (SET_PC_SERVER_CMD_BIT(SET_REP_BIT(COMMON_CMD_REALTIME_BATCHED_DATA)))   /*�ظ��豸�����ϴ�ʵʱ�ɼ����ݵĽ��*/
// PC����˹㲥����
#define PC_SERVER_CMD_BROADCAST_REALTIME_DATA           (SET_PC_SERVER_CMD_BIT(SET_NOTIFY_BIT(COMMON_CMD_REALTIME_DATA)))   /*�㲥�豸ʵʱ�ɼ����ݵ��ͻ���*/
#define PC_SERVER_CMD_BROADCAST_DEVICE_STATUS           (SET_PC_SERVER_CMD_BIT(SET_NOTIFY_BIT(COMMON_CMD_GET_DEVICE_STATUS)))/*�㲥�豸����״̬�仯*/


#endif//!_PROTOCOL_CMD_H_