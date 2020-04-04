/********************************************************************************* 
  * FileName: CurrentThread.h
  * Date 2020/4/3
  * Description: 
**********************************************************************************/

#ifndef ANNOTATE_MUDUO_CURRENTTHREAD_H
#define ANNOTATE_MUDUO_CURRENTTHREAD_H

#include "Types.h"

namespace annotate_muduo{
    namespace CurrentThread{
        // __thread修饰的变量表示是线程局部存储的
        // 如果不用__thread修饰表示该变量是可以被多个线程共享的
        // __thread关键字只能修饰POD（plain old data）类型
        //
        extern __thread int t_cachedTid;
        extern __thread char t_tidString[32];
        extern __thread int t_tidStringLength;
        extern __thread const char* t_threadName;
        void cacheTid();

        inline int tid(){
            if(__builtin_expect(t_cachedTid == 0, 0)){
                cacheTid();
            }
            return t_cachedTid;
        }

        inline const char* tidString(){
            return t_tidString;
        }

        inline int tidStringLength(){
            return t_tidStringLength;
        }

        inline const char* name(){
            return t_threadName;
        }

        bool isMainThread();
        void sleepUsec(int64_t usec);


        std::string stackTrace(bool demangle);

    }
}



#endif //ANNOTATE_MUDUO_CURRENTTHREAD_H
