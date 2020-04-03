/********************************************************************************* 
  * FileName: Timetamp.cpp
  * Date 2020/4/2
  * Description: 
**********************************************************************************/

#include "Timestamp.h"

#include <sys/time.h>
#include <cstdio>

// 下面的__STDC_FORMAT_MACROS 和<cinttypes>都是为了正常使用PRId64这个宏
#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif

#include <cinttypes>
using namespace annotate_muduo;

static_assert(sizeof(Timestamp) == sizeof(int64_t),
        "Timestamp is same size as int64_t");

std::string Timestamp::toString() const {
    char buf[32] = {0};
    int64_t seconds = microSecondsSinceEpoch_ / kMicroSecondsPerSecond;
    int64_t microseconds = microSecondsSinceEpoch_ % kMicroSecondsPerSecond;

    // 因为int64_t在各个平台表示方法可能不一致，如有的是lld有的是ld等，替换成宏PRId64不用考虑平台问题
    // snprintf()函数可以看作是对sprintf()方法的一种改进，因为指定了字符串大小，不会造成缓冲区溢出
    snprintf(buf,sizeof(buf)-1,"%" PRId64 ".%06" PRId64 "", seconds,microseconds);
    return buf;
}

std::string Timestamp::toFormattedString(bool showMicroSeconds) const {
    char buf[64] = {0};
    auto seconds = static_cast<time_t >(microSecondsSinceEpoch_ / kMicroSecondsPerSecond);
    struct tm tm_time{};
    gmtime_r(&seconds,&tm_time);
    if (showMicroSeconds)
    {
        int microseconds = static_cast<int>(microSecondsSinceEpoch_ % kMicroSecondsPerSecond);
        snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d.%06d",
                 tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
                 tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec,
                 microseconds);
    }
    else
    {
        snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d",
                 tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
                 tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
    }

    return buf;
}

Timestamp Timestamp::now() {
    // tv是<sys/time.h>定义的一个结构体，包含两个内容，一个是tv_sec表示秒，另一个是tv_usec表示微秒
    struct timeval tv;

    gettimeofday(&tv,NULL);
    int64_t seconds = tv.tv_sec;
    return Timestamp(seconds * kMicroSecondsPerSecond + tv.tv_usec);
}



