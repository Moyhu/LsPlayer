#include "iothread.h"


extern "C" {
#include <libavformat/avformat.h>
}

IOThread::IOThread()
{

}

IOThread::~IOThread()
{

}

void IOThread::registerCallback(LPlayerCallback *cb)
{
    m_cb = cb;
}

void IOThread::initialize()
{
    avformat_network_init();
}

void IOThread::run()
{}
