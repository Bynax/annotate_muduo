/********************************************************************************* 
  * FileName: CurrentThread.cpp
  * Date 2020/4/3
  * Description: 
**********************************************************************************/

#include "CurrentThread.h"
#include <cxxabi.h>
#include <execinfo.h>
#include <cstdlib>

namespace annotate_muduo{
    namespace CurrentThread{
        __thread int t_cachedTid = 0;
        __thread char t_tidString[32];
        __thread int t_tidStringLength = 6;
        __thread const char* t_threadName = "unknown";
        static_assert((std::is_same<int,pid_t>::value),"pid should be int");

        std::string stackTrace(bool demangle){
            std::string stack;
            const int max_frames = 200;
            void* frame[max_frames];
            // backtrace，栈回溯，用来保存各个栈帧的地址
            // 使用man backtrace查看函数具体信息
            // 两个参数第一个参数是接收栈帧地址的指针，类型为void**
            // 第二个参数是最大容纳的栈帧个数，如果实际栈帧大于的话执行截断操作
            // 返回值为栈帧数组实际容纳的栈帧个数
            int nptrs = ::backtrace(frame,max_frames);
            // backstrace_symbols将地址转换成函数
            // 返回的strings是一个指针数据，数组中的每个指针指向的是一个字符串
            // 字符串的内容是经过编译器改变过的函数名称
            char** strings = ::backtrace_symbols(frame,nptrs);
            if(strings){
                size_t len = 256;
                // 因为编译器对函数名称做了改编，如果要将函数名称改回来的话可以将这个选项打开
                // abi::__cxa_demangle的作用是将函数名称还原
                char* demangled = demangle ? static_cast<char*>(::malloc(len)):nullptr;
                for(int i = 1; i < nptrs; ++i){
                    if(demangle){
                        char* left_par = nullptr;
                        char* plus = nullptr;
                        for(char* p = strings[i]; *p; ++p){
                            if(*p == '(')
                                left_par = p;
                            else if(*p =='+')
                                plus = p;
                        }
                        if(left_par && plus){
                            *plus = '\0';
                            int status = 0;
                            char* ret = abi::__cxa_demangle(left_par+1,demangled,&len,&status);
                            *plus = '+';
                            if(status == 0){
                                demangled = ret;
                                stack.append(strings[i],left_par+1);
                                stack.append(demangled);
                                stack.append(plus);
                                stack.push_back('\n');
                                continue;
                            }
                        }
                    }
                    // 这里用到的append和push_back的区别主要是
                    // append可以对字符串进行操作
                    // 而push_back只能对一个字符进行操作
                    stack.append(strings[i]);
                    stack.push_back('\n');
                }
                // 这里需要强调的是free一个空指针不会有任何问题，
                // 如果一个指针是null，free它编译器什么也不会做
                // 要注意的是连续释放一段内存区域两次则会有问题
                // 这也是为什么会有free后将指针置为null这一编程习惯的由来
                free(demangled);
                // 这边需要注意的是在文档中有说
                // This  array  is malloced  by backtrace_symbols()
                // and must be freed by the caller，也就是要手动释放strings这一数组
                free(strings);
            }
            return stack;
        }

    }
}
