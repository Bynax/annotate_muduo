/********************************************************************************* 
  * FileName: Condition.h
  * Date 2020/4/4
  * Description: 
**********************************************************************************/

#ifndef ANNOTATE_MUDUO_CONDITION_H
#define ANNOTATE_MUDUO_CONDITION_H

#include "Mutext.h"
#include <pthread.h>

namespace annotate_muduo{
    class Condition : noncopyable{

    public:
        explicit Condition(MutexLock& mutex)
        :mutex_(mutex){
            MCHECK(pthread_cond_init(&pcond_, nullptr));
        }
        ~Condition(){
            MCHECK(pthread_cond_destroy(&pcond_));
        }

        void wait(){
            // pthread_cond_wait要做的有3件事：
            // 1.阻塞
            // 2. 加锁
            // 3. 申请锁
            // 其中1+2是一个原子操作，因此在调用wait之前必须保证mutex被init出来且是lock状态
            MutexLock::UnassignGuard ug(mutex_);
            MCHECK(pthread_cond_wait(&pcond_,mutex_.getPthreadMutex()));
        }

        bool waitForSeconds(double seconds);
        void notify(){
            MCHECK(pthread_cond_signal(&pcond_));
        }

        void notifyALL(){
            MCHECK(pthread_cond_broadcast(&pcond_));
        }

    private:
        MutexLock& mutex_;
        pthread_cond_t pcond_;

       // 这里可以看到Condition持有的是MutexLock的引用
       // 面向对象程序设计中，对象的关系主要有三种：关联（association）、聚合（aggregation）、组合（composition）
       // 区别主要是composition有compose占有的意思，因此composition是表示一个对象占有另外一个对象，因此负责该对象的生命周期
       // 另一个是association是一种很宽泛的关系，表示一个对象用到了另一个对象，包括该对象的函数之类，但是不负责生命周期，代码表示
       // 上来说就是持有一个对象的指针或者是引用
       // 第三个是aggregation，与association一致，但是存在整体和局部的关系，而association没有这类规定
       // 这里condition与MutexLock的关系就是关联（association）关系

    };

}
#endif //ANNOTATE_MUDUO_CONDITION_H
