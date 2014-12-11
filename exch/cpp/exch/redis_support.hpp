#ifndef __EXCH_REDIS_SUPPORT_HPP__
#define __EXCH_REDIS_SUPPORT_HPP__

#include "exch/exch.hpp"

namespace exch {
  // custom <FcbBeginNamespace redis_support>

  std::string redis_key(Order const& order) {
    std::string result;
    result.reserve(32); // 11 for market id + 1 for colon + 11 order id up
    // to power of 2
    result.append(market_id_str_);
    result.push_back(':');
    result.append(boost::lexical_cast< std::string >(order.order_id()));
    return result;
  }

  // end <FcbBeginNamespace redis_support>

  /**
   Subscribes to client requests on redis pub/sub channels
  */
  class Redis_listener :
    public Request_listener
  {
  public:
    // custom <ClsPublic Redis_listener>
    // end <ClsPublic Redis_listener>

  private:
    RedisClient & redis_client_;

  };


  /**
   Implements the Market_publisher interface using redis as pub/sub
   middleware
  */
  class Redis_publisher :
    public Market_publisher
  {
  public:
    // custom <ClsPublic Redis_publisher>
    // end <ClsPublic Redis_publisher>

  private:
    RedisClient & redis_client_;

  };


  class Redis_persister :
    public Request_persister
  {
  public:
    // custom <ClsPublic Redis_persister>
    // end <ClsPublic Redis_persister>

  private:
    RedisClient & redis_client_;

  };


} // namespace exch
#endif // __EXCH_REDIS_SUPPORT_HPP__
