#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#ifndef SRC_SERVICE_GLOBALSERVICE_H_
#define SRC_SERVICE_GLOBALSERVICE_H_

#include "service/tradeService.h"
#include "service/accountService.h"
#include "service/customerService.h"

using namespace server_client;

namespace globalService {

    static CustomerService customervice;
    static AccountService accountServive;
    static TradeService tradeService;

} // namespace server_client

#endif // SRC_SERVICE_GLOBALSERVICE_H_