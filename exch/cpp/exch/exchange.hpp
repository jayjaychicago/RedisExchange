#ifndef __EXCH_EXCHANGE_HPP__
#define __EXCH_EXCHANGE_HPP__

#include "exch/exch.hpp"
#include "exch/market.hpp"
#include <string>

namespace exch {
  using Req_id_t = int64_t;

  enum Create_market_result {
    Create_market_succeeded_e,
    Create_market_failed_e
  };

  class Create_market_req
  {
  public:
    Create_market_req(
      Req_id_t req_id,
      std::string const & name,
      Timestamp_t start_time,
      Timestamp_t end_time,
      int decimal_shift,
      int shifted_modulus,
      Price_t max_price) :
      req_id_ { req_id },
      name_ { name },
      start_time_ { start_time },
      end_time_ { end_time },
      decimal_shift_ { decimal_shift },
      shifted_modulus_ { shifted_modulus },
      max_price_ { max_price } {
    }

    //! getter for req_id_ (access is Ro)
    Req_id_t req_id() const { return req_id_; }

    //! getter for name_ (access is Ro)
    std::string const& name() const { return name_; }

    //! getter for start_time_ (access is Ro)
    Timestamp_t start_time() const { return start_time_; }

    //! getter for end_time_ (access is Ro)
    Timestamp_t end_time() const { return end_time_; }

    //! getter for decimal_shift_ (access is Ro)
    int decimal_shift() const { return decimal_shift_; }

    //! getter for shifted_modulus_ (access is Ro)
    int shifted_modulus() const { return shifted_modulus_; }

    //! getter for max_price_ (access is Ro)
    Price_t max_price() const { return max_price_; }
  private:
    Req_id_t const req_id_;
    std::string const name_;
    Timestamp_t const start_time_;
    Timestamp_t const end_time_;
    int const decimal_shift_;
    int const shifted_modulus_;
    Price_t const max_price_;

  };


  class Create_market_resp
  {
  public:
    Create_market_resp(
      Req_id_t req_id,
      Create_market_result create_market_result,
      Market_id_t market_id) :
      req_id_ { req_id },
      create_market_result_ { create_market_result },
      market_id_ { market_id } {
    }

    //! getter for req_id_ (access is Ro)
    Req_id_t req_id() const { return req_id_; }

    //! getter for create_market_result_ (access is Ro)
    Create_market_result create_market_result() const { return create_market_result_; }

    //! getter for market_id_ (access is Ro)
    Market_id_t market_id() const { return market_id_; }
  private:
    Req_id_t const req_id_;
    Create_market_result const create_market_result_;
    Market_id_t const market_id_;

  };


  class Submit_req
  {
  public:
    Submit_req(
      Req_id_t req_id,
      Market_id_t market_id,
      User_id_t user_id,
      Price_t price,
      Quantity_t quantity) :
      req_id_ { req_id },
      market_id_ { market_id },
      user_id_ { user_id },
      price_ { price },
      quantity_ { quantity } {
    }

    //! getter for req_id_ (access is Ro)
    Req_id_t req_id() const { return req_id_; }

    //! getter for market_id_ (access is Ro)
    Market_id_t market_id() const { return market_id_; }

    //! getter for user_id_ (access is Ro)
    User_id_t user_id() const { return user_id_; }

    //! getter for price_ (access is Ro)
    Price_t price() const { return price_; }

    //! getter for quantity_ (access is Ro)
    Quantity_t quantity() const { return quantity_; }
  private:
    Req_id_t const req_id_;
    Market_id_t const market_id_;
    User_id_t const user_id_;
    Price_t const price_;
    Quantity_t const quantity_;

  };


  class Submit_resp
  {
  public:
    Submit_resp(
      Req_id_t req_id,
      Market_id_t market_id,
      User_id_t user_id,
      Order_id_t order_id,
      Submit_result submit_result) :
      req_id_ { req_id },
      market_id_ { market_id },
      user_id_ { user_id },
      order_id_ { order_id },
      submit_result_ { submit_result } {
    }

    //! getter for req_id_ (access is Ro)
    Req_id_t req_id() const { return req_id_; }

    //! getter for market_id_ (access is Ro)
    Market_id_t market_id() const { return market_id_; }

    //! getter for user_id_ (access is Ro)
    User_id_t user_id() const { return user_id_; }

    //! getter for order_id_ (access is Ro)
    Order_id_t order_id() const { return order_id_; }

    //! getter for submit_result_ (access is Ro)
    Submit_result submit_result() const { return submit_result_; }
  private:
    Req_id_t const req_id_;
    Market_id_t const market_id_;
    User_id_t const user_id_;
    Order_id_t const order_id_;
    Submit_result const submit_result_;

  };


  class Cancel_req
  {
  public:
    Cancel_req(
      Req_id_t req_id,
      Market_id_t market_id,
      User_id_t user_id,
      Order_id_t order_id) :
      req_id_ { req_id },
      market_id_ { market_id },
      user_id_ { user_id },
      order_id_ { order_id } {
    }

