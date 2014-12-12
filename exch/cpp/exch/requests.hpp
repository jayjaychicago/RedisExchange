#ifndef __EXCH_REQUESTS_HPP__
#define __EXCH_REQUESTS_HPP__

#include "cereal/archives/json.hpp"
#include "cereal/cereal.hpp"
#include "exch/exch.hpp"
#include "fcs/timestamp/cereal.hpp"
#include <iosfwd>
#include <string>

namespace exch {
  class Create_market_req
  {
  public:
    Create_market_req(
      Req_id_t req_id,
      User_id_t user_id,
      std::string const & name,
      Timestamp_t start_time,
      Timestamp_t end_time,
      int decimal_shift,
      int tick_size) :
      req_id_ { req_id },
      user_id_ { user_id },
      name_ { name },
      start_time_ { start_time },
      end_time_ { end_time },
      decimal_shift_ { decimal_shift },
      tick_size_ { tick_size } {
    }

    //! getter for req_id_ (access is Ro)
    Req_id_t req_id() const { return req_id_; }

    //! getter for user_id_ (access is Ro)
    User_id_t user_id() const { return user_id_; }

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
    friend inline
    std::ostream& operator<<(std::ostream& out,
                             Create_market_req const& item) {
      out << '\n' << "req_id:" << item.req_id_;
      out << '\n' << "user_id:" << item.user_id_;
      out << '\n' << "name:" << item.name_;
      out << '\n' << "start_time:" << item.start_time_;
      out << '\n' << "end_time:" << item.end_time_;
      out << '\n' << "decimal_shift:" << item.decimal_shift_;
      out << '\n' << "tick_size:" << item.tick_size_;
      return out;
    }

    template <class Archive>
    void serialize(Archive &ar__) {
      ar__(cereal::make_nvp("req_id", req_id_));
      ar__(cereal::make_nvp("user_id", user_id_));
      ar__(cereal::make_nvp("name", name_));
      ar__(cereal::make_nvp("start_time", start_time_));
      ar__(cereal::make_nvp("end_time", end_time_));
      ar__(cereal::make_nvp("decimal_shift", decimal_shift_));
      ar__(cereal::make_nvp("tick_size", tick_size_));
    }

    void serialize_to_json(std::ostream & out__) {
      cereal::JSONOutputArchive ar__(out__);
      serialize(ar__);
    }

    void serialize_from_json(std::istream & in__) {
      cereal::JSONInputArchive ar__ { in__ };
      serialize(ar__);
    }


  private:
    Req_id_t const req_id_;
    User_id_t const user_id_;
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
    friend inline
    std::ostream& operator<<(std::ostream& out,
                             Create_market_resp const& item) {
      out << '\n' << "req_id:" << item.req_id_;
      out << '\n' << "market_id:" << item.market_id_;
      out << '\n' << "result:" << item.result_;
      return out;
    }

    template <class Archive>
    void serialize(Archive &ar__) {
      ar__(cereal::make_nvp("req_id", req_id_));
      ar__(cereal::make_nvp("market_id", market_id_));
      ar__(cereal::make_nvp("result", result_));
    }

    void serialize_to_json(std::ostream & out__) {
      cereal::JSONOutputArchive ar__(out__);
      serialize(ar__);
    }

    void serialize_from_json(std::istream & in__) {
      cereal::JSONInputArchive ar__ { in__ };
      serialize(ar__);
    }


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
    friend inline
    std::ostream& operator<<(std::ostream& out,
                             Submit_req const& item) {
      out << '\n' << "req_id:" << item.req_id_;
      out << '\n' << "market_id:" << item.market_id_;
      out << '\n' << "user_id:" << item.user_id_;
      out << '\n' << "side:" << item.side_;
      out << '\n' << "price:" << item.price_;
      out << '\n' << "quantity:" << item.quantity_;
      return out;
    }

    template <class Archive>
    void serialize(Archive &ar__) {
      ar__(cereal::make_nvp("req_id", req_id_));
      ar__(cereal::make_nvp("market_id", market_id_));
      ar__(cereal::make_nvp("user_id", user_id_));
      ar__(cereal::make_nvp("side", side_));
      ar__(cereal::make_nvp("price", price_));
      ar__(cereal::make_nvp("quantity", quantity_));
    }

    void serialize_to_json(std::ostream & out__) {
      cereal::JSONOutputArchive ar__(out__);
      serialize(ar__);
    }

    void serialize_from_json(std::istream & in__) {
      cereal::JSONInputArchive ar__ { in__ };
      serialize(ar__);
    }


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
    friend inline
    std::ostream& operator<<(std::ostream& out,
                             Submit_resp const& item) {
      out << '\n' << "req_id:" << item.req_id_;
      out << '\n' << "market_id:" << item.market_id_;
      out << '\n' << "user_id:" << item.user_id_;
      out << '\n' << "order_id:" << item.order_id_;
      out << '\n' << "result:" << item.result_;
      return out;
    }

