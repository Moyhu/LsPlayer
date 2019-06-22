#ifndef LIOMANAGER_H
#define LIOMANAGER_H

#include <atomic>
#include "lutil/lqueue.h"
#include "lutil/lthread.h"
#include "lplayercallback.h"

using std::atomic;

class IOManager
{
public:
    IOManager();

    ~IOManager();

    void registerCallback(LPlayerCallback *cb);

    void initialize(LQueue* paketQueue);

protected:
    void run();

private:
    LPlayerCallback*    m_cb;
    LThread             m_ioThread;
    LQueue*             m_paketQueue;
    atomic<bool>        m_running;
};

#endif // LIOMANAGER_H
