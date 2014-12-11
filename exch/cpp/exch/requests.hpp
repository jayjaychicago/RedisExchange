#ifndef __EXCH_REQUESTS_HPP__
#define __EXCH_REQUESTS_HPP__

#include "exch/exch.hpp"
#include <string>

namespace exch {
  class Create_market_req
  {
  public:
    Create_market_req(
      Req_id_t req_id,
      Market_id_t market_id,
      std::string const & name,
      Timestamp_t start_time,
      Timestamp_t end_time,
      int decimal_shift,
      int tick_size) :
      req_id_ { req_id },
      market_id_ { market_id },
      name_ { name },
      start_time_ { start_time },
      end_time_ { end_time },
      decimal_shift_ { decimal_shift },
      tick_size_ { tick_size } {
    }

    //! getter for req_id_ (access is Ro)
    Req_id_t req_id() const { return req_id_; }

    //! getter for market_id_ (access is Ro)
    Market_id_t market_id() const { return market_id_; }

    //! getter for name_ (access is Ro)
    std::string const& name() const { return name_; }

    //! getter for start_time_ (access is Ro)
    Timestamp_t start_time() const { return start_time_; }

    //! getter for end_time_ (access is Ro)
    Timestamp_t end_time() const { return end_time_; }

    //! getter for decimal_shift_ (access is Ro)
    int decimal_shift() const { return decimal_shift_; }

    //! getter for tick_size_ (access is Ro)
    int tick_size() const { return tick_size_; }
  private:
    Req_id_t const req_id_;
    Market_id_t const market_id_;
    std::string const name_;
    Timestamp_t const start_time_;
    Timestamp_t const end_time_;
    int const decimal_shift_;
    int const tick_size_;

  };


  class Create_market_resp
  {
  public:
    Create_market_resp(
      Req_id_t req_id,
      Market_id_t market_id,
      Create_market_result result) :
      req_id_ { req_id },
      market_id_ { market_id },
      result_ { result } {
    }

    //! getter for req_id_ (access is Ro)
    Req_id_t req_id() const { return req_id_; }

    //! getter for market_id_ (access is Ro)
    Market_id_t market_id() const { return market_id_; }

    //! getter for result_ (access is Ro)
    Create_market_result result() const { return result_; }
  private:
    Req_id_t const req_id_;
    Market_id_t const market_id_;
    Create_market_result const result_;

  };


  class Submit_req
  {
  public:
    Submit_req(
      Req_id_t req_id,
      Market_id_t market_id,
      User_id_t user_id,
      Side side,
      Price_t price,
      Quantity_t quantity) :
      req_id_ { req_id },
      market_id_ { market_id },
      user_id_ { user_id },
      side_ { side },
      price_ { price },
      quantity_ { quantity } {
    }

    //! getter for req_id_ (access is Ro)
    Req_id_t req_id() const { return req_id_; }

    //! getter for market_id_ (access is Ro)
    Market_id_t market_id() const { return market_id_; }

    //! getter for user_id_ (access is Ro)
    User_id_t user_id() const { return user_id_; }

    //! getter for side_ (access is Ro)
    Side side() const { return side_; }

    //! getter for price_ (access is Ro)
    Price_t price() const { return price_; }

    //! getter for quantity_ (access is Ro)
    Quantity_t quantity() const { return quantity_; }
  private:
    Req_id_t const req_id_;
    Market_id_t const market_id_;
    User_id_t const user_id_;
    Side const side_;
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
      Submit_result result) :
      req_id_ { req_id },
      market_id_ { market_id },
      user_id_ { user_id },
      order_id_ { order_id },
      result_ { result } {
    }

    //! getter for req_id_ (access is Ro)
    Req_id_t req_id() const { return req_id_; }

    //! getter for market_id_ (access is Ro)
    Market_id_t market_id() const { return market_id_; }

    //! getter for user_id_ (access is Ro)
    User_id_t user_id() const { return user_id_; }

    //! getter for order_id_ (access is Ro)
    Order_id_t order_id() const { return order_id_; }

    //! getter for result_ (access is Ro)
    Submit_result result() const { return result_; }
  private:
    Req_id_t const req_id_;
    Market_id_t const market_id_;
    User_id_t const user_id_;
    Order_id_t const order_id_;
    Submit_result const result_;

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
      Cancel_result result) :
      req_id_ { req_id },
      market_id_ { market_id },
      user_id_ { user_id },
      order_id_ { order_id },
      result_ { result } {
    }

    //! getter for req_id_ (access is Ro)
    Req_id_t req_id() const { return req_id_; }

    //! getter for market_id_ (access is Ro)
    Market_id_t market_id() const { return market_id_; }

    //! getter for user_id_ (access is Ro)
    User_id_t user_id() const { return user_id_; }

    //! getter for order_id_ (access is Ro)
    Order_id_t order_id() const { return order_id_; }

    //! getter for result_ (access is Ro)
    Cancel_result result() const { return result_; }
  private:
    Req_id_t const req_id_;
    Market_id_t const market_id_;
    User_id_t const user_id_;
    Order_id_t const order_id_;
    Cancel_result const result_;

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
      Replace_result result) :
      req_id_ { req_id },
      market_id_ { market_id },
      user_id_ { user_id },
      canceled_order_id_ { canceled_order_id },
      order_id_ { order_id },
      result_ { result } {
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

    //! getter for result_ (access is Ro)
    Replace_result result() const { return result_; }
  private:
    Req_id_t const req_id_;
    Market_id_t const market_id_;
    User_id_t const user_id_;
    Order_id_t const canceled_order_id_;
    Order_id_t const order_id_;
    Replace_result const result_;

  };


} // namespace exch
#endif // __EXCH_REQUESTS_HPP__
