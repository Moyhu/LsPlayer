#ifndef LVESSEL_H
#define LVESSEL_H

#include <QDebug>


/*
 * 静态容器，特点是直接指针操作固定内存,不会在高频使用时因申请内存浪费时间
 * 后期考虑仿照vector可以动态设置大小
*/
class LVessel
{
public:
    LVessel(int queueSize, int count);
    ~LVessel();

    void release();

    int pop_front(void** address);

    int push_back(void** address);

    int count() const {return m_count;}

    int maxiMum() const {return m_maxiMum;}

    // 遍历所有元素首地址，传入回调函数。如有此需要，建议在容器初始化与销毁时应用
    void traverse(void(*cb)(int index, unsigned char *data, int size));


private:
    unsigned char *m_data;          // 数据区
    unsigned long m_size;           // 数据区总大小
    unsigned long m_queueSize;      // 单一元素大小
    int m_maxiMum;                  // 元素上限
    int m_count;                    // 元素个数

    int m_pointHead;
    int m_pointTail;
};

#endif // LVESSEL_H
