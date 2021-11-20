#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#ifndef SRC_SERVICE_ACCOUNT_H_
#define SRC_SERVICE_ACCOUNT_H_

class Account {
public:
    long id;
    long customer_id;
    long money; // how money the account
    int type; // 
    int status;
};

#endif // SRC_SERVICE_ACCOUNT_H_