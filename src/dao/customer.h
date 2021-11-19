#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#ifndef SRC_DAO_CUSTOMER_H_
#define SRC_DAO_CUSTOMER_H_

#include <string>

namespace server_client {

class Customer {
public:
    long id;
    std::string username;
    std::string password;
    int sex;
    int age;
    std::string modified_time;
    std::string created_time;
};

} // namespace server_client

#endif // SRC_DAO_CUSTOMER_H_