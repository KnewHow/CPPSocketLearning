#include "gtest/gtest.h"

#include <glog/logging.h>
#include "dao/customer.h"
#include "dao/account.h"
#include "service/customerService.h"
#include "common/stringprint.h"
#include "service/globalService.h"
#include <string.h>

TEST(Account, AllocateMoney) {
    globalService::customervice.deleteAll();
    Customer c1("18326600931", "123456", 1, 26, "袁国浩");
    Customer c2("18326600932", "123456", 1, 26, "Knewhow");
    globalService::customervice.insert(c1);
    globalService::customervice.insert(c2);
    std::vector<Customer> cs = globalService::customervice.selectBySex(1);
    EXPECT_EQ(cs.size(), 2);
    Account acc1(cs[0].id, 100);
    Account acc2(cs[1].id, 0);
    int r1 = globalService::accountServive.insert(acc1);
    int r2 = globalService::accountServive.insert(acc2);
    EXPECT_EQ(r1, 1);
    EXPECT_EQ(r2, 1);
}

TEST(Account, DoTrade) {
    globalService::customervice.deleteAll();
    globalService::accountServive.deleteAll();
    globalService::tradeService.deleteAll();
    Customer c1("18326600931", "123456", 1, 26, "袁国浩");
    Customer c2("18326600932", "123456", 1, 26, "Knewhow");
    globalService::customervice.insert(c1);
    globalService::customervice.insert(c2);
    std::vector<Customer> cs = globalService::customervice.selectBySex(1);
    EXPECT_EQ(cs.size(), 2);
    int totalMoney = 100;
    int tradeMoney = 130;
    Account acc1(cs[0].id, totalMoney);
    Account acc2(cs[1].id, 0);
    int r1 = globalService::accountServive.insert(acc1);
    int r2 = globalService::accountServive.insert(acc2);
    EXPECT_EQ(r1, 1);
    EXPECT_EQ(r2, 1);
    bool r = globalService::accountServive.doTrade(cs[0].id, cs[1].id, tradeMoney);
    if(tradeMoney <= totalMoney) {
        EXPECT_TRUE(r);
    } else {
        EXPECT_FALSE(r);
    }
}