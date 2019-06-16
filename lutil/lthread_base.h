#ifndef LTHREAD_BASE_H
#define LTHREAD_BASE_H

#include <QThread>

class L_BASE_Thread
{
public:
    L_BASE_Thread();

protected:
    virtual void start() = 0;
    virtual void run() = 0;
};


#endif // LTHREAD_BASE_H

