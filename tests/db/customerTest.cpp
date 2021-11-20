#include "gtest/gtest.h"
#include "_postgresql.h"

#include <glog/logging.h>
#include "dao/customer.h"
#include "common/timeUtil.h"
#include "service/customerService.h"
#include "common/stringprint.h"
#include <string.h>

using namespace server_client;

CustomerService customerService;

TEST(Customer, INSERT) {
    std::string phoneNumber = "18326600931";
    customerService.deleteByPhoneNumber(phoneNumber);
    Customer customer(phoneNumber, "123456", 1, 26, "Knewhow");
    int r = customerService.insert(customer);
    EXPECT_EQ(r, 1);
}

TEST(Customer, INSERTWITHCHINESE) {
    std::string phoneNumber = "18326600933";
    customerService.deleteByPhoneNumber(phoneNumber);
    Customer customer(phoneNumber, "123456", 1, 26, "袁国浩");
    int r = customerService.insert(customer);
    EXPECT_EQ(r, 1);
}

TEST(Customer, DELETE) {
    std::string phoneNumber = "18326600932";
    customerService.deleteByPhoneNumber(phoneNumber);
    Customer customer(phoneNumber, "1234567", 1, 26, "Tom");
    int r1 = customerService.insert(customer);
    int r2 = customerService.deleteByPhoneNumber(phoneNumber);
    EXPECT_EQ(r1, 1);
    EXPECT_EQ(r2, 1);
}

TEST(Customer, SELECT_BY_PHONENUMBER) {
    std::string phoneNumber = "18326600933";
    customerService.deleteByPhoneNumber(phoneNumber);
    Customer customer(phoneNumber, "123456", 1, 26, "袁国浩");
    customerService.insert(customer);
    const std::optional<Customer> result = customerService.selectByPhoneNumber(phoneNumber);
    if(result.has_value()) {
        LOG(INFO) << "Customer id: " << result.value().id << ", phone_number: " << result.value().phone_number << ", username:" << result.value().username;
    }
    EXPECT_TRUE(result.has_value());
}

TEST(Customer, SELECT_BY_SEX) {
    customerService.deleteAll();
    Customer c1("18326600931", "123456", 1, 26, "袁国浩");
    Customer c2("18326600932", "123456", 1, 26, "Knewhow");
    customerService.insert(c1);
    customerService.insert(c2);
    std::vector<Customer> cs = customerService.selectBySex(1);
    EXPECT_EQ(cs.size(), 2);
}

TEST(Customer, UPDATE_USERNAME_BY_ID) {
    std::string phoneNumber = "18326600933";
    customerService.deleteByPhoneNumber(phoneNumber);
    Customer customer(phoneNumber, "123456", 1, 26, "袁国浩");
    customerService.insert(customer);
    const std::optional<Customer> result = customerService.selectByPhoneNumber(phoneNumber);
    EXPECT_TRUE(result.has_value());
    if(result.has_value()){
        int r = customerService.updateUsernameById(result.value().id, "袁国昊");
        EXPECT_EQ(r, 1);
    }
}

