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
        explicit Condition(MutexLock&mutex)
        :mutex_(mutex){
            MCHECK(pthread_cond_init(&pcond_, nullptr));
        }
        ~Condition(){
            MCHECK(pthread_cond_destroy(&pcond_));
        }

        void wait(){
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

    };

}
#endif //ANNOTATE_MUDUO_CONDITION_H