    //! getter for req_id_ (access is Ro)
    Req_id_t req_id() const { return req_id_; }

    //! getter for market_id_ (access is Ro)
    Market_id_t market_id() const { return market_id_; }

    //! getter for user_id_ (access is Ro)
    User_id_t user_id() const { return user_id_; }

    //! getter for order_id_ (access is Ro)
    Order_id_t order_id() const { return order_id_; }
  private:
    Req_id_t const req_id_;
    Market_id_t const market_id_;
    User_id_t const user_id_;
    Order_id_t const order_id_;

  };


  class Cancel_resp
  {
  public:
    Cancel_resp(
      Req_id_t req_id,
      Market_id_t market_id,
      User_id_t user_id,
      Order_id_t order_id,
      Cancel_result cancel_result) :
      req_id_ { req_id },
      market_id_ { market_id },
      user_id_ { user_id },
      order_id_ { order_id },
      cancel_result_ { cancel_result } {
    }

    //! getter for req_id_ (access is Ro)
    Req_id_t req_id() const { return req_id_; }

    //! getter for market_id_ (access is Ro)
    Market_id_t market_id() const { return market_id_; }

    //! getter for user_id_ (access is Ro)
    User_id_t user_id() const { return user_id_; }

    //! getter for order_id_ (access is Ro)
    Order_id_t order_id() const { return order_id_; }

    //! getter for cancel_result_ (access is Ro)
    Cancel_result cancel_result() const { return cancel_result_; }
  private:
    Req_id_t const req_id_;
    Market_id_t const market_id_;
    User_id_t const user_id_;
    Order_id_t const order_id_;
    Cancel_result const cancel_result_;

  };


  class Replace_req
  {
  public:
    Replace_req(
      Req_id_t req_id,
      Market_id_t market_id,
      User_id_t user_id,
      Order_id_t existing_order_id,
      Price_t price,
      Quantity_t quantity) :
      req_id_ { req_id },
      market_id_ { market_id },
      user_id_ { user_id },
      existing_order_id_ { existing_order_id },
      price_ { price },
      quantity_ { quantity } {
    }

    //! getter for req_id_ (access is Ro)
    Req_id_t req_id() const { return req_id_; }

    //! getter for market_id_ (access is Ro)
    Market_id_t market_id() const { return market_id_; }

    //! getter for user_id_ (access is Ro)
    User_id_t user_id() const { return user_id_; }

    //! getter for existing_order_id_ (access is Ro)
    Order_id_t existing_order_id() const { return existing_order_id_; }

    //! getter for price_ (access is Ro)
    Price_t price() const { return price_; }

    //! getter for quantity_ (access is Ro)
    Quantity_t quantity() const { return quantity_; }
  private:
    Req_id_t const req_id_;
    Market_id_t const market_id_;
    User_id_t const user_id_;
    Order_id_t const existing_order_id_;
    Price_t const price_;
    Quantity_t const quantity_;

  };


  class Replace_resp
  {
  public:
    Replace_resp(
      Req_id_t req_id,
      Market_id_t market_id,
      User_id_t user_id,
      Order_id_t canceled_order_id,
      Order_id_t order_id,
      Replace_result replace_result) :
      req_id_ { req_id },
      market_id_ { market_id },
      user_id_ { user_id },
      canceled_order_id_ { canceled_order_id },
      order_id_ { order_id },
      replace_result_ { replace_result } {
    }

    //! getter for req_id_ (access is Ro)
    Req_id_t req_id() const { return req_id_; }

    //! getter for market_id_ (access is Ro)
    Market_id_t market_id() const { return market_id_; }

    //! getter for user_id_ (access is Ro)
    User_id_t user_id() const { return user_id_; }

    //! getter for canceled_order_id_ (access is Ro)
    Order_id_t canceled_order_id() const { return canceled_order_id_; }

    //! getter for order_id_ (access is Ro)
    Order_id_t order_id() const { return order_id_; }

    //! getter for replace_result_ (access is Ro)
    Replace_result replace_result() const { return replace_result_; }
  private:
    Req_id_t const req_id_;
    Market_id_t const market_id_;
    User_id_t const user_id_;
    Order_id_t const canceled_order_id_;
    Order_id_t const order_id_;
    Replace_result const replace_result_;

  };


  class Market_created_evt
  {
  public:
    Market_created_evt(
      Market_id_t market_id) :
      market_id_ { market_id } {
    }

    //! getter for market_id_ (access is Ro)
    Market_id_t market_id() const { return market_id_; }
  private:
    Market_id_t const market_id_;

  };


  class Top_of_book_evt
  {
  public:
    Top_of_book_evt(
      Market_id_t market_id) :
      market_id_ { market_id } {
    }

    //! getter for market_id_ (access is Ro)
    Market_id_t market_id() const { return market_id_; }
  private:
    Market_id_t const market_id_;

  };


