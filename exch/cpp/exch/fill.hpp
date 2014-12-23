#ifndef __EXCH_FILL_HPP__
#define __EXCH_FILL_HPP__

#include "cereal/archives/json.hpp"
#include "cereal/cereal.hpp"
#include "cppformat/format.h"
#include "exch/exch.hpp"
#include "fcs/timestamp/cereal.hpp"
#include "fcs/timestamp/conversion.hpp"
#include <iosfwd>

namespace exch {
class Fill {
 public:
  Fill(Fill_id_t fill_id, Timestamp_t timestamp, Order_id_t bid_id,
       Order_id_t ask_id, Price_t price, Quantity_t quantity)
      : fill_id_{fill_id},
        timestamp_{timestamp},
        bid_id_{bid_id},
        ask_id_{ask_id},
        price_{price},
        quantity_{quantity} {}

  Fill() = default;
  // custom <ClsPublic Fill>
  // end <ClsPublic Fill>

  //! getter for fill_id_ (access is Ro)
  Fill_id_t fill_id() const { return fill_id_; }

  //! getter for timestamp_ (access is Ro)
  Timestamp_t timestamp() const { return timestamp_; }

  //! getter for bid_id_ (access is Ro)
  Order_id_t bid_id() const { return bid_id_; }

  //! getter for ask_id_ (access is Ro)
  Order_id_t ask_id() const { return ask_id_; }

  //! getter for price_ (access is Ro)
  Price_t price() const { return price_; }

  //! getter for quantity_ (access is Ro)
  Quantity_t quantity() const { return quantity_; }
  friend inline std::ostream& operator<<(std::ostream& out, Fill const& item) {
    out << '\n' << "fill_id:" << item.fill_id_;
    out << '\n' << "timestamp:" << item.timestamp_;
    out << '\n' << "bid_id:" << item.bid_id_;
    out << '\n' << "ask_id:" << item.ask_id_;
    out << '\n' << "price:" << item.price_;
    out << '\n' << "quantity:" << item.quantity_;
    return out;
  }

  template <class Archive>
  void serialize(Archive& ar__) {
    ar__(cereal::make_nvp("fill_id", fill_id_));
    ar__(cereal::make_nvp("timestamp", timestamp_));
    ar__(cereal::make_nvp("bid_id", bid_id_));
    ar__(cereal::make_nvp("ask_id", ask_id_));
    ar__(cereal::make_nvp("price", price_));
    ar__(cereal::make_nvp("quantity", quantity_));
  }

  void serialize_to_json(std::ostream& out__) const {
    cereal::JSONOutputArchive ar__(out__);
    const_cast<Fill*>(this)->serialize(ar__);
  }

  void serialize_from_json(std::istream& in__) {
    cereal::JSONInputArchive ar__{in__};
    serialize(ar__);
  }

  std::string serialize_to_dsv() const {
    fmt::MemoryWriter w__;
    w__ << fill_id_ << ':' << fcs::timestamp::ticks(timestamp_) << ':'
        << bid_id_ << ':' << ask_id_ << ':' << price_ << ':' << quantity_;

    return w__.str();
  }

  static Fill serialize_from_dsv(std::string const& tuple__) {
    using namespace boost;
    char_separator<char> const sep__{":"};
    tokenizer<char_separator<char> > tokens__(tuple__, sep__);
    tokenizer<boost::char_separator<char> >::iterator it__{tokens__.begin()};

    Fill_id_t fill_id_;
    Timestamp_t timestamp_;
    Order_id_t bid_id_;
    Order_id_t ask_id_;
    Price_t price_;
    Quantity_t quantity_;

    if (it__ != tokens__.end()) {
      fill_id_ = lexical_cast<Fill_id_t>(*it__);
      ++it__;
    } else {
      throw std::logic_error("Tokenize Fill failed: expected fill_id_");
    }

    if (it__ != tokens__.end()) {
      if (!fcs::timestamp::convert_to_timestamp_from_ticks(*it__, timestamp_)) {
        std::string msg{"Encountered invalid timestamp ticks:"};
        msg += *it__;
        throw std::logic_error(msg);
      }

      ++it__;
    } else {
      throw std::logic_error("Tokenize Fill failed: expected timestamp_");
    }

    if (it__ != tokens__.end()) {
      bid_id_ = lexical_cast<Order_id_t>(*it__);
      ++it__;
    } else {
      throw std::logic_error("Tokenize Fill failed: expected bid_id_");
    }

    if (it__ != tokens__.end()) {
      ask_id_ = lexical_cast<Order_id_t>(*it__);
      ++it__;
    } else {
      throw std::logic_error("Tokenize Fill failed: expected ask_id_");
    }

    if (it__ != tokens__.end()) {
      price_ = lexical_cast<Price_t>(*it__);
      ++it__;
    } else {
      throw std::logic_error("Tokenize Fill failed: expected price_");
    }

    if (it__ != tokens__.end()) {
      quantity_ = lexical_cast<Quantity_t>(*it__);
      ++it__;
    } else {
      throw std::logic_error("Tokenize Fill failed: expected quantity_");
    }

    return Fill(fill_id_, timestamp_, bid_id_, ask_id_, price_, quantity_);
  }

 private:
  Fill_id_t const fill_id_{};
  Timestamp_t const timestamp_{};
  Order_id_t const bid_id_{};
  Order_id_t const ask_id_{};
  Price_t const price_{};
  Quantity_t const quantity_{};
};

}  // namespace exch
#endif  // __EXCH_FILL_HPP__
