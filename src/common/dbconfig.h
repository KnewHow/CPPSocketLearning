#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#ifndef SRC_COMMON_DBCONFIG_H_
#define SRC_COMMON_DBCONFIG_H_

#include <string>

namespace server_client {
class DBConfig{
public:
    DBConfig(const std::string &properties);
    static std::string config;
    static std::string charset;
};


    
} // namespace server_client
#endif // SRC_COMMON_DBCONFIG_H_