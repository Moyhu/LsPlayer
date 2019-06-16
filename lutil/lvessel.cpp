#include "lvessel.h"

LVessel::LVessel(int queueSize, int maxiMum)
    : m_queueSize(queueSize),
      m_maxiMum(maxiMum)
{
    if ((queueSize <= 0) || (maxiMum <= 0))
    {
        qDebug() << "LQueue: error queueSize:" << queueSize << " maxiMum:" << maxiMum;
        return;
    }

    m_data = new unsigned char[m_queueSize * maxiMum];
    if (m_data == NULL)
    {
        qDebug() << "LQueue: new data failed!";
    }
    release();
}

LVessel::~LVessel()
{
    if (m_data != NULL)
    {
        delete[] m_data;
    }
}

void LVessel::release()
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
int LVessel::pop_front(void** address)
{
    if (m_count <= 0)
        return 0;
    m_pointHead = (m_pointHead + 1) % m_maxiMum;
    m_count--;
    *address = m_data + (m_pointHead * m_queueSize);
    return 1;
}

/*
 * @param: address [out]
 * @return 0:fail 1:success
*/
int LVessel::push_back(void** address)
{
    if (m_count >= m_maxiMum)
        return 0;
    m_pointTail = (m_pointTail + 1) % m_maxiMum;
    m_count++;
    *address = m_data + (m_pointTail * m_queueSize);
    return 1;
}


void LVessel::traverse(void(*cb)(int index, unsigned char *data, int size))
{
    int index = 0;
    for (; index < m_maxiMum; index++)
    {
        cb(index, m_data + (index * m_queueSize), m_queueSize);
    }
}
