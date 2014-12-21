#ifndef __EXCH_REDIS_SUPPORT_HPP__
#define __EXCH_REDIS_SUPPORT_HPP__

#include "exch/exch.hpp"
#include "exch/interfaces.hpp"
#include "exch/order_book.hpp"
#include "redisclient/redisclient.h"
#include <functional>
#include <sstream>

namespace exch {
// custom <FcbBeginNamespace redis_support>

std::string redis_key(std::string const& market_id, Order const& order) {
  std::string result;
  result.reserve(32);  // 11 for market id + 1 for colon + 11 order id up
  // to power of 2
  result.append(market_id);
  result.push_back(':');
  result.append(boost::lexical_cast<std::string>(order.order_id()));
  return result;
}

// end <FcbBeginNamespace redis_support>

constexpr char const* M_req_key{"EX_REQ:M"};
constexpr char const* S_req_key{"EX_REQ:S"};
constexpr char const* C_req_key{"EX_REQ:C"};
constexpr char const* R_req_key{"EX_REQ:R"};
constexpr char const* H_req_key{"EX_REQ:H"};
constexpr char const* Cmd_key{"CMD"};
constexpr char const* Fills_key{"FILLS"};

using Req_func_t = boost::function<void(const std::string& request)>;

/**
 Subscribes to client requests on redis pub/sub channels
*/
class Redis_listener : public Request_listener {
 public:
  Redis_listener(RedisClient& redis_client) : redis_client_{redis_client} {}

  // custom <ClsPublic Redis_listener>

  virtual void subscribe(Create_market_handler_t create_market_handler,
                         Submit_handler_t submit_handler,
                         Cancel_handler_t cancel_handler,
                         Replace_handler_t replace_handler,
                         Halt_handler_t halt_handler) {

    create_market_handler_ = create_market_handler;
    submit_handler_ = submit_handler;
    cancel_handler_ = cancel_handler;
    replace_handler_ = replace_handler;
    halt_handler_ = halt_handler;

    m_handle_ = redis_client_.subscribe(
        M_req_key,
        std::bind(&Redis_listener::create_market, this, std::placeholders::_1));

    s_handle_ = redis_client_.subscribe(
        S_req_key,
        std::bind(&Redis_listener::submit, this, std::placeholders::_1));

    c_handle_ = redis_client_.subscribe(
        C_req_key,
        std::bind(&Redis_listener::cancel, this, std::placeholders::_1));

    r_handle_ = redis_client_.subscribe(
        R_req_key,
        std::bind(&Redis_listener::replace, this, std::placeholders::_1));

    h_handle_ = redis_client_.subscribe(H_req_key,
                                        std::bind(&Redis_listener::halt, this));
  }

  virtual void unsubscribe() {
    if (m_handle_.id) {
      redis_client_.unsubscribe(m_handle_);
    }
    if (s_handle_.id) {
      redis_client_.unsubscribe(s_handle_);
    }
    if (c_handle_.id) {
      redis_client_.unsubscribe(c_handle_);
    }
    if (r_handle_.id) {
      redis_client_.unsubscribe(r_handle_);
    }
    if (h_handle_.id) {
      redis_client_.unsubscribe(h_handle_);
    }
  }

  void create_market(std::string const& command) {
    Create_market_req req;
    std::istringstream in{command};
    req.serialize_from_json(in);
    create_market_handler_(req);
  }

  void submit(std::string const& command) {
    Submit_req req;
    std::istringstream in{command};
    req.serialize_from_json(in);
    submit_handler_(req);
  }

  void cancel(std::string const& command) {
    Cancel_req req;
    std::istringstream in{command};
    req.serialize_from_json(in);
    cancel_handler_(req);
  }

  void replace(std::string const& command) {
    Replace_req req;
    std::istringstream in{command};
    req.serialize_from_json(in);
    replace_handler_(req);
  }

  void halt() {
    std::cout << "halt req " << std::endl;
    halt_handler_();
  }

  virtual ~Redis_listener() { unsubscribe(); }

  // end <ClsPublic Redis_listener>

 private:
  RedisClient& redis_client_;
  RedisClient::Handle m_handle_{0};
  RedisClient::Handle s_handle_{0};
  RedisClient::Handle c_handle_{0};
  RedisClient::Handle r_handle_{0};
  RedisClient::Handle h_handle_{0};
  Create_market_handler_t create_market_handler_{};
  Submit_handler_t submit_handler_{};
  Cancel_handler_t cancel_handler_{};
  Replace_handler_t replace_handler_{};
  Halt_handler_t halt_handler_{};
};

/**
 Loads all saved commands from persistent storage at startup, effectively
 turning
 previously saved commands into a stream of commands for a subscriber (i.e. an
 exchange) to use at startup to process all messages to get to state just prior
 to last message before last shutdown.

*/
class Redis_bootstrap_listener : public Request_listener {
 public:
  Redis_bootstrap_listener(RedisClient& redis_client)
      : redis_client_{redis_client} {}

