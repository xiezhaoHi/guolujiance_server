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

#include "message_queue.h"

#include <QDebug>
//�������󷽷�
static CMessageQueue *_sharedObject = nullptr;
int CMessageQueue:: m_count = 0;

CMessageQueue* CMessageQueue::getInstance()
{
    if (! _sharedObject)
    {
        _sharedObject = new CMessageQueue();
        _sharedObject->init();
    }

    return _sharedObject;
}

//��ʼ��
bool CMessageQueue::init()
{

    return true;
}

//����
CMessageQueue::CMessageQueue()
{
    isConnect = false;
}

//����
CMessageQueue::~CMessageQueue(void)
{
}

//��ʼTcp����
//session_message_queue CMessageQueue::GetRecMessageQueue()
//{
//	//m_mutex.lock();
//	session_message_queue ret;// = rec_msgs_queue;
//	//m_mutex.unlock();
//	return ret;
//}
bool CMessageQueue::isEmpty()
{
    bool ismepty = rec_msgs_queue.Empty();
    return ismepty;
}
bool CMessageQueue::wait(unsigned long timeout)
{
    return rec_msgs_queue.Wait( timeout );
}
bool CMessageQueue::RecQueuePushBack(CMessage * pMessage)
{
    m_mutex.lock();
    rec_msgs_queue.Push(pMessage);
    m_count++;
    qDebug() << "CMessageQueue recive count:" << m_count <<"  current count:" << rec_msgs_queue.count();
    m_mutex.unlock();
    return true;
}
CMessage* CMessageQueue::RecQueuePopFront()
{
    m_mutex.lock();
    CMessage* ret = NULL;
    if( !rec_msgs_queue.isEmpty())
    {
        ret = rec_msgs_queue.Take();
    }
    m_mutex.unlock();
    return ret;
}
void CMessageQueue::setConnectState(bool states )
{
    m_mutex.lock();
    isConnect = states;
    m_mutex.unlock();
}
bool CMessageQueue::GetConnectState()
{
    m_mutex.lock();
    bool isresult = isConnect;
    m_mutex.unlock();
    return isresult;
}