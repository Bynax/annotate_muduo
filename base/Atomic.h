/********************************************************************************* 
  * FileName: Atomic.h
  * Date 2020/4/3
  * Description: 实现原子操作，GCC通过汇编级别的代码来保证赋值类操作的原子性，
  *               相对于涉及到操作系统系统调用和应用层同步的锁和互斥量
**********************************************************************************/

#ifndef ANNOTATE_MUDUO_ATOMIC_H
#define ANNOTATE_MUDUO_ATOMIC_H

#include <cstdint>
#include "noncopyable.h"
namespace annotate_muduo{
    namespace detail{
        template <typename T>
        class AtomicIntegerT : noncopyable{
        public:
            AtomicIntegerT():value_(0){}
            T get(){
                return __atomic_load_n(&value_,__ATOMIC_SEQ_CST);
            }

            T getAndAdd(T x){
                return __atomic_fetch_add(&value_,x,__ATOMIC_SEQ_CST);
            }
            T addAndGet(T x){
                return getAndAdd(x) + x;
            }
            T incrementAndGet(){
                return addAndGet(1);
            }
            T decrementAndGet(){
                return addAndGet(-1);
            }
            void add(T x){
                getAndAdd(x);
            }
            void increment(){
                incrementAndGet();
            }
            void decrement(){
                decrementAndGet();
            }
            T getAndSet(T newValue){
                return __atomic_exchage_n(&value_,newValue);
            }
        private:
            volatile T value_;

        };
        typedef detail::AtomicIntegerT<int32_t> AtomicInt32;
        typedef detail::AtomicIntegerT<int64_t > AtomicInt64;

    }
}

#endif //ANNOTATE_MUDUO_ATOMIC_H