  // custom <ClsPublic Redis_bootstrap_listener>

  virtual void subscribe(Create_market_handler_t create_market_handler,
                         Submit_handler_t submit_handler,
                         Cancel_handler_t cancel_handler,
                         Replace_handler_t replace_handler,
                         Halt_handler_t halt_handler) override {

    // The following needs revisiting, need a way to make the calls
    // synchronous here.
    //
    // std::cout << "Reading " << Cmd_key << " 0 -1 " << std::endl;
    // redis_client_.command("LRANGE", Cmd_key, "0", "-1",
    //                       [&](const RedisValue &redisValue) {
    //   std::cout << "Got a return!!! " << std::endl;
    //   assert(redisValue.isArray());
    //   auto array = redisValue.toArray();
    //   std::cout << "Got an array " << std::endl;
    //   for (auto const& cmd : array) {
    //     assert(cmd.isString());
    //     auto cmdStr = cmd.toString();
    //     std::cout << "Got first cmd: " << cmdStr << std::endl;
    //     assert(cmdStr.size() > 0);
    //     switch (cmdStr[0]) {
    //     case 'M':
    //       create_market_handler(
    //           Create_market_req::serialize_from_dsv(cmdStr.substr(2)));
    //       break;
    //     case 'S':
    //       submit_handler(Submit_req::serialize_from_dsv(cmdStr.substr(2)));
    //       break;
    //     case 'C':
    //       cancel_handler(Cancel_req::serialize_from_dsv(cmdStr.substr(2)));
    //       break;
    //     case 'R':
    //       replace_handler(Replace_req::serialize_from_dsv(cmdStr.substr(2)));
    //       break;
    //     default: {
    //       std::ostringstream msg;
    //       msg << "Can not bootstrap: Invalid command found " << cmdStr;
    //       throw std::logic_error(msg.str());
    //     }
    //     }
    //   }
    // });
    // std::cout << "OOOPS" << std::endl;
  }

  virtual void unsubscribe() override {}

  // end <ClsPublic Redis_bootstrap_listener>

 private:
  RedisClient& redis_client_;
};

class Redis_persister : public Request_persister {
 public:
  Redis_persister(RedisClient& redis_client) : redis_client_{redis_client} {}

  // custom <ClsPublic Redis_persister>

  virtual void persist(Create_market_req const& req) override {
    _persist(req, 'M');
  }

  virtual void persist(Submit_req const& req) override { _persist(req, 'S'); }

  virtual void persist(Cancel_req const& req) override { _persist(req, 'C'); }

  virtual void persist(Replace_req const& req) override { _persist(req, 'R'); }

  virtual void persist(Fill const& fill) override {
    fmt::MemoryWriter w;
    w << fill.serialize_to_dsv();
    redis_client_.command("LPUSH", Fills_key, w.str());
  }

  // end <ClsPublic Redis_persister>

 private:
  // custom <ClsPrivate Redis_persister>

  template <typename T>
  void _persist(T const& item, char cmd) {
    fmt::MemoryWriter w;
    w << cmd << ':' << item.serialize_to_dsv();
    redis_client_.command("LPUSH", Cmd_key, w.str());
  }

  // end <ClsPrivate Redis_persister>

  RedisClient& redis_client_;
};

/**
 Implements the Market_publisher interface using redis as pub/sub
 middleware
*/
class Redis_publisher : public Market_publisher {
 public:
  Redis_publisher(RedisClient& redis_client) : redis_client_{redis_client} {}

  // custom <ClsPublic Redis_publisher>

  virtual void publish(Create_market_resp const& resp) override {
    _publish(resp);
  }

  virtual void publish(Submit_resp const& resp) override { _publish(resp); }

  virtual void publish(Cancel_resp const& resp) override { _publish(resp); }

  virtual void publish(Replace_resp const& resp) override { _publish(resp); }

  virtual void publish(Market_created_evt const& evt) override {
    _publish(evt);
  }

  virtual void publish(Top_of_book_evt const& evt) override { _publish(evt); }

  virtual void publish(Book_update_evt const& evt) override { _publish(evt); }

  virtual void publish(Trade_evt const& evt) override { _publish(evt); }

  // end <ClsPublic Redis_publisher>

 private:
  // custom <ClsPrivate Redis_publisher>

  template <typename T>
  void _publish(T const& item) {
    std::ostringstream out;
    item.serialize_to_json(out);
    redis_client_.publish(RESP_KEY, out.str());
  }

  // end <ClsPrivate Redis_publisher>

  RedisClient& redis_client_;
  static constexpr char const* RESP_KEY{"EX_RESP"};
  static constexpr char const* EVENT_KEY{"EX_EVENT"};
};

}  // namespace exch
#endif  // __EXCH_REDIS_SUPPORT_HPP__
