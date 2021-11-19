#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#ifndef SRC_SERVICE_CUSTOMERSERVICE_H_
#define SRC_SERVICE_CUSTOMERSERVICE_H_

#include "dao/customer.h"

namespace server_client {

class CustomerService {
public:
    int insert(Customer &customer);
    int deleteByUsername(std::string& username);
};

} // namespace server_client

#endif // SRC_SERVICE_CUSTOMERSERVICE_H_