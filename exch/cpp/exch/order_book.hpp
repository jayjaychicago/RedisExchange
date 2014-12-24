#ifndef __EXCH_ORDER_BOOK_HPP__
#define __EXCH_ORDER_BOOK_HPP__

#include "cereal/archives/json.hpp"
#include "cereal/cereal.hpp"
#include "exch/exch.hpp"
#include "exch/fill.hpp"
#include "fcs/timestamp/cereal.hpp"
#include "fcs/utils/streamers/containers.hpp"
#include "fcs/utils/streamers/table.hpp"
#include <boost/range/adaptor/reversed.hpp>
#include <functional>
#include <iosfwd>
#include <map>
#include <sstream>
#include <vector>

namespace exch {
// custom <FcbBeginNamespace order_book>

using Tokenizer_t = boost::tokenizer<boost::char_separator<char> >;
using Token_iter_t = boost::tokenizer<boost::char_separator<char> >::iterator;

template <typename T>
inline bool next_token(Tokenizer_t& tokens, Token_iter_t& it, T& result) {
  if (it != tokens.end()) {
    result = boost::lexical_cast<T>(*it);
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

class Fill;
using Fill_list_t = std::vector<Fill>;
using Price_list_t = std::vector<Price_t>;

enum Order_state { Submitted_e, Active_e, Canceled_e, Filled_e };

struct Book_entry {
  Book_entry(Price_t price, Quantity_t quantity)
      : price{price}, quantity{quantity} {}

  Book_entry() = default;
  Price_t price;
  Quantity_t quantity;

  friend inline std::ostream& operator<<(std::ostream& out,
                                         Book_entry const& item) {
    out << '\n' << "price:" << item.price;
    out << '\n' << "quantity:" << item.quantity;
    return out;
  }
};

struct Market {
  Market(Book_entry bid, Book_entry ask) : bid{bid}, ask{ask} {}

  Market() = default;
  Book_entry bid;
  Book_entry ask;

  friend inline std::ostream& operator<<(std::ostream& out,
                                         Market const& item) {
    out << '\n' << "bid:" << item.bid;
    out << '\n' << "ask:" << item.ask;
    return out;
  }
};

class Order {
 public:
  Order(Order_id_t order_id, Timestamp_t timestamp, Side side, Price_t price,
        Quantity_t quantity)
      : order_id_{order_id},
        timestamp_{timestamp},
        side_{side},
        price_{price},
        quantity_{quantity} {}

  // custom <ClsPublic Order>

  std::string to_tuple() const {
    using namespace fcs::timestamp;
    using namespace boost;
    using namespace std;
    std::stringstream out;
    out << lexical_cast<string>(order_id_) << ':'
        << lexical_cast<string>(ticks(timestamp_)) << ':'
        << lexical_cast<string>(side_ == Bid_side_e ? 'B' : 'S') << ':'
        << lexical_cast<string>(price_) << ':'
        << lexical_cast<string>(quantity_);
    return out.str();
  }

  static Order from_tuple(std::string const& tuple) {
    using namespace boost;
    using namespace fcs::timestamp;
    char_separator<char> sep{":"};
    tokenizer<char_separator<char> > tokens(tuple, sep);
    boost::tokenizer<boost::char_separator<char> >::iterator it{tokens.begin()};

    Order_id_t order_id;
    long long ticks;
    char side;
    Quantity_t quantity;
    Price_t price;

    if (!next_token(tokens, it, order_id)) invalid_order(tuple);
    if (!next_token(tokens, it, ticks)) invalid_order(tuple);
    if (!next_token(tokens, it, side)) invalid_order(tuple);
    if (!next_token(tokens, it, price)) invalid_order(tuple);
    if (!next_token(tokens, it, quantity)) invalid_order(tuple);

    return Order(order_id, Timestamp_t(Timestamp_t::time_rep_type(ticks)),
                 side == 'B' ? Bid_side_e : Ask_side_e, price, quantity);
  }

  bool is_bid() const { return side_ == Bid_side_e; }

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
  friend inline std::ostream& operator<<(std::ostream& out, Order const& item) {
    out << '\n' << "order_id:" << item.order_id_;
    out << '\n' << "timestamp:" << item.timestamp_;
    out << '\n' << "side:" << item.side_;
    out << '\n' << "price:" << item.price_;
    out << '\n' << "quantity:" << item.quantity_;
    return out;
  }

  template <class Archive>
  void serialize(Archive& ar__) {
    ar__(cereal::make_nvp("order_id", order_id_));
    ar__(cereal::make_nvp("timestamp", timestamp_));
    ar__(cereal::make_nvp("side", side_));
    ar__(cereal::make_nvp("price", price_));
    ar__(cereal::make_nvp("quantity", quantity_));
  }

  void serialize_to_json(std::ostream& out__) const {
    cereal::JSONOutputArchive ar__(out__);
    const_cast<Order*>(this)->serialize(ar__);
  }

  void serialize_from_json(std::istream& in__) {
    cereal::JSONInputArchive ar__{in__};
    serialize(ar__);
  }

 private:
  Order_id_t order_id_{};
  Timestamp_t timestamp_{};
  Side side_{};
  Price_t price_{};
  Quantity_t quantity_{};
};

class Managed_order {
 public:
  Managed_order(Order const& order) : order{order} {}

  // custom <ClsPublic Managed_order>

  Quantity_t quantity() const { return order.quantity(); }

  Price_t price() const { return order.price(); }

  Quantity_t remaining_quantity() const {
    assert(filled_ <= quantity());
    return quantity() - filled_;
  }

  Order_id_t order_id() const { return order.order_id(); }

  Side side() const { return order.side(); }

  void fill_quantity(Quantity_t qty) {
    assert(qty <= remaining_quantity());
    filled_ += qty;
  }

  // end <ClsPublic Managed_order>

  Order order;
  Order_state order_state{Submitted_e};

  friend inline std::ostream& operator<<(std::ostream& out,
                                         Managed_order const& item) {
    out << '\n' << "order:" << item.order;
    out << '\n' << "order_state:" << item.order_state;
    out << '\n' << "filled:" << item.filled_;
    return out;
  }

 private:
  Quantity_t filled_{0};
};

using Managed_order_list_t = std::vector<Managed_order>;

// custom <ClsPostDecl Managed_order>

template <typename IT>
std::string orders_display_string(IT begin, IT end, size_t max_shows = 4) {
  Quantity_t total{0};
  fmt::MemoryWriter w;
  size_t shown{0};
  for (; begin != end; ++begin) {
    Managed_order const& order{*begin};
    auto const quantity = order.remaining_quantity();
    total += quantity;
    if (shown++ <= max_shows) {
      w << '[' << order.order_id() << ' ' << quantity << ']';
    }
  }
  return w.str();
}

// end <ClsPostDecl Managed_order>

class Order_book {
 public:
  using Bid_compare_t = std::greater<Price_t>;
  using Bids_t = std::map<Price_t, Managed_order_list_t, Bid_compare_t>;
  using Asks_t = std::map<Price_t, Managed_order_list_t>;
  using Active_map_t = std::map<Order_id_t, Price_t>;

  // custom <ClsPublic Order_book>

  void process_bid(Order const& bid, Fill_list_t& fills,
                   Price_list_t& affected) {
    if (false) {
      std::cout << "Processing bid " << bid << std::endl;
    }

    Managed_order managed_bid{bid};
    auto const bid_price = managed_bid.price();

    Asks_t::iterator it{asks_.begin()};
    Asks_t::iterator end{asks_.end()};

    for (; it != end;) {
      Managed_order_list_t& ask_orders = it->second;
      size_t delete_to{0};
      for (auto& ask : ask_orders) {
        auto const ask_price = ask.price();
        auto remaining = managed_bid.remaining_quantity();
        if (remaining == 0 || bid_price < ask_price) {
          break;
        } else {
          auto const ask_quantity = ask.remaining_quantity();
          affected.push_back(ask_price);
          Quantity_t matched = std::min(remaining, ask_quantity);
          ask.fill_quantity(matched);
          managed_bid.fill_quantity(matched);
          if (ask.remaining_quantity() == 0) {
            remove_active_order(ask);
            ++delete_to;
            // TODO: set state to dead/fully filled
          }
          fills.emplace_back(next_fill_id(), bid.timestamp(), bid.order_id(),
                             ask.order_id(), ask_price, matched);
        }
      }

      if (delete_to != 0) {
        if (delete_to == ask_orders.size()) {
          asks_.erase(it++);
          continue;
        } else {
          auto const begin = ask_orders.begin();
          ask_orders.erase(begin, begin + delete_to);
        }
      }
      ++it;
    }

    assert(managed_bid.remaining_quantity() >= 0);
    if (managed_bid.remaining_quantity() > 0) {
      using Insert_result_t = std::pair<Bids_t::iterator, bool>;
      Insert_result_t insert_result{
          bids_.insert(Bids_t::value_type(bid_price, Managed_order_list_t()))};

      Managed_order_list_t& orders{insert_result.first->second};
      add_order(managed_bid);
      orders.push_back(managed_bid);

      if (affected.empty() || affected.back() != bid_price) {
        affected.emplace_back(bid_price);
      }
    }
    assert(active_order_invariant());
  }

  void process_ask(Order const& ask, Fill_list_t& fills,
                   Price_list_t& affected) {
    if (false) {
      std::cout << "Processing ask " << ask << std::endl;
    }
    Managed_order managed_ask{ask};
    auto const ask_price = managed_ask.price();

    Bids_t::iterator it{bids_.begin()};
    Bids_t::iterator end{bids_.end()};

    for (; it != end;) {
      Managed_order_list_t& bid_orders = it->second;
      size_t delete_to{0};
      for (auto& bid : bid_orders) {
        auto remaining = managed_ask.remaining_quantity();
        auto const bid_price = bid.price();
        if (remaining == 0 || ask_price > bid_price) {
          break;
        } else {
          auto const bid_quantity = bid.remaining_quantity();
          affected.emplace_back(bid_price);
          Quantity_t matched = std::min(bid_quantity, remaining);
          bid.fill_quantity(matched);
          managed_ask.fill_quantity(matched);
          fills.emplace_back(next_fill_id(), ask.timestamp(), bid.order_id(),
                             ask.order_id(), bid_price, matched);
          if (bid.remaining_quantity() == 0) {
            remove_active_order(bid);
            ++delete_to;
          }
        }
      }

      if (delete_to != 0) {
        if (delete_to == bid_orders.size()) {
          bids_.erase(it++);
          continue;
        } else {
          auto const begin = bid_orders.begin();
          bid_orders.erase(begin, begin + delete_to);
        }
      }
      ++it;
    }

    assert(managed_ask.remaining_quantity() >= 0);
    if (managed_ask.remaining_quantity() > 0) {
      using Insert_result_t = std::pair<Asks_t::iterator, bool>;
      Insert_result_t insert_result{
          asks_.insert(Asks_t::value_type(ask_price, Managed_order_list_t()))};

      Managed_order_list_t& orders = {insert_result.first->second};
      add_order(managed_ask);
      orders.push_back(managed_ask);

      if (affected.empty() || affected.back() != ask_price) {
        affected.emplace_back(ask_price);
      }
    }

    assert(active_order_invariant());
  }

  bool cancel(Order_id_t order_id) {
    bool result{false};
    Active_map_t::iterator found = active_map_.find(order_id);
    if (found != active_map_.end()) {
      Price_t price = found->second;
      std::cout << "Cancelling " << order_id << " at px " << price << std::endl;
      if (price < 0) {
        result = remove_order_from_book(asks_, -price, order_id);
      } else {
        result = remove_order_from_book(bids_, price, order_id);
      }
    }
    return result;
  }

  template <typename MAP>
  bool remove_order_from_book(MAP& map, Price_t price, Order_id_t order_id) {
    bool result{false};
    auto orders_at_price = map.find(price);
    if (orders_at_price != map.end()) {
      Managed_order_list_t& orders{orders_at_price->second};
      result = remove_order_from_list(orders, order_id);
      if (result && orders.empty()) {
        map.erase(price);
      }
    }
    return result;
  }

  bool remove_order_from_list(Managed_order_list_t& orders,
                              Order_id_t order_id) {
    bool result{false};
    auto found_order = std::find_if(orders.begin(), orders.end(),
                                    [=](Managed_order const& order)->bool {
      return order.order_id() == order_id;
    });

    if (found_order != orders.end()) {
      orders.erase(found_order);
      result = true;
    }
    return result;
  }

  Fill_id_t next_fill_id() { return ++next_fill_id_; }

  size_t num_active() const { return active_map_.size(); }

  friend inline std::ostream& operator<<(std::ostream& out,
                                         Order_book const& item) {
    using namespace fcs::utils::streamers;
    using fcs::utils::streamers::operator<<;

    String_list_t header{"bids", "bid", "ask", "asks"};
    auto const& asks = item.asks_;
    auto const& bids = item.bids_;

    String_matrix_t table{asks.size() + bids.size(), String_list_t{4}};
    size_t i{0};
    auto asks_it = asks.rbegin();
    auto asks_end = asks.rend();

    for (; asks_it != asks_end; ++asks_it) {
      auto const& ask_pair = *asks_it;
      Managed_order_list_t const& orders{ask_pair.second};
      table[i][2] = boost::lexical_cast<std::string>(ask_pair.first);
      table[i][3] = orders_display_string(orders.begin(), orders.end());
      ++i;
    }

    for (auto& bid_pair : bids) {
      Managed_order_list_t const& orders{bid_pair.second};
      table[i][1] = boost::lexical_cast<std::string>(bid_pair.first);
      table[i][0] = orders_display_string(orders.rbegin(), orders.rend());
      ++i;
    }
    Table_streamer<> table_streamer{table, header};
    table_streamer.print_string_table(out);
    return out;
  }

  // end <ClsPublic Order_book>

 private:
  // custom <ClsPrivate Order_book>

  void add_order(Managed_order const& order) {
    assert(active_map_.find(order.order_id()) == active_map_.end());
    active_map_.insert(Active_map_t::value_type(
        order.order_id(),
        order.side() == Bid_side_e ? order.price() : -order.price()));
  }

  void remove_active_order(Managed_order const& order) {
    assert(active_map_.find(order.order_id()) != active_map_.end());
    active_map_.erase(order.order_id());
  }

  bool active_order_invariant() const {
    return active_map_.size() == count_orders(bids_) + count_orders(asks_);
  }

  template <typename MAP>
  size_t count_orders(MAP const& map) const {
    size_t result{0};
    for (auto const& pair : map) {
      Managed_order_list_t const& orders{pair.second};
      result += orders.size();
    }
    return result;
  }

  // end <ClsPrivate Order_book>

  Bids_t bids_{};
  Asks_t asks_{};
  int next_fill_id_{0};
  Active_map_t active_map_{};
};

}  // namespace exch
#endif  // __EXCH_ORDER_BOOK_HPP__
