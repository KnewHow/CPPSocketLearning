#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#ifndef SRC_SERVICE_ACCOUNT_H_
#define SRC_SERVICE_ACCOUNT_H_

#include "common/timeUtil.h"
#include <string.h>

namespace server_client {

class Account {
public:
    Account(){}
    Account(long customer_id, long money = 0, int type = 1, int status = 0) 
    :customer_id(customer_id), money(money), type(type), status(status){
        std::string now = getNowStr();
        strcpy(created_time, now.c_str());
        strcpy(modified_time, now.c_str());
    }
    long id;
    long customer_id;
    long money; // how money the account, 单位：分
    int type; // account type, default 1:normal
    int status; // account status 0:normal 1:freeze
    char modified_time[30];
    char created_time[30];
};
} // namespace server_client



#endif // SRC_SERVICE_ACCOUNT_H_