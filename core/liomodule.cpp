#include "liomodule.h"


extern "C" {
#include <libavformat/avformat.h>
}

LIOModule::LIOModule()
{

}

LIOModule::~LIOModule()
{

}

void LIOModule::registerCallback(LPlayerCallback *cb)
{
    m_cb = cb;
}

void LIOModule::initialize()
{
    avformat_network_init();
}

void LIOModule::run()
{}
