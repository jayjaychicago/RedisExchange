#ifndef __EXCH_INTERFACES_HPP__
#define __EXCH_INTERFACES_HPP__

#include "exch/events.hpp"
#include "exch/exch.hpp"
#include "exch/fill.hpp"
#include "exch/requests.hpp"
#include <boost/function.hpp>

namespace exch {
using Create_market_handler_t =
    boost::function<void(const Create_market_req& req)>;
using Submit_handler_t = boost::function<void(const Submit_req& req)>;
using Cancel_handler_t = boost::function<void(const Cancel_req& req)>;
using Replace_handler_t = boost::function<void(const Replace_req& req)>;
using Log_handler_t = boost::function<void(const Log_req& req)>;
using Halt_handler_t = boost::function<void()>;

/**
 Listens for requests (submit, cancel, replace,...) from clients
*/
class Request_listener {
 public:
  virtual ~Request_listener() {}
  // custom <ClsPublic Request_listener>

  virtual void subscribe(Create_market_handler_t create_market_handler,
                         Submit_handler_t submit_handler,
                         Cancel_handler_t cancel_handler,
                         Replace_handler_t replace_handler,
                         Log_handler_t log_handler,
                         Halt_handler_t halt_handler) = 0;

  virtual void unsubscribe() = 0;

  // end <ClsPublic Request_listener>
};

/**
 Persists requests (submit, cancel, replace,...) and results
*/
class Request_persister {
 public:
  virtual ~Request_persister() {}
  // custom <ClsPublic Request_persister>

  virtual void persist(Create_market_req const& req) = 0;
  virtual void persist(Submit_req const& req) = 0;
  virtual void persist(Cancel_req const& req) = 0;
  virtual void persist(Replace_req const& req) = 0;
  virtual void persist(Fill const& fill) = 0;

  // end <ClsPublic Request_persister>
};

/**
 Publishes market events (fill, top_of_book,...)
*/
class Market_publisher {
 public:
  virtual ~Market_publisher() {}
  // custom <ClsPublic Market_publisher>

  virtual void publish(Create_market_resp const& resp) = 0;
  virtual void publish(Submit_resp const& resp) = 0;
  virtual void publish(Cancel_resp const& resp) = 0;
  virtual void publish(Replace_resp const& resp) = 0;

  virtual void publish(Market_created_evt const& evt) = 0;
  virtual void publish(Top_of_book_evt const& evt) = 0;
  virtual void publish(Book_update_evt const& evt) = 0;
  virtual void publish(Trade_evt const& evt) = 0;

  // end <ClsPublic Market_publisher>
};

}  // namespace exch
#endif  // __EXCH_INTERFACES_HPP__
