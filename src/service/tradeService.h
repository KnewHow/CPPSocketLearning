#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#ifndef SRC_SERVICE_TRADESERVICE_H_
#define SRC_SERVICE_TRADESERVICE_H_

#include "dao/trade.h"

namespace server_client {
class TradeService {
public:
    int insert(const Trade& trade) const;
    int deleteAll() const;
};

} // namespace server_client

#endif // SRC_SERVICE_TRADESERVICE_H_