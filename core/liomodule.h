#ifndef LIOMODULE_H
#define LIOMODULE_H

#include "lutil/lvessel.h"
#include "lutil/lthread.h"
#include "lplayercallback.h"

class LIOModule : public LThread
{
public:
    LIOModule();

    ~LIOModule();

    void registerCallback(LPlayerCallback *cb);

    void initialize();

protected:
    virtual void run();

private:
    LPlayerCallback *m_cb;
};

#endif // LIOMODULE_H
