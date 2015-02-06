#ifndef __EXCH_REQUESTS_HPP__
#define __EXCH_REQUESTS_HPP__

#include "cereal/archives/json.hpp"
#include "cereal/cereal.hpp"
#include "cereal/types/vector.hpp"
#include "cppformat/format.h"
#include "exch/exch.hpp"
#include "fcs/timestamp/cereal.hpp"
#include "fcs/timestamp/conversion.hpp"
#include "fcs/utils/streamers/vector.hpp"
#include <iosfwd>
#include <string>

namespace exch {
class Create_market_req {
 public:
  Create_market_req(Req_id_t req_id, User_id_t user_id, std::string const& name,
                    Timestamp_t start_time, Timestamp_t end_time,
                    int decimal_shift, int tick_size)
      : req_id_{req_id},
        user_id_{user_id},
        name_{name},
        start_time_{start_time},
        end_time_{end_time},
        decimal_shift_{decimal_shift},
        tick_size_{tick_size} {}

  Create_market_req() = default;

  bool operator==(Create_market_req const& rhs) const {
    return this == &rhs ||
           (req_id_ == rhs.req_id_ && user_id_ == rhs.user_id_ &&
            name_ == rhs.name_ && start_time_ == rhs.start_time_ &&
            end_time_ == rhs.end_time_ &&
            decimal_shift_ == rhs.decimal_shift_ &&
            tick_size_ == rhs.tick_size_);
  }

