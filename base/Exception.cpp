/********************************************************************************* 
  * FileName: Exception.cpp
  * Date 2020/4/4
  * Description: 
**********************************************************************************/

#include "Exception.h"
#include "CurrentThread.h"

namespace annotate_muduo{
    Exception::Exception(std::string msg)
    :message_(std::move(msg)),
    stack_(CurrentThread::stackTrace(false))
    {}
}
