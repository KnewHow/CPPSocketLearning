#include "customerService.h"

#include "glog/logging.h"
#include "_postgresql.h"
#include "common/dbconfig.h"
#include "common/stringprint.h"

namespace server_client {
int CustomerService::insert(const Customer &customer) const  {
    connection conn; // 数据库连接类

    // 登录数据库，返回值：0-成功，其它-失败。
    // 失败代码在conn.m_cda.rc中，失败描述在conn.m_cda.message中。

    if (conn.connecttodb(config.c_str(), charset.c_str())!=0)
    {
        LOG(ERROR) << StringPrintf("connect database failed.\n%s\n",conn.m_cda.message);
        return 0 ;
    }

    sqlstatement stmt(&conn); // 操作SQL语句的对象。

    // 准备插入表的SQL语句。
    stmt.prepare("\
        insert into customer(phone_number, username,password, sex, age, status, modified_time, created_time) \
                    values(:1, :2, :3, :4, :5, :6, to_date(:7,'yyyy-mm-dd hh24:mi:ss'), to_date(:8,'yyyy-mm-dd hh24:mi:ss'))");

    stmt.bindin(1, customer.phone_number, sizeof(customer.phone_number));
    stmt.bindin(2, customer.username, sizeof(customer.username));
    stmt.bindin(3 ,customer.password, sizeof(customer.password));
    stmt.bindin(4, &customer.sex);
    stmt.bindin(5, &customer.age);
    stmt.bindin(6, &customer.status);
    stmt.bindin(7, customer.modified_time, sizeof(customer.modified_time));
    stmt.bindin(8, customer.created_time, sizeof(customer.created_time));

    if (stmt.execute() != 0)
    {
      LOG(ERROR) << StringPrintf("stmt.execute() failed.\n%s\n%s\n",stmt.m_sql,stmt.m_cda.message);
      return 0;
    }
    conn.commit();
    return stmt.m_cda.rpc;
}

int CustomerService::deleteByPhoneNumber(const std::string& phoneNumber) const {
    connection conn;

    if (conn.connecttodb(config.data(), charset.data())!=0)
    {
        LOG(ERROR) << StringPrintf("connect database failed.\n%s\n",conn.m_cda.message);
        return 0 ;
    }

    sqlstatement stmt(&conn); // 操作SQL语句的对象。
    
    stmt.prepare("delete from customer where phone_number=:1");
    stmt.bindin(1, phoneNumber.c_str(), phoneNumber.size());

    if (stmt.execute() != 0)
    {
      LOG(ERROR) << StringPrintf("stmt.execute() failed.\n%s\n%s\n",stmt.m_sql,stmt.m_cda.message);
      return 0;
    }
    conn.commit();
    return stmt.m_cda.rpc;

}

std::optional<Customer>  CustomerService::selectByPhoneNumber(const std::string& phoneNumber) const {
    connection conn;

    if (conn.connecttodb(config.data(), charset.data())!=0)
    {
        LOG(ERROR) << StringPrintf("connect database failed.\n%s\n",conn.m_cda.message);
        return std::nullopt ;
    }

    sqlstatement stmt(&conn); // 操作SQL语句的对象。
    
    stmt.prepare("select * from customer where phone_number=:1");
    stmt.bindin(1, phoneNumber.c_str(), phoneNumber.size());

    Customer customer;
    stmt.bindout(1, &customer.id);
    stmt.bindout(2, customer.phone_number, sizeof(customer.phone_number));
    stmt.bindout(3, customer.password, sizeof(customer.password));
    stmt.bindout(4, customer.username, sizeof(customer.username));
    stmt.bindout(5, &customer.sex);
    stmt.bindout(6, &customer.age);
    stmt.bindout(7, &customer.status);
    stmt.bindout(8, customer.modified_time, sizeof(customer.modified_time));
    stmt.bindout(9, customer.created_time, sizeof(customer.created_time));
    
    if (stmt.execute() != 0)
    {
      LOG(ERROR) << StringPrintf("stmt.execute() failed.\n%s\n%s\n",stmt.m_sql,stmt.m_cda.message);
      return std::nullopt;
    }

    if(stmt.next() == 0) {
        return customer;
    }
    return std::nullopt;
}

std::vector<Customer> CustomerService::selectBySex(int sex) const {
    connection conn;
    std::vector<Customer> rs;
    if (conn.connecttodb(config.data(), charset.data())!=0)
    {
        LOG(ERROR) << StringPrintf("connect database failed.\n%s\n",conn.m_cda.message);
        return rs;
    }

    sqlstatement stmt(&conn); // 操作SQL语句的对象。
    
    stmt.prepare("select* from customer where sex=:1");
    stmt.bindin(1, &sex);

    Customer customer;
    stmt.bindout(1, &customer.id);
    stmt.bindout(2, customer.phone_number, sizeof(customer.phone_number));
    stmt.bindout(3, customer.password, sizeof(customer.password));
    stmt.bindout(4, customer.username, sizeof(customer.username));
    stmt.bindout(5, &customer.sex);
    stmt.bindout(6, &customer.age);
    stmt.bindout(7, &customer.status);
    stmt.bindout(8, customer.modified_time, sizeof(customer.modified_time));
    stmt.bindout(9, customer.created_time, sizeof(customer.created_time));

    if (stmt.execute() != 0)
    {
      LOG(ERROR) << StringPrintf("stmt.execute() failed.\n%s\n%s\n",stmt.m_sql,stmt.m_cda.message);
      return rs;
    }

    while(1) {
        if(stmt.next() !=0) break;
        rs.push_back(customer);

    }
    return rs;
}

int CustomerService::deleteAll() const {
    connection conn;
    if (conn.connecttodb(config.data(), charset.data())!=0)
    {
        LOG(ERROR) << StringPrintf("connect database failed.\n%s\n",conn.m_cda.message);
        return 0;
    }

    sqlstatement stmt(&conn); // 操作SQL语句的对象。
    stmt.prepare("delete from customer");

    if (stmt.execute() != 0)
    {
      LOG(ERROR) << StringPrintf("stmt.execute() failed.\n%s\n%s\n",stmt.m_sql,stmt.m_cda.message);
      return 0;
    }
    conn.commit();
    return stmt.m_cda.rpc;
}

int CustomerService::updateUsernameById(long id, const std::string &username) const {
    connection conn;
    if (conn.connecttodb(config.data(), charset.data())!=0)
    {
        LOG(ERROR) << StringPrintf("connect database failed.\n%s\n",conn.m_cda.message);
        return 0;
    }

    sqlstatement stmt(&conn); // 操作SQL语句的对象。
    stmt.prepare("update customer set username = :1 where id = :2");
    stmt.bindin(1, username.c_str(), username.size());
    stmt.bindin(2, &id);
    if (stmt.execute() != 0)
    {
      LOG(ERROR) << StringPrintf("stmt.execute() failed.\n%s\n%s\n",stmt.m_sql,stmt.m_cda.message);
      return 0;
    }
    conn.commit();
    return stmt.m_cda.rpc;
}

std::optional<Customer> CustomerService::login(const std::string &phone_number, const std::string &pass) const {
    auto customer = selectByPhoneNumber(phone_number);
    if(!customer.has_value()) return std::nullopt;
    if(customer.value().password == pass) return customer;
    else                                  return std::nullopt;
}

} // namespace server_client
