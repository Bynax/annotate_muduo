/********************************************************************************* 
  * FileName: Type.h
  * Date 2020/4/2
  * Description: 主要实现的是更为安全/高效的up_cast和down_cast
**********************************************************************************/

#ifndef ANNOTATE_MUDUO_TYPES_H
#define ANNOTATE_MUDUO_TYPES_H

#include <string>
#include <cstring>
#ifndef NDEBUG
#include <cassert>

#endif

namespace annotate_muduo
{
    using std::string;

    inline void memZero(void* p, size_t n){
        memset(p,0,n);
    }

    /**
     *
     * implicit_cast是对static_cast和const_cast一种更为安全的实现方式，
     * 使用场景也就是在向上转型的时候，implicit_cast会进行类型检查，但是
     * static_cast不会
     *
     */

    template <typename To, typename From>
    inline To implicit_cast(From const& f){
        return f;
    }

    /**
     *
     * 在运行时尽量避免dynamic_cast的使用，使用static_cast代替
     * 原因参见Google编码规范5.8
     * 替代的方案则是在debug模式下内部使用dynamic_cast进行验证，
     * 在release下使用static_cast替换dynamic_cast
     * 也就是down_cast实现的方法
     *
     */
    template <typename To, typename From>
    inline To down_cast(From* f){
        if(false){
            implicit_cast<From*, To>(0);
        }
#if !defined(NDEBUG) && !defined(GOOGLE_PROTOBUF_NO_RTTI)
        assert(f == NULL || dynamic_cast<To>(f) != NULL);
#endif
        return static_cast<To>(f);
    }
}

#endif //ANNOTATE_MUDUO_TYPES_H
