#include "llog.h"
namespace lutil {

std::fstream    Llog::m_out;
std::ostream*   Llog::o = &std::cout;
Llog::LogOption Llog::global_option = Llog::DefaultOpt;

Llog::Llog(LogOption opt)
{
    option = (opt & Global) ? Llog::global_option : opt;
}

Llog::~Llog()
{
    if (option & LineFeed)
    {
        (*o) << std::endl;
    }
}

Llog& Llog::log(const char*file, const int line, const char* func)
{
    if (option & TimeStamp) {
        (*o) << timeStamp() << " ";
    }

    if (option & FileName) {
        (*o) << file << ":" << line << " ";
    }

    if (option & FuncName) {
        (*o) << func << " ";
    }
    (*o) << "\t";
    return *this;
}

void Llog::outToFile(const char* path)
{
    if (m_out.is_open())
    {
        m_out.close();
    }

    m_out.open(path, std::ios::app | std::ios::out);
    if (m_out.is_open())
    {
        o = &m_out;
    }
    else
    {
        lLog() << "log file [" << path << "] open failed!!";
    }
}

void Llog::outToConsole()
{
    if (m_out.is_open())
    {
        m_out.close();
    }
    o = &std::cout;
}

Llog::LogOption Llog::globalOption()
{
    return global_option;
}

void Llog::setGlobalOption(Llog::LogOption opt)
{
    global_option = opt;
}

const char* Llog::timeStamp()
{
    struct  tm      *ptm;
    struct  timeb   stTimeb;
    static  char    timeStamp[24];

    ftime(&stTimeb);
    ptm = localtime(&stTimeb.time);
    sprintf(timeStamp, "%04d-%02d-%02d %02d:%02d:%02d.%03d",
            ptm->tm_year+1900, ptm->tm_mon+1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec, stTimeb.millitm);
    timeStamp[25] = 0;
    return timeStamp;
}

} // namespace lutil
