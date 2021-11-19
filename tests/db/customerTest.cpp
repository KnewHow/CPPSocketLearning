#include "gtest/gtest.h"
#include "_postgresql.h"

#include <glog/logging.h>
#include "dao/customer.h"
#include "common/timeUtil.h"
#include "service/customerService.h"

using namespace server_client;

CustomerService customerService;



TEST(Customer, INSERT) {
    Customer customer;
    customer.username = "Knewhow";
    customer.password = "123456";
    customer.sex = 1;
    customer.age = 26;
    customer.modified_time = getNowStr();
    customer.created_time = getNowStr();
    int r = customerService.insert(customer);
    EXPECT_EQ(r, 1);
}

TEST(Customer, DELETE) {
    std::string username = "Knewhow";
    Customer customer;
    customer.username = username;
    customer.password = "123456";
    customer.sex = 1;
    customer.age = 26;
    customer.modified_time = getNowStr();
    customer.created_time = getNowStr();
    int r1 = customerService.insert(customer);
    int r2 = customerService.deleteByUsername(username);
    EXPECT_EQ(r1, 1);
    EXPECT_EQ(r2, 1);
}

