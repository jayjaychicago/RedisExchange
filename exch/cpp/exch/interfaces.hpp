#ifndef __EXCH_INTERFACES_HPP__
#define __EXCH_INTERFACES_HPP__

#include "exch/events.hpp"
#include "exch/exch.hpp"
#include "exch/requests.hpp"
#include <boost/function.hpp>

namespace exch {
  using Req_func_t = boost::function< void(const std::string & request) >;

  /**
   Listens for requests (submit, cancel, replace,...) from clients
  */
  class Request_listener
  {
  public:
    virtual ~Request_listener() {}
    // custom <ClsPublic Request_listener>

    virtual void subscribe(Req_func_t handler);

    // end <ClsPublic Request_listener>

  };


  /**
   Persists requests (submit, cancel, replace,...) and results
  */
  class Request_persister
  {
  public:
    virtual ~Request_persister() {}
    // custom <ClsPublic Request_persister>
    // end <ClsPublic Request_persister>

  };


  /**
   Publishes market events (fill, top_of_book,...)
  */
  class Market_publisher
  {
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


} // namespace exch
#endif // __EXCH_INTERFACES_HPP__
