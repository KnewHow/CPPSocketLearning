#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#ifndef SRC_COMMON_DBCONFIG_H_
#define SRC_COMMON_DBCONFIG_H_

#include <string>

namespace server_client {

    static std::string config = "host=127.0.0.1 user=postgres password=postgres dbname=socket_test port=5432";
    static std::string charset = "utf8";
} // namespace server_client
#endif // SRC_COMMON_DBCONFIG_H_