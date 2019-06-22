#include "lqueue.h"

LQueue::LQueue(unsigned long eleSize, unsigned int maxiMum)
    : m_eleSize(eleSize),
      m_maxiMum(maxiMum)
{
    m_data = new unsigned char[m_eleSize * m_maxiMum];

    release();
}

LQueue::~LQueue()
{
    lock_guard<mutex> locker(m_Mutex);
    if (m_data != NULL)
    {
        delete[] m_data;
    }
}

void LQueue::release()
{
    m_count = 0;
    m_pointHead = 0;
    m_pointTail = 0;
}

/*
 * @param: address [out]
 * @return 0:fail 1:success
 * @example:
 *      AVPaket *pkt;
 *      if (vessel.pop_front(&pkt))
 *      {
 *          pkt-> ...
 *      }
*/
void LQueue::pop_front(void** address)
{
    unique_lock<mutex> locker(m_Mutex);
    while (m_count <= 0)
    {
        m_popCond.wait(locker);
    }
    m_pointHead = (m_pointHead + 1) % m_maxiMum;
    m_count--;
    *address = m_data + (m_pointHead * m_eleSize);
    m_pushCond.notify_one();
}

/*
 * @param: address [out]
 * @return 0:fail 1:success
*/
void LQueue::push_back(void** address)
{
    unique_lock<mutex> locker(m_Mutex);
    while (m_count >= m_maxiMum)
    {
        m_pushCond.wait(locker);
    }
    m_pointTail = (m_pointTail + 1) % m_maxiMum;
    m_count++;
    *address = m_data + (m_pointTail * m_eleSize);
    m_popCond.notify_one();
}

int LQueue::count()
{
    lock_guard<mutex> locker(m_Mutex);
    return m_count;
}

int LQueue::maxiMum()
{
    lock_guard<mutex> locker(m_Mutex);
    return m_maxiMum;
}

void LQueue::traverse(void(*cb)(int index, unsigned char *data, int size))
{
    lock_guard<mutex> locker(m_Mutex);
    int index = 0;
    for (; index < m_maxiMum; index++)
    {
        cb(index, m_data + (index * m_eleSize), m_eleSize);
    }
}
