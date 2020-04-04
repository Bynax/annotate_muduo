/********************************************************************************* 
  * FileName: Exception.h
  * Date 2020/4/4
  * Description: 
**********************************************************************************/

#ifndef ANNOTATE_MUDUO_EXCEPTION_H
#define ANNOTATE_MUDUO_EXCEPTION_H

#include "Types.h"
#include <exception>

namespace annotate_muduo {
    class Exception : public std::exception {
    public:
        Exception(std::string what);

        ~Exception() noexcept override = default;

        const char *what() const noexcept override {
            return message_.c_str();
        }

        const char *stackTrace() const noexcept {
            return stack_.c_str();
        }

    private:
        std::string message_;
        std::string stack_;
    };


}


#endif //ANNOTATE_MUDUO_EXCEPTION_H
