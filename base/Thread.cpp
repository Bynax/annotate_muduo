/********************************************************************************* 
  * FileName: Thread.cpp
  * Date 2020/4/3
  * Description: 
**********************************************************************************/

#include "Thread.h"
#include "CurrentThread.h"
#include "Exception.h"
//#include ""

#include <sys/syscall.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>



namespace annotate_muduo{
    namespace detail{
        pid_t gettid(){
            return static_cast<pid_t >(::syscall(SYS_gettid));
        }
        // 当进程fork之后缓存新的线程相关信息
        void afterFork(){
            CurrentThread::t_cachedTid = 0;
            CurrentThread::t_threadName = "main";
            CurrentThread::tid();
        }

        class ThreadNameInitializer{
        public:
            ThreadNameInitializer(){
                CurrentThread::t_threadName = "main";
                CurrentThread::tid();
                pthread_atfork(NULL,NULL,&afterFork);
            }
        };
        ThreadNameInitializer init;
        struct ThreadData{
            typedef annotate_muduo::Thread::ThreadFunc ThreadFunc;
            ThreadFunc func_;
            string name_;
            pid_t* tid_;
            CountDownLatch* latch_;

            ThreadData(ThreadFunc func,
                    const string& name,
                    pid_t* tid,
                    CountDownLatch* latch)
                    :func_(func),name_(name),tid_(tid),latch_(latch){}
            void runThread(){
                *tid_ = annotate_muduo::CurrentThread::tid();
                tid_ = NULL;
                latch_ -> countDown();
                latch_ = NULL;

            }

        };

        void* startThread(void* obj){
            ThreadData* data = static_cast<ThreadData*>(obj);
            data -> runThread();
            delete data;
            return NULL;
        }
    } // namespace detail

    void CurrentThread::cacheTid() {
        if(t_cachedTid == 0){
            t_cachedTid = detail::gettid();
            t_tidStringLength = snprintf(t_tidString, sizeof(t_tidString),
                    "%5d ",t_cachedTid);
        }
    }

    bool CurrentThread::isMainThread() {
        return tid() == ::getpid();
    }

    void CurrentThread::sleepUsec(int64_t usec) {

    }
}