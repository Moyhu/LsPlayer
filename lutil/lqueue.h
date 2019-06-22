#ifndef LQUEUE_H
#define LQUEUE_H

#include <mutex>
#include <condition_variable>

/* 加锁，线程安全 */
using std::mutex;
using std::condition_variable;
using std::lock_guard;
using std::unique_lock;


/*
 * 静态容器，特点是直接指针操作固定内存,不会在高频使用时因申请内存浪费时间
 * 后期考虑仿照vector可以动态设置大小
*/
class LQueue
{
public:
    LQueue(unsigned long eleSize, unsigned int maxiMum);
    ~LQueue();

    void release();

    void pop_front(void** address);

    void push_back(void** address);

    int count();

    int maxiMum();

    // 遍历所有元素首地址，传入回调函数。如有此需要，建议在容器初始化与销毁时应用
    void traverse(void(*cb)(int index, unsigned char *data, int size));


private:
    unsigned char*      m_data;         // 数据区
    unsigned long       m_size;         // 数据区总大小
    unsigned long       m_eleSize;      // 单一元素大小
    unsigned int        m_maxiMum;      // 元素上限
    unsigned int        m_count;        // 元素个数

    int                 m_pointHead;
    int                 m_pointTail;

    mutex               m_Mutex;
    condition_variable  m_pushCond;
    condition_variable  m_popCond;
};

#endif // LQUEUE_H
