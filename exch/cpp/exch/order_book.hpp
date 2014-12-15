#ifndef __EXCH_ORDER_BOOK_HPP__
#define __EXCH_ORDER_BOOK_HPP__

#include "cereal/archives/json.hpp"
#include "cereal/cereal.hpp"
#include "exch/exch.hpp"
#include "fcs/timestamp/cereal.hpp"
#include <iosfwd>
#include <sstream>

namespace exch {
  // custom <FcbBeginNamespace order_book>

  using Tokenizer_t = boost::tokenizer< boost::char_separator<char> >;
  using Token_iter_t = boost::tokenizer< boost::char_separator<char> >::iterator;

  template< typename T >
  inline
  bool next_token(Tokenizer_t & tokens,
                  Token_iter_t & it,
                  T & result) {
    if(it != tokens.end()) {
      result = boost::lexical_cast< T >(*it);
      it++;
      return true;
    }
    return false;
  }

  inline void invalid_order(std::string const& order) {
    std::stringstream msg;
    msg << "Order is invalid: " << order;
    throw std::logic_error(msg.str());
  }


  // end <FcbBeginNamespace order_book>

enum Order_state { Submitted_e, Active_e, Canceled_e, Filled_e };

struct Book_entry {
  Book_entry(Price_t price, Quantity_t quantity)
      : price{ price }, quantity{ quantity } {}

  Book_entry() = default;
  Price_t price;
  Quantity_t quantity;

  friend inline std::ostream &operator<<(std::ostream &out,
                                         Book_entry const &item) {
    out << '\n' << "price:" << item.price;
    out << '\n' << "quantity:" << item.quantity;
    return out;
  }
};

struct Market {
  Market(Book_entry bid, Book_entry ask) : bid{ bid }, ask{ ask } {}

  Market() = default;
  Book_entry bid;
  Book_entry ask;

  friend inline std::ostream &operator<<(std::ostream &out,
                                         Market const &item) {
    out << '\n' << "bid:" << item.bid;
    out << '\n' << "ask:" << item.ask;
    return out;
  }
};

class Order {
public:
  Order(Order_id_t order_id, Timestamp_t timestamp, Side side, Price_t price,
        Quantity_t quantity)
      : order_id_{ order_id }, timestamp_{ timestamp }, side_{ side },
        price_{ price }, quantity_{ quantity } {}

    // custom <ClsPublic Order>

    std::string to_tuple() const {
      using namespace fcs::timestamp;
      using namespace boost;
      using namespace std;
      std::stringstream out;
      out << lexical_cast< string >(order_id_) << ':'
          << lexical_cast< string >(ticks(timestamp_)) << ':'
          << lexical_cast< string >(side_ == Bid_side_e? 'B' : 'S') << ':'
          << lexical_cast< string >(price_) << ':'
          << lexical_cast< string >(quantity_);
      return out.str();
    }

    static Order from_tuple(std::string const& tuple) {
      using namespace boost;
      using namespace fcs::timestamp;
      char_separator<char> sep {":"};
      tokenizer< char_separator<char> > tokens(tuple, sep);
      boost::tokenizer< boost::char_separator<char> >::iterator it { tokens.begin() };

      Order_id_t order_id;
      long long ticks;
      char side;
      Quantity_t quantity;
      Price_t price;

      if(!next_token(tokens, it, order_id)) invalid_order(tuple);
      if(!next_token(tokens, it, ticks)) invalid_order(tuple);
      if(!next_token(tokens, it, side)) invalid_order(tuple);
      if(!next_token(tokens, it, price)) invalid_order(tuple);
      if(!next_token(tokens, it, quantity)) invalid_order(tuple);

      return Order(order_id,
                   Timestamp_t(Timestamp_t::time_rep_type(ticks)),
                   side == 'B'? Bid_side_e : Ask_side_e,
                   price, quantity);
    }

    // end <ClsPublic Order>

  //! getter for order_id_ (access is Ro)
  Order_id_t order_id() const { return order_id_; }

  //! getter for timestamp_ (access is Ro)
  Timestamp_t timestamp() const { return timestamp_; }

  //! getter for side_ (access is Ro)
  Side side() const { return side_; }

  //! getter for price_ (access is Ro)
  Price_t price() const { return price_; }

  //! getter for quantity_ (access is Ro)
  Quantity_t quantity() const { return quantity_; }
  friend inline std::ostream &operator<<(std::ostream &out, Order const &item) {
    out << '\n' << "order_id:" << item.order_id_;
    out << '\n' << "timestamp:" << item.timestamp_;
    out << '\n' << "side:" << item.side_;
    out << '\n' << "price:" << item.price_;
    out << '\n' << "quantity:" << item.quantity_;
    return out;
  }

