#ifndef LLOG_H
#define LLOG_H

#include <stdio.h>
#include <sys/timeb.h>
#include <time.h>
#include <iostream>
#include <fstream>

namespace lutil {


class Llog;

/*
 * 有参无参的宏重载
 * 我已经被搞晕了，我也不知道发生了什么
 * 为什么 #define lLog(...) LLOG(_unused, __VA_ARGS__)达不到无参的效果
*/
#define LLOG_HAS_PARAM(_unused, opts)  lutil::Llog((opts)).log(__FILE__,  __LINE__, __FUNCTION__)
#define LLOG_NO_PARAM(_unused)         lutil::Llog().log(__FILE__,  __LINE__, __FUNCTION__)

#define GET_MACRO(_1, _2, NAME,...) NAME
#define LLOG(...) GET_MACRO(__VA_ARGS__, LLOG_HAS_PARAM, LLOG_NO_PARAM, ...)(__VA_ARGS__)

#define lLog(opts...) LLOG(_unused, ##opts)

//#define lLog(...) Llog(__VA_ARGS__).log(__FILE__,  __LINE__, __FUNCTION__)
//#define lLog(opts...) Llog(##opts).log(__FILE__,  __LINE__, __FUNCTION__)

//#define lLog lutil::Llog(__FILE__,  __LINE__, __FUNCTION__).log
class Llog
{
public:
    enum LogOption {
        TimeStamp   =   0x0000001,  // 时间戳
        FilePath    =   0x0000002,  // 文件路径
        LineNum     =   0x0000004,  // 行号
        FuncName    =   0x0000008,  // 函数名
        LineFeed    =   0x0000010,  // 在结尾处换行

        Global      =   0x1000000,  //使用全局设置

        FileName    =   FilePath | LineNum,

        /* 以下为固定场景使用的选项集合，如有需求也可自定义选项集合 */
        FuncEntry   =   TimeStamp | LineFeed,
        DefaultOpt  =   TimeStamp | FilePath | LineNum | FuncName | LineFeed,
    };

public:

    Llog(LogOption opt = Global);

    ~Llog();

    Llog& log(const char*file, const int line, const char* func);

    static void outToFile(const char* path);

    static void outToConsole();

    static LogOption globalOption();

    static void setGlobalOption(LogOption opt);

    inline Llog& operator<<(const char* t)      { (*o) << t; return (*this); }

    inline Llog& operator<<(char t)             { (*o) << t; return (*this); }

    inline Llog& operator<<(bool t)             { (*o) << (t ? "true" : "false"); return (*this); }

    inline Llog& operator<<(signed short t)     { (*o) << t; return (*this); }

    inline Llog& operator<<(unsigned short t)   { (*o) << t; return (*this); }

    inline Llog& operator<<(signed int t)       { (*o) << t; return (*this); }

    inline Llog& operator<<(unsigned int t)     { (*o) << t; return (*this); }

    inline Llog& operator<<(signed long t)      { (*o) << t; return (*this); }

    inline Llog& operator<<(unsigned long t)    { (*o) << t; return (*this); }

    inline Llog& operator<<(float t)            { (*o) << t; return (*this); }

    inline Llog& operator<<(double t)           { (*o) << t; return (*this); }

    inline Llog& operator<<(std::string &t)     { (*o) << t; return (*this); }

    inline Llog& operator<<(const void * t)     { (*o) << t; return (*this); }

private:
    const char* timeStamp();

private:
    static std::fstream     m_out;
    static std::ostream*    o;
    static LogOption        global_option;
    LogOption               option;
};

#define LENTER_FUN lutil::FunctionLog FunctionLog(__FUNCTION__);
class FunctionLog
{
public:
    FunctionLog(const char* fun) : funName(fun) { lLog(Llog::FuncEntry) << "========> Enter Function " << funName; }
    ~FunctionLog() { lLog(Llog::FuncEntry) << "<======== Leave Function " << funName;}
private:
    std::string funName;
};

} // namespace lutil

#endif // LLOG_H
