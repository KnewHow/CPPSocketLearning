#include "gtest/gtest.h"

#include <glog/logging.h>
#include "dao/customer.h"
#include "service/customerService.h"
#include "common/stringprint.h"
#include "service/globalService.h"
#include <string.h>

using namespace server_client;


TEST(Customer, INSERT) {
    std::string phoneNumber = "18326600931";
    globalService::customervice.deleteByPhoneNumber(phoneNumber);
    Customer customer(phoneNumber, "123456", 1, 26, "Knewhow");
    int r = globalService::customervice.insert(customer);
    EXPECT_EQ(r, 1);
}

TEST(Customer, INSERTWITHCHINESE) {
    std::string phoneNumber = "18326600933";
    globalService::customervice.deleteByPhoneNumber(phoneNumber);
    Customer customer(phoneNumber, "123456", 1, 26, "袁国浩");
    int r = globalService::customervice.insert(customer);
    EXPECT_EQ(r, 1);
}

TEST(Customer, DELETE) {
    std::string phoneNumber = "18326600932";
    globalService::customervice.deleteByPhoneNumber(phoneNumber);
    Customer customer(phoneNumber, "1234567", 1, 26, "Tom");
    int r1 = globalService::customervice.insert(customer);
    int r2 = globalService::customervice.deleteByPhoneNumber(phoneNumber);
    EXPECT_EQ(r1, 1);
    EXPECT_EQ(r2, 1);
}

TEST(Customer, SELECT_BY_PHONENUMBER) {
    std::string phoneNumber = "18326600933";
    globalService::customervice.deleteByPhoneNumber(phoneNumber);
    Customer customer(phoneNumber, "123456", 1, 26, "袁国浩");
    globalService::customervice.insert(customer);
    const std::optional<Customer> result = globalService::customervice.selectByPhoneNumber(phoneNumber);
    if(result.has_value()) {
        LOG(INFO) << "Customer id: " << result.value().id << ", phone_number: " << result.value().phone_number << ", username:" << result.value().username;
    }
    EXPECT_TRUE(result.has_value());
}

TEST(Customer, SELECT_BY_SEX) {
    globalService::customervice.deleteAll();
    Customer c1("18326600931", "123456", 1, 26, "袁国浩");
    Customer c2("18326600932", "123456", 1, 26, "Knewhow");
    globalService::customervice.insert(c1);
    globalService::customervice.insert(c2);
    std::vector<Customer> cs = globalService::customervice.selectBySex(1);
    EXPECT_EQ(cs.size(), 2);
}

TEST(Customer, UPDATE_USERNAME_BY_ID) {
    std::string phoneNumber = "18326600933";
    globalService::customervice.deleteByPhoneNumber(phoneNumber);
    Customer customer(phoneNumber, "123456", 1, 26, "袁国浩");
    globalService::customervice.insert(customer);
    const std::optional<Customer> result = globalService::customervice.selectByPhoneNumber(phoneNumber);
    EXPECT_TRUE(result.has_value());
    if(result.has_value()){
        int r = globalService::customervice.updateUsernameById(result.value().id, "袁国昊");
        EXPECT_EQ(r, 1);
    }
}

