/********************************************************************************* 
  * FileName: Timetamp.h
  * Date 2020/4/2
  * Description: 时间戳 值语意
**********************************************************************************/

#ifndef ANNOTATE_MUDUO_TIMESTAMP_H
#define ANNOTATE_MUDUO_TIMESTAMP_H

#include "Types.h"

namespace annotate_muduo{
    class Timestamp {
    public:
        // 无参构造函数
        Timestamp()
        :microSecondsSinceEpoch_(0){}
        // 有参构造函数，为了放置隐式转换，在方法前加explicit关键字
        explicit Timestamp(int64_t microSecondsSinceEpochArg)
        : microSecondsSinceEpoch_(microSecondsSinceEpochArg){}

        // 实现自定义swap
        void swap(Timestamp& that){
            std::swap(microSecondsSinceEpoch_,that.microSecondsSinceEpoch_);
        }

        std::string toString() const;
        std::string toFormattedString(bool showMicroSeconds = true) const;

        bool valid()const{return microSecondsSinceEpoch_ > 0;}

        int64_t microSecondsSinceEpoch()const{
            return microSecondsSinceEpoch_;
        }

        time_t secondsSinceEpoch()const{
            return static_cast<time_t >(microSecondsSinceEpoch_ / kMicroSecondsPerSecond);
        }

        // 获得现在的时间
        static Timestamp now();
        static Timestamp invalid(){
            return Timestamp();
        }

        // 根据unix系统的时间获得一个Timestamp对象
        static Timestamp fromUnixTime(time_t t){
            return fromUnixTime(t,0);
        }

        static Timestamp fromUnixTime(time_t t, int microseconds){
            return Timestamp(static_cast<int64_t >(t) * kMicroSecondsPerSecond
            + microseconds);
        }

    static const int kMicroSecondsPerSecond = 1000 * 1000;
    private:
        int64_t microSecondsSinceEpoch_;

    };

    // 运算符重载
    inline bool operator <(Timestamp lhs, Timestamp rhs){
        return lhs.microSecondsSinceEpoch() < rhs.microSecondsSinceEpoch();
    }
    // 运算符重载
    inline bool operator ==(Timestamp lhs, Timestamp rhs){
        return lhs.microSecondsSinceEpoch() == rhs.microSecondsSinceEpoch();
    }

    // 比较两个时间戳大小，差异单位为秒
    inline double timeDifferences(Timestamp high, Timestamp low){
        int64_t diff = high.microSecondsSinceEpoch() - low.microSecondsSinceEpoch();
        return static_cast<double >(diff) / Timestamp::kMicroSecondsPerSecond;
    }

    inline Timestamp addTime(Timestamp timestamp, double seconds){
        auto delta = static_cast<int64_t >(seconds * Timestamp::kMicroSecondsPerSecond);
        return Timestamp(timestamp.microSecondsSinceEpoch()+delta);
    }



}



#endif //ANNOTATE_MUDUO_TIMESTAMP_H
