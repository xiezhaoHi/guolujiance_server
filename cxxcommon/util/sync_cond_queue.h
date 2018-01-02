/*****************************************************
** �ļ�����  sync_cond_queue.h
** ��  Ȩ��  XXX ��Ȩ����
** ��  ����  
** ���ݼ�������ͬ�����ĵȴ�����
** �������ڣ�2015/08/11
** �����ˣ�  ����
** �޸ļ�¼��
   ����        �汾      �޸���    �޸����� 
    
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

    // �ȴ��������п�ȡ�õ�Ԫ��
    inline bool Wait(unsigned long timeout)
    {
        QMutexLocker lock(&m_mutex);
        return m_condition.wait(&m_mutex, timeout);
    }

    // ������д��Ԫ��
    inline void Push(const T & t)
    {
        QMutexLocker lock(&m_mutex);
        QQueue::enqueue(t);
        m_condition.wakeOne();
    }

    // �Ӷ�����ȡ�߶���Ԫ��
    inline T Take()
    {
        QMutexLocker lock(&m_mutex);
        return QQueue::dequeue();
    }

    // �ж϶����Ƿ�Ϊ��
    inline bool Empty()
    {
        QMutexLocker lock(&m_mutex);
        return QQueue::isEmpty();
    }

    // ���ض���Ԫ��
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
