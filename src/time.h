#ifndef _TIME_
#define _TIME_

#include <chrono>
#include <iostream>
#include <ctime>
#include <iomanip>

class Timestamp {
public:
    // 获取当前时间戳（毫秒）
    static int64_t getTimestampMs() {
        auto now = std::chrono::system_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()).count();
    }
    
    // 获取当前时间戳（秒）
    static int64_t getTimestamp() {
        auto now = std::chrono::system_clock::now();
        return std::chrono::duration_cast<std::chrono::seconds>(
            now.time_since_epoch()).count();
    }
    
    // 获取高精度时间戳（微秒）
    static int64_t getTimestampUs() {
        auto now = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::microseconds>(
            now.time_since_epoch()).count();
    }
    
    // 格式化时间字符串
    static std::string getFormattedTime(const std::string& format = "%Y-%m-%d %H:%M:%S") {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        
        std::tm tm;
        #ifdef _WIN32
            localtime_s(&tm, &time_t);
        #else
            localtime_r(&time_t, &tm);
        #endif
        
        std::stringstream ss;
        ss << std::put_time(&tm, format.c_str());
        return ss.str();
    }
};

#endif