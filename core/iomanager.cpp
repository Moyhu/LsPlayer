#include "iomanager.h"


extern "C" {
#include <libavformat/avformat.h>
}

IOManager::IOManager() : m_running(false)
{

}

IOManager::~IOManager()
{

}

void IOManager::registerCallback(LPlayerCallback *cb)
{
    m_cb = cb;
}

void IOManager::initialize(LQueue *paketQueue)
{
//    avformat_network_init();
    m_paketQueue = paketQueue;
    m_running = true;
    m_ioThread.start(&IOManager::run, this);
}

void IOManager::run()
{
    while (m_running)
    {
        printf("run run run!!!\n");
        LThread::sleep(1);
    }
}
