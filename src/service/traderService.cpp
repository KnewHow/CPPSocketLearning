#include "tradeService.h"
#include "glog/logging.h"
#include "_postgresql.h"
#include "common/dbconfig.h"
#include "common/stringprint.h"

namespace server_client {

int TradeService::insert(const Trade& trade) const {
    connection conn;

    if (conn.connecttodb(DBConfig::config.data(), DBConfig::charset.data())!=0)
    {
        LOG(ERROR) << StringPrintf("connect database failed.\n%s\n",conn.m_cda.message);
        return 0 ;
    }

    sqlstatement stmt(&conn); // 操作SQL语句的对象。
    
    stmt.prepare("insert into trade(source_account_id, dest_account_id, money, status, modified_time, created_time) values (:1, :2, :3, :4, to_date(:5,'yyyy-mm-dd hh24:mi:ss'), to_date(:6,'yyyy-mm-dd hh24:mi:ss'))");
    stmt.bindin(1, &trade.source_account_id);
    stmt.bindin(2, &trade.dest_account_id);
    stmt.bindin(3, &trade.money);
    stmt.bindin(4, &trade.status);
    stmt.bindin(5, trade.modified_time, sizeof(trade.modified_time));
    stmt.bindin(6, trade.created_time, sizeof(trade.created_time));

    if (stmt.execute() != 0)
    {
      LOG(ERROR) << StringPrintf("stmt.execute() failed.\n%s\n%s\n",stmt.m_sql,stmt.m_cda.message);
      return 0;
    }
    conn.commit();
    return stmt.m_cda.rpc;
}

int TradeService::deleteAll() const {
    connection conn;

    if (conn.connecttodb(DBConfig::config.data(), DBConfig::charset.data())!=0)
    {
        LOG(ERROR) << StringPrintf("connect database failed.\n%s\n",conn.m_cda.message);
        return 0;
    }

    sqlstatement stmt(&conn);
    stmt.prepare("delete from trade");
    if (stmt.execute() != 0)
    {
      LOG(ERROR) << StringPrintf("stmt.execute() failed.\n%s\n%s\n",stmt.m_sql,stmt.m_cda.message);
      return 0;
    }
    conn.commit();
    return stmt.m_cda.rpc;
  }

} // namespace server_client