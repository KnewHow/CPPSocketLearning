#include "customerService.h"

#include "glog/logging.h"
#include "_postgresql.h"
#include "common/dbconfig.h"

namespace server_client {
int CustomerService::insert(Customer &customer) {
    connection conn; // 数据库连接类

    // 登录数据库，返回值：0-成功，其它-失败。
    // 失败代码在conn.m_cda.rc中，失败描述在conn.m_cda.message中。

    if (conn.connecttodb(config.data(), charset.data())!=0)
    {
        printf("connect database failed.\n%s\n",conn.m_cda.message);
        return 0 ;
    }

     sqlstatement stmt(&conn); // 操作SQL语句的对象。

    // 准备插入表的SQL语句。
    stmt.prepare("\
        insert into customer(username,password,sex, age, modified_time, created_time) \
                    values(:1, :2, :3, :4, to_date(:5,'yyyy-mm-dd hh24:mi:ss'), to_date(:6,'yyyy-mm-dd hh24:mi:ss'))");



    stmt.bindin(1, customer.username.data(), customer.username.size());
    stmt.bindin(2 ,customer.password.data(), customer.password.size());
    stmt.bindin(3, &customer.sex);
    stmt.bindin(4, &customer.age);
    stmt.bindin(5, customer.modified_time.data(), customer.modified_time.size());
    stmt.bindin(6, customer.created_time.data(), customer.created_time.size());

    if (stmt.execute() != 0)
    {
      printf("stmt.execute() failed.\n%s\n%s\n",stmt.m_sql,stmt.m_cda.message);
      return 0;
    }
    conn.commit();
    return stmt.m_cda.rpc;
}

} // namespace server_client
