#ifndef LTHREAD_QT_H
#define LTHREAD_QT_H

#include "lthread_base.h"

#define QT_THREAD   1

#if QT_THREAD
    #include <QThread>
    #define LThread QThread
#else
    #ifdef WIN32
        #define L_PARANT_Thread L_WIN_Thread
    #else
        #define L_PARANT_Thread L_UNIX_Thread
    #endif
#endif

//class LThread : public L_PARANT_Thread
//{
//public:
//    LThread();
//};


#endif // LTHREAD_QT_H