  class Book_update_evt
  {
  public:
    Book_update_evt(
      Market_id_t market_id,
      Side side,
      Price_t price,
      Quantity_t quantity,
      Price_t top_price) :
      market_id_ { market_id },
      side_ { side },
      price_ { price },
      quantity_ { quantity },
      top_price_ { top_price } {
    }

    //! getter for market_id_ (access is Ro)
    Market_id_t market_id() const { return market_id_; }

    //! getter for side_ (access is Ro)
    Side side() const { return side_; }

    //! getter for price_ (access is Ro)
    Price_t price() const { return price_; }

    //! getter for quantity_ (access is Ro)
    Quantity_t quantity() const { return quantity_; }

    //! getter for top_price_ (access is Ro)
    Price_t top_price() const { return top_price_; }
  private:
    Market_id_t const market_id_;
    Side const side_;
    Price_t const price_;
    Quantity_t const quantity_;
    Price_t const top_price_;

  };


  class Trade_evt
  {
  public:
    Trade_evt(
      Market_id_t market_id,
      Side side,
      Quantity_t quantity,
      Price_t price,
      Quantity_t net_volume) :
      market_id_ { market_id },
      side_ { side },
      quantity_ { quantity },
      price_ { price },
      net_volume_ { net_volume } {
    }

    //! getter for market_id_ (access is Ro)
    Market_id_t market_id() const { return market_id_; }

    //! getter for side_ (access is Ro)
    Side side() const { return side_; }

    //! getter for quantity_ (access is Ro)
    Quantity_t quantity() const { return quantity_; }

    //! getter for price_ (access is Ro)
    Price_t price() const { return price_; }

    //! getter for net_volume_ (access is Ro)
    Quantity_t net_volume() const { return net_volume_; }
  private:
    Market_id_t const market_id_;
    Side const side_;
    Quantity_t const quantity_;
    Price_t const price_;
    Quantity_t const net_volume_;

  };


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


  /**
     Manages multiple markets. Requests are forwarded to the appropriate
     exchange for handling. Responses from the Market_exchange instances
     are used to create and publish events using the Market_publisher. An
     example publisher might serialize the responses to Json and post via
     redis - but that is an implementation detail from the perspective of
     this class.
  */
  class Exchange
  {
  public:
    using Market_exchange_ptr = std::unique_ptr< Market_exchange >;
    using Market_exchange_naked_ptr = Market_exchange *;
    using Market_exchange_map_t = std::map< Market_id_t, Market_exchange_ptr >;

    Exchange(
      Market_publisher & market_publisher) :
      market_publisher_ { market_publisher } {
      // custom <Exchange(from_args)>
      // end <Exchange(from_args)>
    }

    // custom <ClsPublic Exchange>

    void create_market(Create_market_req const& req) {
      return market_publisher_.publish(
        Create_market_resp(req.req_id(), Create_market_failed_e, 0));
    };

    void submit(Submit_req const& req) {
      Market_exchange_naked_ptr market { get_market(req.market_id()) };
      Submit_result result;
      Order_id_t submitted_id {};

      if(market == nullptr) {
        result = Submit_invalid_market_e;
      } else {
        submitted_id = market->next_order_id();
        Order order { submitted_id,
            fcs::timestamp::current_time(),
            req.quantity(),
            req.price() };
        result = market->submit(order);
      }

      market_publisher_.publish(
        Submit_resp(req.req_id(),
                    req.market_id(),
                    req.user_id(),
                    submitted_id,
                    result));
    }

    void cancel(Cancel_req const& req) {
      Market_exchange_naked_ptr market { get_market(req.market_id()) };
      Cancel_result result;

      result = (market == nullptr)? Cancel_invalid_market_e :
        market->cancel(req.order_id());

      market_publisher_.publish(
        Cancel_resp(req.req_id(),
                    req.market_id(),
                    req.user_id(),
                    req.order_id(),
                    result));
    }

    void replace(Replace_req const& req) {
      Market_exchange_naked_ptr market { get_market(req.market_id()) };
      Replace_result result;
      Order_id_t revised_order_id {};

      if(market == nullptr) {
        result = Replace_invalid_market_e;
      } else {
        revised_order_id = market->next_order_id();
        Order revised_order { revised_order_id,
            fcs::timestamp::current_time(),
            req.quantity(),
            req.price() };

        result = market->replace_order(req.existing_order_id(), revised_order);
      }

      market_publisher_.publish(
        Replace_resp(req.req_id(),
                     req.market_id(),
                     req.user_id(),
                     req.existing_order_id(),
                     revised_order_id,
                     result));
    }

    // end <ClsPublic Exchange>

  private:
    // custom <ClsPrivate Exchange>

    Market_exchange_naked_ptr
    get_market(Market_id_t market) {
      Market_exchange_map_t::iterator found { market_exchanges_.find(market) };
      if(found != market_exchanges_.end()) {
        return found->second.get();
      } else {
        return nullptr;
      }
    }

    // end <ClsPrivate Exchange>

    Market_publisher & market_publisher_;
    Market_exchange_map_t market_exchanges_ {};

  };


} // namespace exch
#endif // __EXCH_EXCHANGE_HPP__
