/*****************************************************
**  文 件 名：tcp_logic.cpp
**  版 权：XXX 版权所有
**  版    本：Ver.1.0
**  内容简述：通讯逻辑类实现文件
**
**  创建日期：2015/07/22 
**  创建人：杨恒
**  修改记录：
    日期        版本      修改人    修改内容   
*****************************************************/

#include "message_queue.h"

#include <QDebug>
//单键对象方法
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

//初始化
bool CMessageQueue::init()
{

    return true;
}

//构造
CMessageQueue::CMessageQueue()
{
    isConnect = false;
}

//析构
CMessageQueue::~CMessageQueue(void)
{
}

//开始Tcp服务
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