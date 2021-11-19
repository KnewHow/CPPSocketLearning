#include "gtest/gtest.h"
#include "_postgresql.h"

#include <glog/logging.h>
#include "dao/customer.h"
#include "common/timeUtil.h"
#include "service/customerService.h"

using namespace server_client;

CustomerService customerService;

TEST(Postgresql, Insert) {
    Customer customer;
    customer.username = "Knewhow";
    customer.password = "123456";
    customer.sex = 1;
    customer.age = 26;
    customer.modified_time = getNowStr();
    customer.created_time = getNowStr();
    customerService.insert(customer);
}

TEST(Postgresql, INSERTWithTransaction) {

}

