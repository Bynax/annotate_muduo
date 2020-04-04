/********************************************************************************* 
  * FileName: copyable.h
  * Date 2020/4/3
  * Description: 
**********************************************************************************/

#ifndef ANNOTATE_MUDUO_COPYABLE_H
#define ANNOTATE_MUDUO_COPYABLE_H
namespace annotate_muduo{
    // tag class 强调该对象是可以拷贝的，可拷贝对象为值语意
    class copyable{
    protected:
        copyable() = default;
        ~copyable() = default;
    };
}

#endif //ANNOTATE_MUDUO_COPYABLE_H
