#ifndef LTHREAD_QT_H
#define LTHREAD_QT_H

#include <thread>   // c++ 11 std::thread

#define _STL_SLEEP  1

#if _STL_SLEEP

#elif defined(__linux__) || defined(__unix__)
#include "sys/select.h"
#elif defined(_WIN32)
#include <winsock2.h>
#include <WS2tcpip.h>
#pragma comment(lib,"Winmm.lib")
#pragma comment(lib,"ws2_32.lib")
#endif

class LThread final
{
public:
    typedef std::thread::native_handle_type  lthread_t;

    LThread();

    ~LThread();

    template<typename Func, typename... Args>
    void start(Func func, Args... args) {
        m_thread =  new std::thread(func, args...);
    }

    bool joinAble();

    bool detach();

    lthread_t nativeHandle();

    static unsigned int hardwareConcurrency();

    static void sleep(uint32_t secs);

    static void msleep(uint32_t ms);

private:
    std::thread* m_thread;
};
#endif // LTHREAD_QT_H
