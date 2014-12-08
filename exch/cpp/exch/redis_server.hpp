#ifndef __EXCH_REDIS_SERVER_HPP__
#define __EXCH_REDIS_SERVER_HPP__

#include "exch/exch.hpp"

namespace exch {
  /**
     Subscribes to client requests on redis pub/sub channels
  */
  class Redis_listener
  {
  };


  /**
     Implements the Market_publisher interface using redis as pub/sub
     middleware
  */
  class Redis_publisher :
    public Market_publisher
  {
  };


  class Redis_server :
    public Exchange_server
  {
  private:
    Exchange exchange_ {};
    RedisListener listener_ {};

  };


} // namespace exch
#endif // __EXCH_REDIS_SERVER_HPP__
