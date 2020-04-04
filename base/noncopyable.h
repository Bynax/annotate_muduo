/********************************************************************************* 
  * FileName: noncopyable.h
  * Date 2020/4/3
  * Description: 
**********************************************************************************/

#ifndef ANNOTATE_MUDUO_NONCOPYABLE_H
#define ANNOTATE_MUDUO_NONCOPYABLE_H

namespace annotate_muduo{
    // tag class 强调对象是不可拷贝的，不可拷贝的对象是对象语意
    class noncopyable{
    public:
        noncopyable(const noncopyable&) = delete;
        void operator=(const noncopyable&) = delete;

    protected:
        noncopyable() = default;
        ~noncopyable() = default;
    };
}

#endif //ANNOTATE_MUDUO_NONCOPYABLE_H
