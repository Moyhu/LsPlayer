#include "lthread.h"

LThread::LThread() : m_thread(NULL)
{

}

LThread::~LThread()
{
    if (m_thread != NULL && m_thread->joinable())
    {
        m_thread->join();
    }
}

bool LThread::joinAble()
{
    return (m_thread == NULL) ? false : m_thread->joinable();
}

bool LThread::detach()
{
    if (joinAble())
    {
        m_thread->detach();
        return true;
    }
    else
    {
        return false;
    }
}

LThread::lthread_t LThread::nativeHandle()
{
    return (m_thread == NULL) ? 0 : m_thread->native_handle();
}

unsigned int LThread::hardwareConcurrency()
{
    return std::thread::hardware_concurrency();
}

void LThread::sleep(uint32_t secs)
{
#if _STL_SLEEP
    std::this_thread::sleep_for(std::chrono::seconds(secs));
#elif
    msleep(secs * 1000);
#endif
}

void LThread::msleep(uint32_t ms)
{
#if _STL_SLEEP
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
#elif defined(__linux__) || defined(__unix__)
    timeval timeout;
    timeout.tv_sec = ms / 1000;
    timeout.tv_usec = ms % 1000 * 1000;
    select(0, NULL, NULL, NULL, &timeout);
#elif defined(_WIN32)
    timeBeginPeriod(1);
    Sleep(millisecond);
    DWORD dwTime = ::timeGetTime();
    timeEndPeriod(1);
#endif
}
