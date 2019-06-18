#ifndef LIOMODULE_H
#define LIOMODULE_H

#include "lutil/lvessel.h"
#include "lutil/lthread.h"
#include "lplayercallback.h"

class IOThread
{
public:
    IOThread();

    ~IOThread();

    void registerCallback(LPlayerCallback *cb);

    void initialize();

protected:
    void run();

private:
    LPlayerCallback*    m_cb;
    LThread             m_thread;
};

#endif // LIOMODULE_H
