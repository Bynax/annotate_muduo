/*********************************************************************************
  * FileName: Singleton.h
  * Date 2020/4/5
  * Description: 
**********************************************************************************/

#ifndef ANNOTATE_MUDUO_SINGLETON_H
#define ANNOTATE_MUDUO_SINGLETON_H

#include "noncopyable.h"
#include <cassert>
#include <pthread.h>
#include <cstdlib>

namespace annotate_muduo{
//    namespace detail{
//        template <typename T>
//        struct has_no_destroy{
//            template <typename C>
//            static char test(decltype(&C::no_destroy));
//            template <typename C>
//            static int32_t test(...);
//            const static bool value = sizeof(test<T>(0)) == 1;
//        };
//    } // namespace detail


    // 在singleton中注释掉了namespace detail、atexit和destroy的函数
    // 这三部分主要是增加了销毁资源的方法，其实资源销毁没有必要
    // 因为在长时间运行的服务器程序中这不是一个问题，因为进程也不会在正常情况下退出
    // 而在短时间运行的程序中，当进程退出的时候资源就自动释放了
    // 这个代码的单例实现主要是靠pthread_once来保障的，线程的安全性是由pthread库来保证
    template <typename T>
    class Singleton:noncopyable{
    public:
        Singleton() = delete; // 单例模式不允许自己创建对象
        ~Singleton() = delete;
        static T& instance(){
            pthread_once(&ponce_,&Singleton::init);
            assert(value_ != NULL);
            return *value_;
        }

    private:
        static void init(){
            value_ = new T();
//            if(!detail::has_no_destroy<T>::value){
//                ::atexit(destroy);
//            }
        }
//        static void destroy(){
//            typedef char T_must_be_complete_type[sizeof(T) == 0? -1 : 1];
//            T_must_be_complete_type dummy;
//            (void) dummy;
//            delete value_;
//            value_ = NULL;
//        }

    private:
        static pthread_once_t ponce_;
        static T* value_;

    };

    template <typename T>
    pthread_once_t Singleton<T>::ponce_ = PTHREAD_ONCE_INIT;
    template <typename T>
    T* Singleton<T>::value_ = NULL;

}

#endif //ANNOTATE_MUDUO_SINGLETON_H
