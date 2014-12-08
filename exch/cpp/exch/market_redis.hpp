#ifndef __EXCH_MARKET_REDIS_HPP__
#define __EXCH_MARKET_REDIS_HPP__

#include "exch/exch.hpp"

namespace exch {
  /**
     Provides service for one market exchange, persisting books in redis
  */
  class Market_exchange_redis :
    public Market_exchange
  {
  };


} // namespace exch
#endif // __EXCH_MARKET_REDIS_HPP__