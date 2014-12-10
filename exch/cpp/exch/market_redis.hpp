#ifndef __EXCH_MARKET_REDIS_HPP__
#define __EXCH_MARKET_REDIS_HPP__

#include "exch/exch.hpp"
#include "exch/market.hpp"
#include "redisclient/redisclient.h"
#include <boost/lexical_cast.hpp>
#include <sstream>
#include <string>

namespace exch {
  // custom <ClsPreDecl Market_exchange_redis>
  // end <ClsPreDecl Market_exchange_redis>

  /**
     Provides service for one market exchange, persisting books in redis
  */
  class Market_exchange_redis :
    public Market_exchange
  {
  public:
    Market_exchange_redis(
      Market_config const& market_config,
      Market_id_t market_id,
      RedisClient & redis_client) :
      Market_exchange(market_config, market_id),
      redis_client_ { redis_client },
      market_id_str_ { boost::lexical_cast< std::string >(market_id) },
      orders_key_ { market_id_str_ + std::string(":ORDERS") } {
    }

    // custom <ClsPublic Market_exchange_redis>

    virtual
    Submit_result submit(Order const& order) {
      redis_client_.command("SET", orders_key_, order.to_tuple());
      return Submit_result();
    }

    virtual
    Cancel_result cancel(Order_id_t const& order_id) {
      return Cancel_result();
    }

    virtual
    Replace_result replace_order(Order_id_t original, Order const& order) {
      return Replace_result();
    }

    // end <ClsPublic Market_exchange_redis>

  private:
    // custom <ClsPrivate Market_exchange_redis>

    std::string redis_key(Order const& order) {
      std::string result { market_id_str_ };
      result += boost::lexical_cast< std::string >(order.order_id());
      return result;
    }

    // end <ClsPrivate Market_exchange_redis>

    RedisClient & redis_client_;
    std::string const market_id_str_ {};
    std::string const orders_key_ {};

  };


  class Market_exchange_redis_factory
  {
  public:
    Market_exchange_redis_factory(
      RedisClient & redis_client) :
      redis_client_ { redis_client } {
    }

    // custom <ClsPublic Market_exchange_redis_factory>

    virtual
    Market_exchange_ptr create_market(Market_config const& market_config,
                                      Market_id_t market_id) {
      Market_exchange_ptr result {
        new Market_exchange_redis(market_config, market_id, redis_client_)
      };
      return result;
    }

    // end <ClsPublic Market_exchange_redis_factory>

  private:
    RedisClient & redis_client_;

  };


} // namespace exch
#endif // __EXCH_MARKET_REDIS_HPP__
