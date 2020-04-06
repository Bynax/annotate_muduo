/********************************************************************************* 
  * FileName: ThreadPool.h
  * Date 2020/4/6
  * Description: 
**********************************************************************************/

#ifndef ANNOTATE_MUDUO_THREADPOOL_H
#define ANNOTATE_MUDUO_THREADPOOL_H


#include "Types.h"
#include "Thread.h"
#include "Mutext.h"
#include "Condition.h"
#include <vector>
#include <deque>
namespace annotate_muduo{
    class ThreadPool : noncopyable{
    public:
        typedef std::function<void ()> Task;
        explicit ThreadPool(const string& nameArg = string("ThreadPool"));
        ~ThreadPool();

        // Must be called before stat()
        void setMaxQueueSize(int maxSize){
            maxQueueSize_ = maxSize;
        }

        void setThreadInitCallback(const Task& cb){
            threadInitCallback_ = cb;
        }

        void start(int numThreads);
        void stop();
        const string& name()const {
            return name_;
        }
        size_t queueSize()const ;

        void run(Task f);

    private:
        bool isFull()const REQUIRES(mutex_);
        void runInThread();
        Task take();
        mutable MutexLock mutex_;
        Condition notEmpty_ GUARDED_BY(mutex_);
        Condition notFull_ GUARDED_BY(mutex_);
        string name_;
        Task threadInitCallback_;
        std::vector<std::unique_ptr<annotate_muduo::Thread>> threads_;
        // 线程池的出现本来就是为了做任务的，因此线程池肯定要有任务队列
        std::deque<Task> queue_ GUARDED_BY(mutex_);
        size_t maxQueueSize_;
        bool running_;
    };
}



#endif //ANNOTATE_MUDUO_THREADPOOL_H
