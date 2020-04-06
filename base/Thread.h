/********************************************************************************* 
  * FileName: Thread.h
  * Date 2020/4/3
  * Description: 
**********************************************************************************/

#ifndef ANNOTATE_MUDUO_THREAD_H
#define ANNOTATE_MUDUO_THREAD_H

#include "Atomic.h"
#include "CountDownLatch.h"
#include "Types.h"

#include <functional>
#include <memory>
#include <pthread.h>

namespace annotate_muduo{
    class Thread :noncopyable {
        // 区分基于对象编程风格和面向对象风格
        // 基于对象编程风格主要是借助于bind和function库的风格，也就是函数适配器
        // 函数适配器的作用是从一种函数接口适配成另一种接口，使函数接口发生改变
        // 例如 本来有函数 void member(double , int ,int)现在想固定后两个参数
        // 可以使用function<void(int)> fp = bind(&类名::member，&对象, 参数.....)
        // 可以看作是适配器设计模式
    public:

        typedef std::function<void ()> ThreadFunc;
        explicit Thread(ThreadFunc func, const string& name = string());
        ~Thread();

        void start();
        int join();
        bool started() const{return started_;}
        pid_t tid() const{return tid_;}
        const string& name() const {return name_;}
        static int numCreated() {return numCreated_.get();}

    private:
        void setDefaultName();
        bool started_;
        bool joined_;
        pthread_t pthreadId_;
        // 这里可能会疑惑为什么又有pthread_t又有pid_t两种
        // 一般来说我们使用pid_t作为线程的id，详情可以翻阅陈硕书P89
        pid_t tid_;
        ThreadFunc func_;
        string name_;
        CountDownLatch latch_;

        static detail::AtomicInt32 numCreated_;


    };

}



#endif //ANNOTATE_MUDUO_THREAD_H
