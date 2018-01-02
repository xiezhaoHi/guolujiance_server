/*****************************************************
** 文件名：  sync_cond_queue.h
** 版  权：  XXX 版权所有
** 版  本：  
** 内容简述：带同步锁的等待队列
** 创建日期：2015/08/11
** 创建人：  唐桥
** 修改记录：
   日期        版本      修改人    修改内容 
    
*****************************************************/
#ifndef SYNC_COND_QUEUE_H
#define SYNC_COND_QUEUE_H

#include <QObject>
#include <QQueue>
#include <QMutex>
#include <QMutexLocker>
#include <QWaitCondition>


template <class T>
class CSyncCondQueue : public QQueue<T>
{
public:
    inline CSyncCondQueue()
    {

    }

    inline ~CSyncCondQueue()
    {
    }

    // 等待队列中有可取用的元素
    inline bool Wait(unsigned long timeout)
    {
        QMutexLocker lock(&m_mutex);
        return m_condition.wait(&m_mutex, timeout);
    }

    // 向队列中存放元素
    inline void Push(const T & t)
    {
        QMutexLocker lock(&m_mutex);
        QQueue::enqueue(t);
        m_condition.wakeOne();
    }

    // 从队列中取走队首元素
    inline T Take()
    {
        QMutexLocker lock(&m_mutex);
        return QQueue::dequeue();
    }

    // 判断队列是否为空
    inline bool Empty()
    {
        QMutexLocker lock(&m_mutex);
        return QQueue::isEmpty();
    }

    // 返回队首元素
    inline T Front() 
    {
        QMutexLocker lock(&m_mutex);
        return QQueue::head();
    }

private:
    QMutex          m_mutex;
    QWaitCondition  m_condition;
};




#endif // SYNC_COND_QUEUE_H
