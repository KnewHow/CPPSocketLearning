#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#ifndef SRC_DAO_CUSTOMER_H_
#define SRC_DAO_CUSTOMER_H_

#include <string>
#include "common/timeUtil.h"
#include <string.h>

namespace server_client {

class Customer {
public:
    Customer(){
        std::string now = getNowStr();
        strcpy(created_time, now.c_str());
        strcpy(modified_time, now.c_str());
        
    }
    Customer(const std::string& phone, const std::string &pass,
        int sex, int age, const std::string &uname = "", int status = 0)
        :sex(sex), age(age), status(status) {
        strcpy(phone_number, phone.c_str());
        strcpy(password, pass.c_str());
        strcpy(username, uname.c_str());
        std::string now = getNowStr();
        strcpy(created_time, now.c_str());
        strcpy(modified_time, now.c_str());
    }
    long id;
    char phone_number[12];
    char username[255];
    char password[255];
    int sex;
    int age;
    int status;
    char modified_time[30];
    char created_time[30];
};

} // namespace server_client

#endif // SRC_DAO_CUSTOMER_H_