  bool operator!=(Create_market_req const& rhs) const {
    return !(*this == rhs);
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
  friend inline std::ostream& operator<<(std::ostream& out,
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
  void serialize(Archive& ar__) {
    ar__(cereal::make_nvp("req_id", req_id_));
    ar__(cereal::make_nvp("user_id", user_id_));
    ar__(cereal::make_nvp("name", name_));
    ar__(cereal::make_nvp("start_time", start_time_));
    ar__(cereal::make_nvp("end_time", end_time_));
    ar__(cereal::make_nvp("decimal_shift", decimal_shift_));
    ar__(cereal::make_nvp("tick_size", tick_size_));
  }

  void serialize_to_json(std::ostream& out__) const {
    cereal::JSONOutputArchive ar__(out__);
    const_cast<Create_market_req*>(this)->serialize(ar__);
  }

  void serialize_from_json(std::istream& in__) {
    cereal::JSONInputArchive ar__{in__};
    serialize(ar__);
  }

  std::string serialize_to_dsv() const {
    fmt::MemoryWriter w__;
    w__ << req_id_ << ':' << user_id_ << ':' << name_ << ':'
        << fcs::timestamp::ticks(start_time_) << ':'
        << fcs::timestamp::ticks(end_time_) << ':' << decimal_shift_ << ':'
        << tick_size_;

    return w__.str();
  }

  void serialize_from_dsv(std::string const& tuple__) {
    using namespace boost;
    char_separator<char> const sep__{":"};
    tokenizer<char_separator<char> > tokens__(tuple__, sep__);
    tokenizer<boost::char_separator<char> >::iterator it__{tokens__.begin()};

    if (it__ != tokens__.end()) {
      req_id_ = lexical_cast<Req_id_t>(*it__);
      ++it__;
    } else {
      throw std::logic_error(
          "Tokenize Create_market_req failed: expected req_id_");
    }

    if (it__ != tokens__.end()) {
      user_id_ = lexical_cast<User_id_t>(*it__);
      ++it__;
    } else {
      throw std::logic_error(
          "Tokenize Create_market_req failed: expected user_id_");
    }

    if (it__ != tokens__.end()) {
      name_ = lexical_cast<std::string>(*it__);
      ++it__;
    } else {
      throw std::logic_error(
          "Tokenize Create_market_req failed: expected name_");
    }

    if (it__ != tokens__.end()) {
      if (!fcs::timestamp::convert_to_timestamp_from_ticks(*it__,
                                                           start_time_)) {
        std::string msg{"Encountered invalid timestamp ticks:"};
        msg += *it__;
        throw std::logic_error(msg);
      }

      ++it__;
    } else {
      throw std::logic_error(
          "Tokenize Create_market_req failed: expected start_time_");
    }

    if (it__ != tokens__.end()) {
      if (!fcs::timestamp::convert_to_timestamp_from_ticks(*it__, end_time_)) {
        std::string msg{"Encountered invalid timestamp ticks:"};
        msg += *it__;
        throw std::logic_error(msg);
      }

      ++it__;
    } else {
      throw std::logic_error(
          "Tokenize Create_market_req failed: expected end_time_");
    }

    if (it__ != tokens__.end()) {
      decimal_shift_ = lexical_cast<int>(*it__);
      ++it__;
    } else {
      throw std::logic_error(
          "Tokenize Create_market_req failed: expected decimal_shift_");
    }

    if (it__ != tokens__.end()) {
      tick_size_ = lexical_cast<int>(*it__);
      ++it__;
    } else {
      throw std::logic_error(
          "Tokenize Create_market_req failed: expected tick_size_");
    }
  }

 private:
  Req_id_t req_id_{};
  User_id_t user_id_{};
  std::string name_{};
  Timestamp_t start_time_{};
  Timestamp_t end_time_{};
  int decimal_shift_{};
  int tick_size_{};
};

class Create_market_resp {
 public:
  Create_market_resp(Req_id_t req_id, User_id_t user_id, Market_id_t market_id,
                     Create_market_result result)
      : req_id_{req_id},
        user_id_{user_id},
        market_id_{market_id},
        result_{result} {}

  Create_market_resp() {}

  bool operator==(Create_market_resp const& rhs) const {
    return this == &rhs ||
           (req_id_ == rhs.req_id_ && user_id_ == rhs.user_id_ &&
            market_id_ == rhs.market_id_ && result_ == rhs.result_);
  }

  bool operator!=(Create_market_resp const& rhs) const {
    return !(*this == rhs);
  }
  //! getter for req_id_ (access is Ro)
  Req_id_t req_id() const { return req_id_; }

  //! getter for user_id_ (access is Ro)
  User_id_t user_id() const { return user_id_; }

  //! getter for market_id_ (access is Ro)
  Market_id_t market_id() const { return market_id_; }

  //! getter for result_ (access is Ro)
  Create_market_result result() const { return result_; }
  friend inline std::ostream& operator<<(std::ostream& out,
                                         Create_market_resp const& item) {
    out << '\n' << "req_id:" << item.req_id_;
    out << '\n' << "user_id:" << item.user_id_;
    out << '\n' << "market_id:" << item.market_id_;
    out << '\n' << "result:" << item.result_;
    return out;
  }

  template <class Archive>
  void serialize(Archive& ar__) {
    ar__(cereal::make_nvp("req_id", req_id_));
    ar__(cereal::make_nvp("user_id", user_id_));
    ar__(cereal::make_nvp("market_id", market_id_));
    ar__(cereal::make_nvp("result", result_));
  }

  void serialize_to_json(std::ostream& out__) const {
    cereal::JSONOutputArchive ar__(out__);
    const_cast<Create_market_resp*>(this)->serialize(ar__);
  }

  void serialize_from_json(std::istream& in__) {
    cereal::JSONInputArchive ar__{in__};
    serialize(ar__);
  }

 private:
  Req_id_t req_id_{};
  User_id_t user_id_{};
  Market_id_t market_id_{};
  Create_market_result result_{};
};

class Submit_req {
 public:
  Submit_req(Timestamp_t timestamp, Req_id_t req_id, User_id_t user_id,
             Market_id_t market_id, Side side, Price_t price,
             Quantity_t quantity)
      : timestamp_{timestamp},
        req_id_{req_id},
        user_id_{user_id},
        market_id_{market_id},
        side_{side},
        price_{price},
        quantity_{quantity} {}

  Submit_req() = default;

  bool operator==(Submit_req const& rhs) const {
    return this == &rhs ||
           (timestamp_ == rhs.timestamp_ && req_id_ == rhs.req_id_ &&
            user_id_ == rhs.user_id_ && market_id_ == rhs.market_id_ &&
            side_ == rhs.side_ && price_ == rhs.price_ &&
            quantity_ == rhs.quantity_);
  }

  bool operator!=(Submit_req const& rhs) const { return !(*this == rhs); }
  // custom <ClsPublic Submit_req>

  void timestamp(Timestamp_t timestamp) const { timestamp_ = timestamp; }

  // end <ClsPublic Submit_req>

  //! getter for timestamp_ (access is Ro)
  Timestamp_t timestamp() const { return timestamp_; }

  //! getter for req_id_ (access is Ro)
  Req_id_t req_id() const { return req_id_; }

  //! getter for user_id_ (access is Ro)
  User_id_t user_id() const { return user_id_; }

  //! getter for market_id_ (access is Ro)
  Market_id_t market_id() const { return market_id_; }

  //! getter for side_ (access is Ro)
  Side side() const { return side_; }

  //! getter for price_ (access is Ro)
  Price_t price() const { return price_; }

  //! getter for quantity_ (access is Ro)
  Quantity_t quantity() const { return quantity_; }
  friend inline std::ostream& operator<<(std::ostream& out,
                                         Submit_req const& item) {
    out << '\n' << "timestamp:" << item.timestamp_;
    out << '\n' << "req_id:" << item.req_id_;
    out << '\n' << "user_id:" << item.user_id_;
    out << '\n' << "market_id:" << item.market_id_;
    out << '\n' << "side:" << item.side_;
    out << '\n' << "price:" << item.price_;
    out << '\n' << "quantity:" << item.quantity_;
    return out;
  }

  template <class Archive>
  void serialize(Archive& ar__) {
    ar__(cereal::make_nvp("req_id", req_id_));
    ar__(cereal::make_nvp("user_id", user_id_));
    ar__(cereal::make_nvp("market_id", market_id_));
    ar__(cereal::make_nvp("side", side_));
    ar__(cereal::make_nvp("price", price_));
    ar__(cereal::make_nvp("quantity", quantity_));
  }

  void serialize_to_json(std::ostream& out__) const {
    cereal::JSONOutputArchive ar__(out__);
    const_cast<Submit_req*>(this)->serialize(ar__);
  }

  void serialize_from_json(std::istream& in__) {
    cereal::JSONInputArchive ar__{in__};
    serialize(ar__);
  }

  std::string serialize_to_dsv() const {
    fmt::MemoryWriter w__;
    w__ << fcs::timestamp::ticks(timestamp_) << ':' << req_id_ << ':'
        << user_id_ << ':' << market_id_ << ':' << side_ << ':' << price_ << ':'
        << quantity_;

    return w__.str();
  }

  void serialize_from_dsv(std::string const& tuple__) {
    using namespace boost;
    char_separator<char> const sep__{":"};
    tokenizer<char_separator<char> > tokens__(tuple__, sep__);
    tokenizer<boost::char_separator<char> >::iterator it__{tokens__.begin()};

    if (it__ != tokens__.end()) {
      if (!fcs::timestamp::convert_to_timestamp_from_ticks(*it__, timestamp_)) {
        std::string msg{"Encountered invalid timestamp ticks:"};
        msg += *it__;
        throw std::logic_error(msg);
      }

      ++it__;
    } else {
      throw std::logic_error("Tokenize Submit_req failed: expected timestamp_");
    }

    if (it__ != tokens__.end()) {
      req_id_ = lexical_cast<Req_id_t>(*it__);
      ++it__;
    } else {
      throw std::logic_error("Tokenize Submit_req failed: expected req_id_");
    }

    if (it__ != tokens__.end()) {
      user_id_ = lexical_cast<User_id_t>(*it__);
      ++it__;
    } else {
      throw std::logic_error("Tokenize Submit_req failed: expected user_id_");
    }

    if (it__ != tokens__.end()) {
      market_id_ = lexical_cast<Market_id_t>(*it__);
      ++it__;
    } else {
      throw std::logic_error("Tokenize Submit_req failed: expected market_id_");
    }

    if (it__ != tokens__.end()) {
      side_ = Side(lexical_cast<int>(*it__));
      ++it__;
    } else {
      throw std::logic_error("Tokenize Submit_req failed: expected side_");
    }

    if (it__ != tokens__.end()) {
      price_ = lexical_cast<Price_t>(*it__);
      ++it__;
    } else {
      throw std::logic_error("Tokenize Submit_req failed: expected price_");
    }

    if (it__ != tokens__.end()) {
      quantity_ = lexical_cast<Quantity_t>(*it__);
      ++it__;
    } else {
      throw std::logic_error("Tokenize Submit_req failed: expected quantity_");
    }
  }

 private:
  mutable Timestamp_t timestamp_{Timestamp_t::time_rep_type(0LL)};
  Req_id_t req_id_{};
  User_id_t user_id_{};
  Market_id_t market_id_{};
  Side side_{};
  Price_t price_{};
  Quantity_t quantity_{};
};

class Submit_resp {
 public:
  Submit_resp(Req_id_t req_id, User_id_t user_id, Market_id_t market_id,
              Submit_result result)
      : req_id_{req_id},
        user_id_{user_id},
        market_id_{market_id},
        result_{result} {}
  Submit_resp(Req_id_t req_id, User_id_t user_id, Market_id_t market_id,
              Order_id_t order_id, Submit_result result)
      : req_id_{req_id},
        user_id_{user_id},
        market_id_{market_id},
        order_id_{order_id},
        result_{result} {}

  Submit_resp() = default;

  bool operator==(Submit_resp const& rhs) const {
    return this == &rhs ||
           (req_id_ == rhs.req_id_ && user_id_ == rhs.user_id_ &&
            market_id_ == rhs.market_id_ && order_id_ == rhs.order_id_ &&
            result_ == rhs.result_);
  }

  bool operator!=(Submit_resp const& rhs) const { return !(*this == rhs); }
  //! getter for req_id_ (access is Ro)
  Req_id_t req_id() const { return req_id_; }

  //! getter for user_id_ (access is Ro)
  User_id_t user_id() const { return user_id_; }

  //! getter for market_id_ (access is Ro)
  Market_id_t market_id() const { return market_id_; }

  //! getter for order_id_ (access is Ro)
  Order_id_t order_id() const { return order_id_; }

  //! getter for result_ (access is Ro)
  Submit_result result() const { return result_; }
  friend inline std::ostream& operator<<(std::ostream& out,
                                         Submit_resp const& item) {
    out << '\n' << "req_id:" << item.req_id_;
    out << '\n' << "user_id:" << item.user_id_;
    out << '\n' << "market_id:" << item.market_id_;
    out << '\n' << "order_id:" << item.order_id_;
    out << '\n' << "result:" << item.result_;
    return out;
  }

  template <class Archive>
  void serialize(Archive& ar__) {
    ar__(cereal::make_nvp("req_id", req_id_));
    ar__(cereal::make_nvp("user_id", user_id_));
    ar__(cereal::make_nvp("market_id", market_id_));
    ar__(cereal::make_nvp("order_id", order_id_));
    ar__(cereal::make_nvp("result", result_));
  }

  void serialize_to_json(std::ostream& out__) const {
    cereal::JSONOutputArchive ar__(out__);
    const_cast<Submit_resp*>(this)->serialize(ar__);
  }

  void serialize_from_json(std::istream& in__) {
    cereal::JSONInputArchive ar__{in__};
    serialize(ar__);
  }

 private:
  Req_id_t req_id_{};
  User_id_t user_id_{};
  Market_id_t market_id_{};
  Order_id_t order_id_{};
  Submit_result result_{};
};

class Cancel_req {
 public:
  Cancel_req(Req_id_t req_id, User_id_t user_id, Market_id_t market_id,
             Order_id_t order_id)
      : req_id_{req_id},
        user_id_{user_id},
        market_id_{market_id},
        order_id_{order_id} {}

  Cancel_req() = default;

  bool operator==(Cancel_req const& rhs) const {
    return this == &rhs ||
           (req_id_ == rhs.req_id_ && user_id_ == rhs.user_id_ &&
            market_id_ == rhs.market_id_ && order_id_ == rhs.order_id_);
  }

  bool operator!=(Cancel_req const& rhs) const { return !(*this == rhs); }
  //! getter for req_id_ (access is Ro)
  Req_id_t req_id() const { return req_id_; }

  //! getter for user_id_ (access is Ro)
  User_id_t user_id() const { return user_id_; }

  //! getter for market_id_ (access is Ro)
  Market_id_t market_id() const { return market_id_; }

  //! getter for order_id_ (access is Ro)
  Order_id_t order_id() const { return order_id_; }
  friend inline std::ostream& operator<<(std::ostream& out,
                                         Cancel_req const& item) {
    out << '\n' << "req_id:" << item.req_id_;
    out << '\n' << "user_id:" << item.user_id_;
    out << '\n' << "market_id:" << item.market_id_;
    out << '\n' << "order_id:" << item.order_id_;
    return out;
  }

  template <class Archive>
  void serialize(Archive& ar__) {
    ar__(cereal::make_nvp("req_id", req_id_));
    ar__(cereal::make_nvp("user_id", user_id_));
    ar__(cereal::make_nvp("market_id", market_id_));
    ar__(cereal::make_nvp("order_id", order_id_));
  }

  void serialize_to_json(std::ostream& out__) const {
    cereal::JSONOutputArchive ar__(out__);
    const_cast<Cancel_req*>(this)->serialize(ar__);
  }

  void serialize_from_json(std::istream& in__) {
    cereal::JSONInputArchive ar__{in__};
    serialize(ar__);
  }

  std::string serialize_to_dsv() const {
    fmt::MemoryWriter w__;
    w__ << req_id_ << ':' << user_id_ << ':' << market_id_ << ':' << order_id_;

    return w__.str();
  }

  void serialize_from_dsv(std::string const& tuple__) {
    using namespace boost;
    char_separator<char> const sep__{":"};
    tokenizer<char_separator<char> > tokens__(tuple__, sep__);
    tokenizer<boost::char_separator<char> >::iterator it__{tokens__.begin()};

    if (it__ != tokens__.end()) {
      req_id_ = lexical_cast<Req_id_t>(*it__);
      ++it__;
    } else {
      throw std::logic_error("Tokenize Cancel_req failed: expected req_id_");
    }

    if (it__ != tokens__.end()) {
      user_id_ = lexical_cast<User_id_t>(*it__);
      ++it__;
    } else {
      throw std::logic_error("Tokenize Cancel_req failed: expected user_id_");
    }

    if (it__ != tokens__.end()) {
      market_id_ = lexical_cast<Market_id_t>(*it__);
      ++it__;
    } else {
      throw std::logic_error("Tokenize Cancel_req failed: expected market_id_");
    }

    if (it__ != tokens__.end()) {
      order_id_ = lexical_cast<Order_id_t>(*it__);
      ++it__;
    } else {
      throw std::logic_error("Tokenize Cancel_req failed: expected order_id_");
    }
  }

 private:
  Req_id_t req_id_{};
  User_id_t user_id_{};
  Market_id_t market_id_{};
  Order_id_t order_id_{};
};

class Cancel_resp {
 public:
  Cancel_resp(Req_id_t req_id, User_id_t user_id, Market_id_t market_id,
              Order_id_t order_id, Cancel_result result)
      : req_id_{req_id},
        user_id_{user_id},
        market_id_{market_id},
        order_id_{order_id},
        result_{result} {}

  Cancel_resp() = default;

  bool operator==(Cancel_resp const& rhs) const {
    return this == &rhs ||
           (req_id_ == rhs.req_id_ && user_id_ == rhs.user_id_ &&
            market_id_ == rhs.market_id_ && order_id_ == rhs.order_id_ &&
            result_ == rhs.result_);
  }

  bool operator!=(Cancel_resp const& rhs) const { return !(*this == rhs); }
  //! getter for req_id_ (access is Ro)
  Req_id_t req_id() const { return req_id_; }

  //! getter for user_id_ (access is Ro)
  User_id_t user_id() const { return user_id_; }

  //! getter for market_id_ (access is Ro)
  Market_id_t market_id() const { return market_id_; }

  //! getter for order_id_ (access is Ro)
  Order_id_t order_id() const { return order_id_; }

  //! getter for result_ (access is Ro)
  Cancel_result result() const { return result_; }
  friend inline std::ostream& operator<<(std::ostream& out,
                                         Cancel_resp const& item) {
    out << '\n' << "req_id:" << item.req_id_;
    out << '\n' << "user_id:" << item.user_id_;
    out << '\n' << "market_id:" << item.market_id_;
    out << '\n' << "order_id:" << item.order_id_;
    out << '\n' << "result:" << item.result_;
    return out;
  }

  template <class Archive>
  void serialize(Archive& ar__) {
    ar__(cereal::make_nvp("req_id", req_id_));
    ar__(cereal::make_nvp("user_id", user_id_));
    ar__(cereal::make_nvp("market_id", market_id_));
    ar__(cereal::make_nvp("order_id", order_id_));
    ar__(cereal::make_nvp("result", result_));
  }

  void serialize_to_json(std::ostream& out__) const {
    cereal::JSONOutputArchive ar__(out__);
    const_cast<Cancel_resp*>(this)->serialize(ar__);
  }

  void serialize_from_json(std::istream& in__) {
    cereal::JSONInputArchive ar__{in__};
    serialize(ar__);
  }

 private:
  Req_id_t req_id_{};
  User_id_t user_id_{};
  Market_id_t market_id_{};
  Order_id_t order_id_{};
  Cancel_result result_{};
};

class Replace_req {
 public:
  Replace_req(Req_id_t req_id, User_id_t user_id, Market_id_t market_id,
              Order_id_t order_id, Price_t price, Quantity_t quantity)
      : req_id_{req_id},
        user_id_{user_id},
        market_id_{market_id},
        order_id_{order_id},
        price_{price},
        quantity_{quantity} {}

  Replace_req() = default;

  bool operator==(Replace_req const& rhs) const {
    return this == &rhs ||
           (req_id_ == rhs.req_id_ && user_id_ == rhs.user_id_ &&
            market_id_ == rhs.market_id_ && order_id_ == rhs.order_id_ &&
            price_ == rhs.price_ && quantity_ == rhs.quantity_);
  }

  bool operator!=(Replace_req const& rhs) const { return !(*this == rhs); }
  //! getter for req_id_ (access is Ro)
  Req_id_t req_id() const { return req_id_; }

  //! getter for user_id_ (access is Ro)
  User_id_t user_id() const { return user_id_; }

  //! getter for market_id_ (access is Ro)
  Market_id_t market_id() const { return market_id_; }

  //! getter for order_id_ (access is Ro)
  Order_id_t order_id() const { return order_id_; }

  //! getter for price_ (access is Ro)
  Price_t price() const { return price_; }

  //! getter for quantity_ (access is Ro)
  Quantity_t quantity() const { return quantity_; }
  friend inline std::ostream& operator<<(std::ostream& out,
                                         Replace_req const& item) {
    out << '\n' << "req_id:" << item.req_id_;
    out << '\n' << "user_id:" << item.user_id_;
    out << '\n' << "market_id:" << item.market_id_;
    out << '\n' << "order_id:" << item.order_id_;
    out << '\n' << "price:" << item.price_;
    out << '\n' << "quantity:" << item.quantity_;
    return out;
  }

  template <class Archive>
  void serialize(Archive& ar__) {
    ar__(cereal::make_nvp("req_id", req_id_));
    ar__(cereal::make_nvp("user_id", user_id_));
    ar__(cereal::make_nvp("market_id", market_id_));
    ar__(cereal::make_nvp("order_id", order_id_));
    ar__(cereal::make_nvp("price", price_));
    ar__(cereal::make_nvp("quantity", quantity_));
  }

  void serialize_to_json(std::ostream& out__) const {
    cereal::JSONOutputArchive ar__(out__);
    const_cast<Replace_req*>(this)->serialize(ar__);
  }

  void serialize_from_json(std::istream& in__) {
    cereal::JSONInputArchive ar__{in__};
    serialize(ar__);
  }

  std::string serialize_to_dsv() const {
    fmt::MemoryWriter w__;
    w__ << req_id_ << ':' << user_id_ << ':' << market_id_ << ':' << order_id_
        << ':' << price_ << ':' << quantity_;

    return w__.str();
  }

  void serialize_from_dsv(std::string const& tuple__) {
    using namespace boost;
    char_separator<char> const sep__{":"};
    tokenizer<char_separator<char> > tokens__(tuple__, sep__);
    tokenizer<boost::char_separator<char> >::iterator it__{tokens__.begin()};

    if (it__ != tokens__.end()) {
      req_id_ = lexical_cast<Req_id_t>(*it__);
      ++it__;
    } else {
      throw std::logic_error("Tokenize Replace_req failed: expected req_id_");
    }

    if (it__ != tokens__.end()) {
      user_id_ = lexical_cast<User_id_t>(*it__);
      ++it__;
    } else {
      throw std::logic_error("Tokenize Replace_req failed: expected user_id_");
    }

    if (it__ != tokens__.end()) {
      market_id_ = lexical_cast<Market_id_t>(*it__);
      ++it__;
    } else {
      throw std::logic_error(
          "Tokenize Replace_req failed: expected market_id_");
    }

    if (it__ != tokens__.end()) {
      order_id_ = lexical_cast<Order_id_t>(*it__);
      ++it__;
    } else {
      throw std::logic_error("Tokenize Replace_req failed: expected order_id_");
    }

    if (it__ != tokens__.end()) {
      price_ = lexical_cast<Price_t>(*it__);
      ++it__;
    } else {
      throw std::logic_error("Tokenize Replace_req failed: expected price_");
    }

    if (it__ != tokens__.end()) {
      quantity_ = lexical_cast<Quantity_t>(*it__);
      ++it__;
    } else {
      throw std::logic_error("Tokenize Replace_req failed: expected quantity_");
    }
  }

 private:
  Req_id_t req_id_{};
  User_id_t user_id_{};
  Market_id_t market_id_{};
  Order_id_t order_id_{};
  Price_t price_{};
  Quantity_t quantity_{};
};

class Replace_resp {
 public:
  Replace_resp(Req_id_t req_id, User_id_t user_id, Market_id_t market_id,
               Replace_result result)
      : req_id_{req_id},
        user_id_{user_id},
        market_id_{market_id},
        result_{result} {}
  Replace_resp(Req_id_t req_id, User_id_t user_id, Market_id_t market_id,
               Order_id_t canceled_order_id, Order_id_t order_id,
               Replace_result result)
      : req_id_{req_id},
        user_id_{user_id},
        market_id_{market_id},
        canceled_order_id_{canceled_order_id},
        order_id_{order_id},
        result_{result} {}

  Replace_resp() = default;

  bool operator==(Replace_resp const& rhs) const {
    return this == &rhs ||
           (req_id_ == rhs.req_id_ && user_id_ == rhs.user_id_ &&
            market_id_ == rhs.market_id_ &&
            canceled_order_id_ == rhs.canceled_order_id_ &&
            order_id_ == rhs.order_id_ && result_ == rhs.result_);
  }

  bool operator!=(Replace_resp const& rhs) const { return !(*this == rhs); }
  //! getter for req_id_ (access is Ro)
  Req_id_t req_id() const { return req_id_; }

  //! getter for user_id_ (access is Ro)
  User_id_t user_id() const { return user_id_; }

  //! getter for market_id_ (access is Ro)
  Market_id_t market_id() const { return market_id_; }

  //! getter for canceled_order_id_ (access is Ro)
  Order_id_t canceled_order_id() const { return canceled_order_id_; }

  //! getter for order_id_ (access is Ro)
  Order_id_t order_id() const { return order_id_; }

  //! getter for result_ (access is Ro)
  Replace_result result() const { return result_; }
  friend inline std::ostream& operator<<(std::ostream& out,
                                         Replace_resp const& item) {
    out << '\n' << "req_id:" << item.req_id_;
    out << '\n' << "user_id:" << item.user_id_;
    out << '\n' << "market_id:" << item.market_id_;
    out << '\n' << "canceled_order_id:" << item.canceled_order_id_;
    out << '\n' << "order_id:" << item.order_id_;
    out << '\n' << "result:" << item.result_;
    return out;
  }

  template <class Archive>
  void serialize(Archive& ar__) {
    ar__(cereal::make_nvp("req_id", req_id_));
    ar__(cereal::make_nvp("user_id", user_id_));
    ar__(cereal::make_nvp("market_id", market_id_));
    ar__(cereal::make_nvp("canceled_order_id", canceled_order_id_));
    ar__(cereal::make_nvp("order_id", order_id_));
    ar__(cereal::make_nvp("result", result_));
  }

  void serialize_to_json(std::ostream& out__) const {
    cereal::JSONOutputArchive ar__(out__);
    const_cast<Replace_resp*>(this)->serialize(ar__);
  }

  void serialize_from_json(std::istream& in__) {
    cereal::JSONInputArchive ar__{in__};
    serialize(ar__);
  }

 private:
  Req_id_t req_id_{};
  User_id_t user_id_{};
  Market_id_t market_id_{};
  Order_id_t canceled_order_id_{};
  Order_id_t order_id_{};
  Replace_result result_{};
};

class Market_details_req {
 public:
  Market_details_req(Req_id_t req_id, Market_id_t market_id, User_id_t user_id,
                     Timestamp_t start_time, Timestamp_t end_time,
                     bool include_active, bool include_dead, bool include_fills)
      : req_id_{req_id},
        market_id_{market_id},
        user_id_{user_id},
        start_time_{start_time},
        end_time_{end_time},
        include_active_{include_active},
        include_dead_{include_dead},
        include_fills_{include_fills} {}

  Market_details_req() = default;

  bool operator==(Market_details_req const& rhs) const {
    return this == &rhs ||
           (req_id_ == rhs.req_id_ && market_id_ == rhs.market_id_ &&
            user_id_ == rhs.user_id_ && start_time_ == rhs.start_time_ &&
            end_time_ == rhs.end_time_ &&
            include_active_ == rhs.include_active_ &&
            include_dead_ == rhs.include_dead_ &&
            include_fills_ == rhs.include_fills_);
  }

  bool operator!=(Market_details_req const& rhs) const {
    return !(*this == rhs);
  }
  //! getter for req_id_ (access is Ro)
  Req_id_t req_id() const { return req_id_; }

  //! getter for market_id_ (access is Ro)
  Market_id_t market_id() const { return market_id_; }

  //! getter for user_id_ (access is Ro)
  User_id_t user_id() const { return user_id_; }

  //! getter for start_time_ (access is Ro)
  Timestamp_t start_time() const { return start_time_; }

  //! getter for end_time_ (access is Ro)
  Timestamp_t end_time() const { return end_time_; }

  //! getter for include_active_ (access is Ro)
  bool include_active() const { return include_active_; }

  //! getter for include_dead_ (access is Ro)
  bool include_dead() const { return include_dead_; }

  //! getter for include_fills_ (access is Ro)
  bool include_fills() const { return include_fills_; }
  friend inline std::ostream& operator<<(std::ostream& out,
                                         Market_details_req const& item) {
    out << '\n' << "req_id:" << item.req_id_;
    out << '\n' << "market_id:" << item.market_id_;
    out << '\n' << "user_id:" << item.user_id_;
    out << '\n' << "start_time:" << item.start_time_;
    out << '\n' << "end_time:" << item.end_time_;
    out << '\n' << "include_active:" << item.include_active_;
    out << '\n' << "include_dead:" << item.include_dead_;
    out << '\n' << "include_fills:" << item.include_fills_;
    return out;
  }

  template <class Archive>
  void serialize(Archive& ar__) {
    ar__(cereal::make_nvp("req_id", req_id_));
    ar__(cereal::make_nvp("market_id", market_id_));
    ar__(cereal::make_nvp("user_id", user_id_));
    ar__(cereal::make_nvp("start_time", start_time_));
    ar__(cereal::make_nvp("end_time", end_time_));
    ar__(cereal::make_nvp("include_active", include_active_));
    ar__(cereal::make_nvp("include_dead", include_dead_));
    ar__(cereal::make_nvp("include_fills", include_fills_));
  }

  void serialize_to_json(std::ostream& out__) const {
    cereal::JSONOutputArchive ar__(out__);
    const_cast<Market_details_req*>(this)->serialize(ar__);
  }

  void serialize_from_json(std::istream& in__) {
    cereal::JSONInputArchive ar__{in__};
    serialize(ar__);
  }

  std::string serialize_to_dsv() const {
    fmt::MemoryWriter w__;
    w__ << req_id_ << ':' << market_id_ << ':' << user_id_ << ':'
        << fcs::timestamp::ticks(start_time_) << ':'
        << fcs::timestamp::ticks(end_time_) << ':' << include_active_ << ':'
        << include_dead_ << ':' << include_fills_;

    return w__.str();
  }

  void serialize_from_dsv(std::string const& tuple__) {
    using namespace boost;
    char_separator<char> const sep__{":"};
    tokenizer<char_separator<char> > tokens__(tuple__, sep__);
    tokenizer<boost::char_separator<char> >::iterator it__{tokens__.begin()};

    if (it__ != tokens__.end()) {
      req_id_ = lexical_cast<Req_id_t>(*it__);
      ++it__;
    } else {
      throw std::logic_error(
          "Tokenize Market_details_req failed: expected req_id_");
    }

    if (it__ != tokens__.end()) {
      market_id_ = lexical_cast<Market_id_t>(*it__);
      ++it__;
    } else {
      throw std::logic_error(
          "Tokenize Market_details_req failed: expected market_id_");
    }

    if (it__ != tokens__.end()) {
      user_id_ = lexical_cast<User_id_t>(*it__);
      ++it__;
    } else {
      throw std::logic_error(
          "Tokenize Market_details_req failed: expected user_id_");
    }

    if (it__ != tokens__.end()) {
      if (!fcs::timestamp::convert_to_timestamp_from_ticks(*it__,
                                                           start_time_)) {
        std::string msg{"Encountered invalid timestamp ticks:"};
        msg += *it__;
        throw std::logic_error(msg);
      }

      ++it__;
    } else {
      throw std::logic_error(
          "Tokenize Market_details_req failed: expected start_time_");
    }

    if (it__ != tokens__.end()) {
      if (!fcs::timestamp::convert_to_timestamp_from_ticks(*it__, end_time_)) {
        std::string msg{"Encountered invalid timestamp ticks:"};
        msg += *it__;
        throw std::logic_error(msg);
      }

      ++it__;
    } else {
      throw std::logic_error(
          "Tokenize Market_details_req failed: expected end_time_");
    }

    if (it__ != tokens__.end()) {
      include_active_ = lexical_cast<bool>(*it__);
      ++it__;
    } else {
      throw std::logic_error(
          "Tokenize Market_details_req failed: expected include_active_");
    }

    if (it__ != tokens__.end()) {
      include_dead_ = lexical_cast<bool>(*it__);
      ++it__;
    } else {
      throw std::logic_error(
          "Tokenize Market_details_req failed: expected include_dead_");
    }

    if (it__ != tokens__.end()) {
      include_fills_ = lexical_cast<bool>(*it__);
      ++it__;
    } else {
      throw std::logic_error(
          "Tokenize Market_details_req failed: expected include_fills_");
    }
  }

 private:
  Req_id_t req_id_{};
  Market_id_t market_id_{};
  /**
   If non-0 filters by user id
  */
  User_id_t user_id_{};
  /**
   Lower bound (inclusive) of time for orders/fills returned. This
   represents a filter on returned orders. Only orders within
   (start_time, end_time] are returned. Similarly only fills within
   (start_time, end_time] are returned. Note: with this setup for a
   market spanning multiple days a query on a day might return fills on
   that day for orders happening on a prior day. The prior day orders
   associated with the fill will not be returned.

   Note if end_time equals start_time all orders are returned
  */
  Timestamp_t start_time_{};
  /**
   Upper bound (exclusive) of time for orders/fills returned
  */
  Timestamp_t end_time_{};
  bool include_active_{};
  bool include_dead_{};
  bool include_fills_{};
};

class Market_details_resp {
 public:
  Market_details_resp(Req_id_t req_id, Market_id_t market_id,
                      Market_details_result result)
      : req_id_{req_id}, market_id_{market_id}, result_{result} {}
  Market_details_resp(Req_id_t req_id, Market_id_t market_id, Order_list_t bids,
                      Order_list_t asks, Order_list_t dead, Fill_list_t fills,
                      Market_details_result result)
      : req_id_{req_id},
        market_id_{market_id},
        bids_{bids},
        asks_{asks},
        dead_{dead},
        fills_{fills},
        result_{result} {}

  Market_details_resp() = default;

  bool operator==(Market_details_resp const& rhs) const {
    return this == &rhs ||
           (req_id_ == rhs.req_id_ && market_id_ == rhs.market_id_ &&
            bids_ == rhs.bids_ && asks_ == rhs.asks_ && dead_ == rhs.dead_ &&
            fills_ == rhs.fills_ && result_ == rhs.result_);
  }

  bool operator!=(Market_details_resp const& rhs) const {
    return !(*this == rhs);
  }
  //! getter for req_id_ (access is Ro)
  Req_id_t req_id() const { return req_id_; }

  //! getter for market_id_ (access is Ro)
  Market_id_t market_id() const { return market_id_; }

  //! getter for bids_ (access is Ro)
  Order_list_t bids() const { return bids_; }

  //! getter for asks_ (access is Ro)
  Order_list_t asks() const { return asks_; }

  //! getter for dead_ (access is Ro)
  Order_list_t dead() const { return dead_; }

  //! getter for fills_ (access is Ro)
  Fill_list_t fills() const { return fills_; }

  //! getter for result_ (access is Ro)
  Market_details_result result() const { return result_; }
  friend inline std::ostream& operator<<(std::ostream& out,
                                         Market_details_resp const& item) {
    using fcs::utils::streamers::operator<<;
    out << '\n' << "req_id:" << item.req_id_;
    out << '\n' << "market_id:" << item.market_id_;
    out << '\n' << "bids:" << item.bids_;
    out << '\n' << "asks:" << item.asks_;
    out << '\n' << "dead:" << item.dead_;
    out << '\n' << "fills:" << item.fills_;
    out << '\n' << "result:" << item.result_;
    return out;
  }

  template <class Archive>
  void serialize(Archive& ar__) {
    ar__(cereal::make_nvp("req_id", req_id_));
    ar__(cereal::make_nvp("market_id", market_id_));
    ar__(cereal::make_nvp("bids", bids_));
    ar__(cereal::make_nvp("asks", asks_));
    ar__(cereal::make_nvp("dead", dead_));
    ar__(cereal::make_nvp("fills", fills_));
    ar__(cereal::make_nvp("result", result_));
  }

  void serialize_to_json(std::ostream& out__) const {
    cereal::JSONOutputArchive ar__(out__);
    const_cast<Market_details_resp*>(this)->serialize(ar__);
  }

  void serialize_from_json(std::istream& in__) {
    cereal::JSONInputArchive ar__{in__};
    serialize(ar__);
  }

 private:
  Req_id_t req_id_{};
  Market_id_t market_id_{};
  Order_list_t bids_{};
  Order_list_t asks_{};
  Order_list_t dead_{};
  Fill_list_t fills_{};
  Market_details_result result_{};
};

class Log_req {
 public:
  Log_req(Log_type log_type, Market_id_t market_id)
      : log_type_{log_type}, market_id_{market_id} {}

  Log_req() = default;

  bool operator==(Log_req const& rhs) const {
    return this == &rhs ||
           (log_type_ == rhs.log_type_ && market_id_ == rhs.market_id_);
  }

  bool operator!=(Log_req const& rhs) const { return !(*this == rhs); }
  //! getter for log_type_ (access is Ro)
  Log_type log_type() const { return log_type_; }

  //! getter for market_id_ (access is Ro)
  Market_id_t market_id() const { return market_id_; }
  friend inline std::ostream& operator<<(std::ostream& out,
                                         Log_req const& item) {
    out << '\n' << "log_type:" << item.log_type_;
    out << '\n' << "market_id:" << item.market_id_;
    return out;
  }

  template <class Archive>
  void serialize(Archive& ar__) {
    ar__(cereal::make_nvp("log_type", log_type_));
    ar__(cereal::make_nvp("market_id", market_id_));
  }

  void serialize_to_json(std::ostream& out__) const {
    cereal::JSONOutputArchive ar__(out__);
    const_cast<Log_req*>(this)->serialize(ar__);
  }

  void serialize_from_json(std::istream& in__) {
    cereal::JSONInputArchive ar__{in__};
    serialize(ar__);
  }

  std::string serialize_to_dsv() const {
    fmt::MemoryWriter w__;
    w__ << log_type_ << ':' << market_id_;

    return w__.str();
  }

  void serialize_from_dsv(std::string const& tuple__) {
    using namespace boost;
    char_separator<char> const sep__{":"};
    tokenizer<char_separator<char> > tokens__(tuple__, sep__);
    tokenizer<boost::char_separator<char> >::iterator it__{tokens__.begin()};

    if (it__ != tokens__.end()) {
      log_type_ = Log_type(lexical_cast<int>(*it__));
      ++it__;
    } else {
      throw std::logic_error("Tokenize Log_req failed: expected log_type_");
    }

    if (it__ != tokens__.end()) {
      market_id_ = lexical_cast<Market_id_t>(*it__);
      ++it__;
    } else {
      throw std::logic_error("Tokenize Log_req failed: expected market_id_");
    }
  }

 private:
  Log_type log_type_{};
  Market_id_t market_id_{};
};

}  // namespace exch
#endif  // __EXCH_REQUESTS_HPP__
