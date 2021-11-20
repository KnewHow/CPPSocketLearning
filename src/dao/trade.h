#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#ifndef SRC_DAO_TRADE_H_
#define SRC_DAO_TRADE_H_

#include <string.h>

#include "common/timeUtil.h"


namespace server_client {

class Trade {
public:
    Trade(){}
    Trade(long source_account_id, long dest_account_id, long money, int status = 0)
    :source_account_id(source_account_id), dest_account_id(dest_account_id), money(money), status(status) {
        std::string now = getNowStr();
        strcpy(created_time, now.c_str());
        strcpy(modified_time, now.c_str());
    }
    long id;
    long source_account_id;
    long dest_account_id;
    long money;
    int status; // 0:Normal
    char modified_time[30];
    char created_time[30];
};

} // namespace server_client

#endif // SRC_DAO_TRADE_H_