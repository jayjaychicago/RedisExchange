#ifndef __EXCH_REDIS_SUPPORT_HPP__
#define __EXCH_REDIS_SUPPORT_HPP__

#include "exch/exch.hpp"
#include "exch/interfaces.hpp"
#include "exch/order_book.hpp"
#include "redisclient/redisclient.h"
#include <sstream>

namespace exch {
  // custom <FcbBeginNamespace redis_support>

  std::string redis_key(std::string const& market_id, Order const& order) {
    std::string result;
    result.reserve(32); // 11 for market id + 1 for colon + 11 order id up
    // to power of 2
    result.append(market_id);
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
    Redis_listener(
      RedisClient & redis_client) :
      redis_client_ { redis_client } {
    }

    // custom <ClsPublic Redis_listener>

    virtual void subscribe(Req_func_t handler) {
      handle_ = redis_client_.subscribe(REQ_KEY, handler);
    }

    virtual ~Redis_listener() {
      if(handle_.id) {
        redis_client_.unsubscribe(handle_);
      }
    }

    // end <ClsPublic Redis_listener>

  private:
    RedisClient & redis_client_;
    RedisClient::Handle handle_ { 0 };
    static constexpr char const* REQ_KEY { "EX_REQ" };

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


  /**
   Implements the Market_publisher interface using redis as pub/sub
   middleware
  */
  class Redis_publisher :
    public Market_publisher
  {
  public:
    Redis_publisher(
      RedisClient & redis_client) :
      redis_client_ { redis_client } {
    }

    // custom <ClsPublic Redis_publisher>

    virtual void publish(Create_market_resp const& resp) {
      _publish(resp);
    }

    virtual void publish(Submit_resp const& resp) {
      _publish(resp);
    }

    virtual void publish(Cancel_resp const& resp) {
      _publish(resp);
    }

    virtual void publish(Replace_resp const& resp) {
      _publish(resp);
    }


    virtual void publish(Market_created_evt const& evt) {
      _publish(evt);
    }

    virtual void publish(Top_of_book_evt const& evt) {
      _publish(evt);
    }

    virtual void publish(Book_update_evt const& evt) {
      _publish(evt);
    }

    virtual void publish(Trade_evt const& evt) {
      _publish(evt);
    }


    // end <ClsPublic Redis_publisher>

  private:
    // custom <ClsPrivate Redis_publisher>

    template< typename T > void _publish(T const& item) {
      std::ostringstream out;
      item.serialize_to_json(out);
      redis_client_.publish(RESP_KEY, out.str());
    }

    // end <ClsPrivate Redis_publisher>

    RedisClient & redis_client_;
    static constexpr char const* RESP_KEY { "EX_RESP" };
    static constexpr char const* EVENT_KEY { "EX_EVENT" };

  };


} // namespace exch
#endif // __EXCH_REDIS_SUPPORT_HPP__
