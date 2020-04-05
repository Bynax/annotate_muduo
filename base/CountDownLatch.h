/********************************************************************************* 
  * FileName: CountDownLatch.h
  * Date 2020/4/4
  * Description: 
**********************************************************************************/

#ifndef ANNOTATE_MUDUO_COUNTDOWNLATCH_H
#define ANNOTATE_MUDUO_COUNTDOWNLATCH_H


#include "noncopyable.h"
#include "Mutext.h"
#include "Condition.h"

namespace annotate_muduo {
    class CountDownLatch : noncopyable {
    public:
        explicit CountDownLatch(int count);

        void wait();

        void countDown();

        int getCount() const;

    private:
        // mutable的作用详见https://www.zhihu.com/question/64969053
        // 主要就是当用到const成员函数的时候，编译器会对象的内部状态是否改变（成员是否改变等），如果改变则会编译错误
        // 将一个成员使用mutable修饰则是告诉编译器这个成员不算对象的内部状态
        mutable MutexLock mutex_;
        Condition condition_ GUARDED_BY(mutex_);
        int count_ GUARDED_BY(mutex_);

    };
}

#endif //ANNOTATE_MUDUO_COUNTDOWNLATCH_H