    template <class Archive>
    void serialize(Archive &ar__) {
      ar__(cereal::make_nvp("req_id", req_id_));
      ar__(cereal::make_nvp("market_id", market_id_));
      ar__(cereal::make_nvp("user_id", user_id_));
      ar__(cereal::make_nvp("order_id", order_id_));
      ar__(cereal::make_nvp("result", result_));
    }

    void serialize_to_json(std::ostream & out__) {
      cereal::JSONOutputArchive ar__(out__);
      serialize(ar__);
    }

    void serialize_from_json(std::istream & in__) {
      cereal::JSONInputArchive ar__ { in__ };
      serialize(ar__);
    }


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
    friend inline
    std::ostream& operator<<(std::ostream& out,
                             Cancel_req const& item) {
      out << '\n' << "req_id:" << item.req_id_;
      out << '\n' << "market_id:" << item.market_id_;
      out << '\n' << "user_id:" << item.user_id_;
      out << '\n' << "order_id:" << item.order_id_;
      return out;
    }

    template <class Archive>
    void serialize(Archive &ar__) {
      ar__(cereal::make_nvp("req_id", req_id_));
      ar__(cereal::make_nvp("market_id", market_id_));
      ar__(cereal::make_nvp("user_id", user_id_));
      ar__(cereal::make_nvp("order_id", order_id_));
    }

    void serialize_to_json(std::ostream & out__) {
      cereal::JSONOutputArchive ar__(out__);
      serialize(ar__);
    }

    void serialize_from_json(std::istream & in__) {
      cereal::JSONInputArchive ar__ { in__ };
      serialize(ar__);
    }


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
    friend inline
    std::ostream& operator<<(std::ostream& out,
                             Cancel_resp const& item) {
      out << '\n' << "req_id:" << item.req_id_;
      out << '\n' << "market_id:" << item.market_id_;
      out << '\n' << "user_id:" << item.user_id_;
      out << '\n' << "order_id:" << item.order_id_;
      out << '\n' << "result:" << item.result_;
      return out;
    }

    template <class Archive>
    void serialize(Archive &ar__) {
      ar__(cereal::make_nvp("req_id", req_id_));
      ar__(cereal::make_nvp("market_id", market_id_));
      ar__(cereal::make_nvp("user_id", user_id_));
      ar__(cereal::make_nvp("order_id", order_id_));
      ar__(cereal::make_nvp("result", result_));
    }

    void serialize_to_json(std::ostream & out__) {
      cereal::JSONOutputArchive ar__(out__);
      serialize(ar__);
    }

    void serialize_from_json(std::istream & in__) {
      cereal::JSONInputArchive ar__ { in__ };
      serialize(ar__);
    }


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
    friend inline
    std::ostream& operator<<(std::ostream& out,
                             Replace_req const& item) {
      out << '\n' << "req_id:" << item.req_id_;
      out << '\n' << "market_id:" << item.market_id_;
      out << '\n' << "user_id:" << item.user_id_;
      out << '\n' << "existing_order_id:" << item.existing_order_id_;
      out << '\n' << "price:" << item.price_;
      out << '\n' << "quantity:" << item.quantity_;
      return out;
    }

    template <class Archive>
    void serialize(Archive &ar__) {
      ar__(cereal::make_nvp("req_id", req_id_));
      ar__(cereal::make_nvp("market_id", market_id_));
      ar__(cereal::make_nvp("user_id", user_id_));
      ar__(cereal::make_nvp("existing_order_id", existing_order_id_));
      ar__(cereal::make_nvp("price", price_));
      ar__(cereal::make_nvp("quantity", quantity_));
    }

    void serialize_to_json(std::ostream & out__) {
      cereal::JSONOutputArchive ar__(out__);
      serialize(ar__);
    }

    void serialize_from_json(std::istream & in__) {
      cereal::JSONInputArchive ar__ { in__ };
      serialize(ar__);
    }


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
    friend inline
    std::ostream& operator<<(std::ostream& out,
                             Replace_resp const& item) {
      out << '\n' << "req_id:" << item.req_id_;
      out << '\n' << "market_id:" << item.market_id_;
      out << '\n' << "user_id:" << item.user_id_;
      out << '\n' << "canceled_order_id:" << item.canceled_order_id_;
      out << '\n' << "order_id:" << item.order_id_;
      out << '\n' << "result:" << item.result_;
      return out;
    }

    template <class Archive>
    void serialize(Archive &ar__) {
      ar__(cereal::make_nvp("req_id", req_id_));
      ar__(cereal::make_nvp("market_id", market_id_));
      ar__(cereal::make_nvp("user_id", user_id_));
      ar__(cereal::make_nvp("canceled_order_id", canceled_order_id_));
      ar__(cereal::make_nvp("order_id", order_id_));
      ar__(cereal::make_nvp("result", result_));
    }

    void serialize_to_json(std::ostream & out__) {
      cereal::JSONOutputArchive ar__(out__);
      serialize(ar__);
    }

    void serialize_from_json(std::istream & in__) {
      cereal::JSONInputArchive ar__ { in__ };
      serialize(ar__);
    }


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
