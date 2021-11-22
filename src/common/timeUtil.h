#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#ifndef SRC_COMMON_TIMEUTIL_H_
#define SRC_COMMON_TIMEUTIL_H_

#include <cstdio>
#include <ctime>
#include <string>
#include <chrono>

namespace server_client {

inline std::string getNowStr() {
    std::time_t rawtime;
    std::tm* timeinfo;
    char buffer [80];

    std::time(&rawtime);
    timeinfo = std::localtime(&rawtime);

    std::strftime(buffer,80,"%Y-%m-%d %H:%M:%S",timeinfo);
    return std::string(buffer);
}

inline std::chrono::milliseconds getCurrentMillseconds() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch());
}
} // namespace server_client




#endif // SRC_COMMON_TIMEUTIL_H_