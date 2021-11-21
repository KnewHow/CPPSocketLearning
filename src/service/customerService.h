#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#ifndef SRC_SERVICE_CUSTOMERSERVICE_H_
#define SRC_SERVICE_CUSTOMERSERVICE_H_

#include "dao/customer.h"
#include <optional>
#include <vector>

namespace server_client {

class CustomerService {
public:
    int insert(const Customer &customer) const ;
    int deleteByPhoneNumber(const std::string& phoneNumber) const;
    std::optional<Customer> selectByPhoneNumber(const std::string& phoneNumber) const;
    std::vector<Customer> selectBySex(int sex) const;
    int deleteAll() const;
    int updateUsernameById(long id, const std::string &username) const;
    std::optional<Customer> login(const std::string &phone_number, const std::string &pass) const;
};

} // namespace server_client

#endif // SRC_SERVICE_CUSTOMERSERVICE_H_