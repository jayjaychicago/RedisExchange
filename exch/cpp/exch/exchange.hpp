#ifndef __EXCH_EXCHANGE_HPP__
#define __EXCH_EXCHANGE_HPP__

#include "exch/events.hpp"
#include "exch/exch.hpp"
#include "exch/interfaces.hpp"
#include "exch/market_exch.hpp"
#include "exch/requests.hpp"

namespace exch {
/**
 Manages multiple markets. Requests come from the listener, to which
 this exchange subscribes. Those requests are persisted and turned into
 Market_exchange type objects (e.g. order) which are forwarded to the
 appropriate Market_exchange for handling. Responses from the
 Market_exchange instances are then persisted and used to create and
 publish events using the Market_publisher. An example publisher might
 serialize the responses to Json and post via redis - but that is an
 implementation detail from the perspective of this class.
*/
class Exchange {
 public:
  using Market_exchange_naked_ptr = Market_exchange *;
  using Market_exchange_ptr = std::unique_ptr<Market_exchange>;
  using Market_exchange_map_t = std::map<Market_id_t, Market_exchange_ptr>;

  Exchange(Request_listener &bootstrap_listener,
           Request_listener &request_listener,
           Request_persister &request_persister,
           Market_publisher &market_publisher)
      : bootstrap_listener_{bootstrap_listener},
        request_listener_{request_listener},
        request_persister_{request_persister},
        market_publisher_{market_publisher} {
    // custom <Exchange(from_args)>

    // bootstrap_listener_.subscribe(
    //     std::bind(&Exchange::create_market, this, std::placeholders::_1),
    //     std::bind(&Exchange::submit, this, std::placeholders::_1),
    //     std::bind(&Exchange::cancel, this, std::placeholders::_1),
    //     std::bind(&Exchange::replace, this, std::placeholders::_1),
    //     std::bind(&Exchange::halt_handler, this));

    is_live_ = true;

    request_listener_.subscribe(
        std::bind(&Exchange::create_market, this, std::placeholders::_1),
        std::bind(&Exchange::submit, this, std::placeholders::_1),
        std::bind(&Exchange::cancel, this, std::placeholders::_1),
        std::bind(&Exchange::replace, this, std::placeholders::_1),
        std::bind(&Exchange::halt_handler, this));

    // end <Exchange(from_args)>
  }

  // custom <ClsPublic Exchange>

  // end <ClsPublic Exchange>

 private:
  // custom <ClsPrivate Exchange>

  void halt_handler() { request_listener_.unsubscribe(); }

  Market_exchange_naked_ptr get_market(Market_id_t market) {
    Market_exchange_map_t::iterator found{market_exchanges_.find(market)};
    if (found != market_exchanges_.end()) {
      return found->second.get();
    } else {
      return nullptr;
    }
  }

  void create_market(Create_market_req const &req) {

    Create_market_result result;
    Market_id_t market_id{++next_market_id_};

    using Insert_result_t = std::pair<Market_exchange_map_t::iterator, bool>;

    Insert_result_t insert_result{market_exchanges_.insert(
        Market_exchange_map_t::value_type(market_id, Market_exchange_ptr()))};

    if (insert_result.second) {
      Market_config config{req.name(),      req.start_time(),
                           req.end_time(),  req.decimal_shift(),
                           req.tick_size(), };

      insert_result.first->second =
          Market_exchange_ptr(new Market_exchange(config, market_id));

      result = Create_market_succeeded_e;
    } else {
      result = Create_market_failed_e;
    }

    if (is_live_) {
      request_persister_.persist(req);
      market_publisher_.publish(
          Create_market_resp(req.req_id(), req.user_id(), market_id, result));
    }
  };

  void submit(Submit_req const &req) {
    Market_exchange_naked_ptr market{get_market(req.market_id())};
    Submit_result result;
    Order_id_t submitted_id{};

    if (market == nullptr) {
      result = Submit_invalid_market_e;
    } else {
      submitted_id = market->next_order_id();
      Order order{submitted_id, fcs::timestamp::current_time(), req.side(),
                  req.price(), req.quantity()};
      result = market->submit(order);
    }

    if (is_live_) {
      request_persister_.persist(req);
      market_publisher_.publish(Submit_resp(
          req.req_id(), req.user_id(), req.market_id(), submitted_id, result));
    }
  }

  void cancel(Cancel_req const &req) {
    Market_exchange_naked_ptr market{get_market(req.market_id())};
    Cancel_result result;

    result = (market == nullptr) ? Cancel_invalid_market_e
                                 : market->cancel(req.order_id());

    if (is_live_) {
      request_persister_.persist(req);
      market_publisher_.publish(Cancel_resp(req.req_id(), req.user_id(),
                                            req.market_id(), req.order_id(),
                                            result));
    }
  }

  void replace(Replace_req const &req) {
    Market_exchange_naked_ptr market{get_market(req.market_id())};
    Replace_result result;
    Order_id_t revised_order_id{};

    if (market == nullptr) {
      result = Replace_invalid_market_e;
    } else {
      revised_order_id = market->next_order_id();
      Order revised_order{
          revised_order_id, fcs::timestamp::current_time(),
          Bid_side_e,  // Not used: Side in book pulled from resting order
          req.quantity(), req.price()};

      result = market->replace_order(req.order_id(), revised_order);
    }

    if (is_live_) {
      request_persister_.persist(req);
      market_publisher_.publish(Replace_resp(req.req_id(), req.user_id(),
                                             req.market_id(), req.order_id(),
                                             revised_order_id, result));
    }
  }

  void shutdown() {}

  // end <ClsPrivate Exchange>

  Request_listener &bootstrap_listener_;
  Request_listener &request_listener_;
  Request_persister &request_persister_;
  Market_publisher &market_publisher_;
  Market_exchange_map_t market_exchanges_{};
  /**
   Indicates bootstrapping is complete and new commands should be saved and
   responses published
  */
  int is_live_{false};
  int next_market_id_{0};
};

}  // namespace exch
#endif  // __EXCH_EXCHANGE_HPP__
