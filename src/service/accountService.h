#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#ifndef SRC_SERVICE_ACCOUNTSERVICE_H_
#define SRC_SERVICE_ACCOUNTSERVICE_H_

#include <optional>

#include "dao/account.h"

class connection;

namespace server_client {

class AccountService {
public:
    int insert(const Account &account) const;
    bool doTrade(long source_constomer_id, long dest_customer_id, long money) const;
    std::optional<Account> selectByCustomerId(long customer_id) const;
    int deleteAll() const;
private:
    int minusMoneyById(connection &conn, long id, long money) const;
    int addMoneyById(connection &conn, long id, long money) const;
};

} // namespace server_client

#endif // SRC_SERVICE_ACCOUNTSERVICE_H_