  template <class Archive> void serialize(Archive &ar__) {
    ar__(cereal::make_nvp("order_id", order_id_));
    ar__(cereal::make_nvp("timestamp", timestamp_));
    ar__(cereal::make_nvp("side", side_));
    ar__(cereal::make_nvp("price", price_));
    ar__(cereal::make_nvp("quantity", quantity_));
  }

  void serialize_to_json(std::ostream &out__) const {
    cereal::JSONOutputArchive ar__(out__);
    const_cast<Order *>(this)->serialize(ar__);
  }

  void serialize_from_json(std::istream &in__) {
    cereal::JSONInputArchive ar__{ in__ };
    serialize(ar__);
  }

private:
  Order_id_t const order_id_;
  Timestamp_t const timestamp_;
  Side const side_;
  Price_t const price_;
  Quantity_t const quantity_;
};

class Fill {
public:
  Fill(Fill_id_t fill_id, Timestamp_t timestamp, Order_id_t order_id, Side side,
       Price_t price, Quantity_t quantity)
      : fill_id_{ fill_id }, timestamp_{ timestamp }, order_id_{ order_id },
        side_{ side }, price_{ price }, quantity_{ quantity } {}

    // custom <ClsPublic Fill>

    std::string to_tuple() const {
      using namespace fcs::timestamp;
      using namespace boost;
      using namespace std;
      std::stringstream out;
      out << lexical_cast< string >(fill_id_) << ':'
          << lexical_cast< string >(ticks(timestamp_)) << ':'
          << lexical_cast< string >(order_id_) << ':'
          << lexical_cast< string >(side_ == Bid_side_e? 'B' : 'S') << ':'
          << lexical_cast< string >(price_) << ':'
          << lexical_cast< string >(quantity_);
      return out.str();
    }

    static Fill from_tuple(std::string const& tuple) {
      using namespace boost;
      using namespace fcs::timestamp;
      char_separator<char> sep {":"};
      tokenizer< char_separator<char> > tokens(tuple, sep);
      boost::tokenizer< boost::char_separator<char> >::iterator it { tokens.begin() };

      Fill_id_t fill_id;
      long long ticks;
      Order_id_t order_id;
      char side;
      Quantity_t quantity;
      Price_t price;

      if(!next_token(tokens, it, fill_id)) invalid_order(tuple);
      if(!next_token(tokens, it, ticks)) invalid_order(tuple);
      if(!next_token(tokens, it, order_id)) invalid_order(tuple);
      if(!next_token(tokens, it, side)) invalid_order(tuple);
      if(!next_token(tokens, it, price)) invalid_order(tuple);
      if(!next_token(tokens, it, quantity)) invalid_order(tuple);

      return Fill(fill_id,
                  Timestamp_t(Timestamp_t::time_rep_type(ticks)),
                  order_id,
                  side == 'B'? Bid_side_e : Ask_side_e,
                  price,
                  quantity);
    }

    // end <ClsPublic Fill>

  //! getter for fill_id_ (access is Ro)
  Fill_id_t fill_id() const { return fill_id_; }

  //! getter for timestamp_ (access is Ro)
  Timestamp_t timestamp() const { return timestamp_; }

  //! getter for order_id_ (access is Ro)
  Order_id_t order_id() const { return order_id_; }

  //! getter for side_ (access is Ro)
  Side side() const { return side_; }

  //! getter for price_ (access is Ro)
  Price_t price() const { return price_; }

  //! getter for quantity_ (access is Ro)
  Quantity_t quantity() const { return quantity_; }
  friend inline std::ostream &operator<<(std::ostream &out, Fill const &item) {
    out << '\n' << "fill_id:" << item.fill_id_;
    out << '\n' << "timestamp:" << item.timestamp_;
    out << '\n' << "order_id:" << item.order_id_;
    out << '\n' << "side:" << item.side_;
    out << '\n' << "price:" << item.price_;
    out << '\n' << "quantity:" << item.quantity_;
    return out;
  }

  template <class Archive> void serialize(Archive &ar__) {
    ar__(cereal::make_nvp("fill_id", fill_id_));
    ar__(cereal::make_nvp("timestamp", timestamp_));
    ar__(cereal::make_nvp("order_id", order_id_));
    ar__(cereal::make_nvp("side", side_));
    ar__(cereal::make_nvp("price", price_));
    ar__(cereal::make_nvp("quantity", quantity_));
  }

  void serialize_to_json(std::ostream &out__) const {
    cereal::JSONOutputArchive ar__(out__);
    const_cast<Fill *>(this)->serialize(ar__);
  }

  void serialize_from_json(std::istream &in__) {
    cereal::JSONInputArchive ar__{ in__ };
    serialize(ar__);
  }

private:
  Fill_id_t const fill_id_;
  Timestamp_t const timestamp_;
  Order_id_t const order_id_;
  Side const side_;
  Price_t const price_;
  Quantity_t const quantity_;
};

} // namespace exch
#endif // __EXCH_ORDER_BOOK_HPP__
