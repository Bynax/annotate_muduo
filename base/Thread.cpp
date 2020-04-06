/********************************************************************************* 
  * FileName: Thread.cpp
  * Date 2020/4/3
  * Description: 
**********************************************************************************/

#include "Thread.h"
#include "CurrentThread.h"
#include "Exception.h"
#include "Timestamp.h"
//#include ""

#include <sys/syscall.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
//#include <sys/pr>



namespace annotate_muduo{
    namespace detail{
        pid_t gettid(){
            return static_cast<pid_t >(::syscall(SYS_gettid));
        }
        // 执行fork后子进程可能会看到stale的缓存结果
        // 解决办法是用pthread_atfork注册一个回调，清空缓存的线程id
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
            ThreadFunc func_; // 启动线程所需要的函数
            string name_; // 线程名称
            pid_t* tid_; // 线程id
            CountDownLatch* latch_;

            ThreadData(const ThreadFunc& func,
                    const string& name,
                    pid_t* tid,
                    CountDownLatch* latch)
                    :func_(func),name_(name),tid_(tid),latch_(latch){}
            void runInThread(){
                *tid_ = annotate_muduo::CurrentThread::tid();
                tid_ = NULL;
                latch_ -> countDown();
                latch_ = NULL;

                annotate_muduo::CurrentThread::t_threadName = name_.empty()?
                        "muduoThread" : name_.c_str();
                // 其实就是设置进程名，只在Linux上有，mac没有
                // PR_SET_NAME set the name of calling thread
//                ::prctl(PR_SET_NAME, CurrentThread::t_threadName);
                try{
                    func_();
                    // 如果可以顺利执行完成，threadname变为finished
                    annotate_muduo::CurrentThread::t_threadName = "finished";
                }catch (const Exception& ex){
                    CurrentThread::t_threadName = "crashed";
                    fprintf(stderr,"exception caught in Thread %s\n",
                            name_.c_str());
                    fprintf(stderr,"reson:%s\n",ex.what());
                    fprintf(stderr,"stack trace: %s\n",ex.stackTrace());
                    abort();
                }catch (const std::exception& ex){
                    CurrentThread::t_threadName = "crashed";
                    fprintf(stderr,"exception caught in Thread %s\n",
                            name_.c_str());
                    fprintf(stderr,"reason:%s\n",ex.what());
                    abort();
                }catch (...){
                    CurrentThread::t_threadName = "crashed";
                    fprintf(stderr,"unknown exception caught in Thread %s\n",name_.c_str());
                    throw;
                }
            }
        };

        void* startThread(void* obj){
            auto* data = static_cast<ThreadData*>(obj);
            data -> runInThread();
            delete(data);
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
        return tid() == ::getpid(); // 进程的id代表的其实就是当前进程主线程的id
    }

    void CurrentThread::sleepUsec(int64_t usec)
    {
        struct timespec ts = { 0, 0 };
        ts.tv_sec = static_cast<time_t>(usec / Timestamp::kMicroSecondsPerSecond);
        ts.tv_nsec = static_cast<long>(usec % Timestamp::kMicroSecondsPerSecond * 1000);
        ::nanosleep(&ts, NULL);
    }

    Thread::Thread(ThreadFunc func, const string& n)
            : started_(false),
              joined_(false),
              pthreadId_(0),
              tid_(0),
              func_(std::move(func)),
              name_(n),
              latch_(1)
    {
        setDefaultName();
    }

    Thread::~Thread()
    {
        if (started_ && !joined_)
        {
            pthread_detach(pthreadId_);
        }
    }

    void Thread::setDefaultName()
    {
        int num = numCreated_.incrementAndGet();
        if (name_.empty())
        {
            char buf[32];
            snprintf(buf, sizeof buf, "Thread%d", num);
            name_ = buf;
        }
    }

    void Thread::start()
    {
        assert(!started_);
        started_ = true;
        // FIXME: move(func_)
        detail::ThreadData* data = new detail::ThreadData(func_, name_, &tid_, &latch_);
        if (pthread_create(&pthreadId_, NULL, &detail::startThread, data))
        {
            started_ = false;
            delete data; // or no delete?
            // LOG_SYSFATAL << "Failed in pthread_create";
        }
        else
        {
            latch_.wait();
            assert(tid_ > 0);
        }
    }

    int Thread::join()
    {
        assert(started_);
        assert(!joined_);
        joined_ = true;
        return pthread_join(pthreadId_, NULL);
    }
}