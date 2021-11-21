#include "accountService.h"
#include "glog/logging.h"
#include "_postgresql.h"
#include "common/dbconfig.h"
#include "common/stringprint.h"
#include "dao/trade.h"
#include "service/tradeService.h"
#include "globalService.h"

namespace server_client {

int AccountService::insert(const Account &account) const {
    connection conn;

    if (conn.connecttodb(config.data(), charset.data())!=0)
    {
        LOG(ERROR) << StringPrintf("connect database failed.\n%s\n",conn.m_cda.message);
        return 0 ;
    }

    sqlstatement stmt(&conn);
    stmt.prepare("insert into account(customer_id, money, type, status, modified_time, created_time) values (:1, :2, :3, :4, to_date(:5,'yyyy-mm-dd hh24:mi:ss'), to_date(:6,'yyyy-mm-dd hh24:mi:ss'))");
    stmt.bindin(1, &account.customer_id);
    stmt.bindin(2, &account.money);
    stmt.bindin(3, &account.type);
    stmt.bindin(4, &account.status);
    stmt.bindin(5, account.modified_time, sizeof(account.modified_time));
    stmt.bindin(6, account.created_time, sizeof(account.created_time));

    if (stmt.execute() != 0)
    {
      LOG(ERROR) << StringPrintf("stmt.execute() failed.\n%s\n%s\n",stmt.m_sql,stmt.m_cda.message);
      return 0;
    }
    conn.commit();
    return stmt.m_cda.rpc;
}

bool AccountService::doTrade(long source_constomer_id, long dest_customer_id, long money) const {
    connection conn;
    auto sourceAcc = selectByCustomerId(source_constomer_id);
    auto destctAcc = selectByCustomerId(dest_customer_id);
    if(!sourceAcc.has_value() || !destctAcc.has_value()) {
        LOG(INFO) << "source account or dest account not exist";
        return false;
    }
    int r1 = minusMoneyById(conn, sourceAcc.value().id, money);
    if(r1 > 0) {
        int r2 = addMoneyById(conn, destctAcc.value().id, money);
        if(r2 > 0) {
            conn.commit();
            Trade t(sourceAcc.value().id, destctAcc.value().id, money);
            globalService::tradeService.insert(t);
            return true;
        } else {
            LOG(INFO) << "add money failure";
            return false;
            conn.rollback();
        }
    }
    LOG(INFO) << "No enough money to trade";
    return false;
}

std::optional<Account> AccountService::selectByCustomerId(long customer_id) const {
    connection conn;

    if (conn.connecttodb(config.data(), charset.data())!=0)
    {
        LOG(ERROR) << StringPrintf("connect database failed.\n%s\n",conn.m_cda.message);
        return std::nullopt ;
    }

    sqlstatement stmt(&conn);

    Account account;
    stmt.prepare("select * from account where customer_id = :1");
    stmt.bindin(1, &customer_id);
    stmt.bindout(1, &account.id);
    stmt.bindout(2, &account.customer_id);
    stmt.bindout(3, &account.money);
    stmt.bindout(4, &account.type);
    stmt.bindout(5, &account.status);
    stmt.bindout(6, account.modified_time, sizeof(account.modified_time));
    stmt.bindout(7, account.created_time, sizeof(account.created_time));

    if (stmt.execute() != 0)
    {
      LOG(ERROR) << StringPrintf("stmt.execute() failed.\n%s\n%s\n",stmt.m_sql,stmt.m_cda.message);
      return std::nullopt;
    }

    if(stmt.next() == 0) {
        return account;
    }
    return std::nullopt;

}

int AccountService::deleteAll() const {
    connection conn;

    if (conn.connecttodb(config.data(), charset.data())!=0)
    {
        LOG(ERROR) << StringPrintf("connect database failed.\n%s\n",conn.m_cda.message);
        return 0;
    }

    sqlstatement stmt(&conn);
    stmt.prepare("delete from account");
    if (stmt.execute() != 0)
    {
      LOG(ERROR) << StringPrintf("stmt.execute() failed.\n%s\n%s\n",stmt.m_sql,stmt.m_cda.message);
      return 0;
    }
    conn.commit();
    return stmt.m_cda.rpc;
}

int AccountService::recharge(long customer_id, long money) const {
    auto account = selectByCustomerId(customer_id);
    if(account.has_value()) {
        connection conn;
        if (conn.connecttodb(config.data(), charset.data())!=0)
        {
            LOG(ERROR) << StringPrintf("connect database failed.\n%s\n",conn.m_cda.message);
            return 0;
        }
        int r = addMoneyById(conn, account.value().id, money);
        conn.commit();
        return r;
    } else {
        Account newAccount = Account(customer_id, money);
        return insert(newAccount);
    }
}


int AccountService::minusMoneyById(connection &conn, long id, long money) const {

    if (conn.connecttodb(config.data(), charset.data())!=0)
    {
        LOG(ERROR) << StringPrintf("connect database failed.\n%s\n",conn.m_cda.message);
        return 0 ;
    }

    sqlstatement stmt(&conn);

    stmt.prepare("update account set money = money - :1 where id = :2 AND money >= :3");
    stmt.bindin(1, &money);
    stmt.bindin(2, &id);
    stmt.bindin(3, &money);
    if (stmt.execute() != 0)
    {
      LOG(ERROR) << StringPrintf("stmt.execute() failed.\n%s\n%s\n",stmt.m_sql,stmt.m_cda.message);
      return 0;
    }
    return stmt.m_cda.rpc;
}

int AccountService::addMoneyById(connection &conn, long id, long money) const {
    if (conn.connecttodb(config.data(), charset.data())!=0)
    {
        LOG(ERROR) << StringPrintf("connect database failed.\n%s\n",conn.m_cda.message);
        return 0 ;
    }

    sqlstatement stmt(&conn);

    stmt.prepare("update account set money = money + :1 where id = :2");
    stmt.bindin(1, &money);
    stmt.bindin(2, &id);
    if (stmt.execute() != 0)
    {
      LOG(ERROR) << StringPrintf("stmt.execute() failed.\n%s\n%s\n",stmt.m_sql,stmt.m_cda.message);
      return 0;
    }
    return stmt.m_cda.rpc;
}

} // namespace server_client

