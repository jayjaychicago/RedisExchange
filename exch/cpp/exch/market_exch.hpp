#ifndef __EXCH_MARKET_EXCH_HPP__
#define __EXCH_MARKET_EXCH_HPP__

#include "exch/exch.hpp"
#include "exch/order_book.hpp"
#include "fcs/utils/streamers/containers.hpp"
#include <iosfwd>
#include <string>

namespace exch {
class Market_config {
 public:
  Market_config(std::string const& name, Timestamp_t start_time,
                Timestamp_t end_time, int decimal_shift, int tick_size)
      : name_{name},
        start_time_{start_time},
        end_time_{end_time},
        decimal_shift_{decimal_shift},
        tick_size_{tick_size} {}

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
                                         Market_config const& item) {
    out << '\n' << "name:" << item.name_;
    out << '\n' << "start_time:" << item.start_time_;
    out << '\n' << "end_time:" << item.end_time_;
    out << '\n' << "decimal_shift:" << item.decimal_shift_;
    out << '\n' << "tick_size:" << item.tick_size_;
    return out;
  }

 private:
  std::string const name_;
  Timestamp_t const start_time_;
  Timestamp_t const end_time_;
  int const decimal_shift_;
  int const tick_size_;
};

class Market_stats {
 public:
  int active{0};
  int submits{0};
  int cancels{0};
  int replaces{0};
  int fills{0};

  friend inline std::ostream& operator<<(std::ostream& out,
                                         Market_stats const& item) {
    out << '\n' << "create_time:" << item.create_time_;
    out << '\n' << "active:" << item.active;
    out << '\n' << "submits:" << item.submits;
    out << '\n' << "cancels:" << item.cancels;
    out << '\n' << "replaces:" << item.replaces;
    out << '\n' << "fills:" << item.fills;
    return out;
  }

 private:
  Timestamp_t create_time_{fcs::timestamp::current_time()};
};

/**
 Responsible for the exchange of a single market (e.g. one market id)
*/
class Market_exchange {
 public:
  Market_exchange(Market_config const& market_config, Market_id_t market_id)
      : market_config_{market_config}, market_id_{market_id} {}

  // custom <ClsPublic Market_exchange>

  Submit_result submit(Order const& order) {
    fills_.clear();
    prices_affected_.clear();
    ++market_stats_.submits;

    if (order.is_bid()) {
      order_book_.process_bid(order, fills_, prices_affected_);
    } else {
      order_book_.process_ask(order, fills_, prices_affected_);
    }

    market_stats_.fills += fills_.size();

    if (false) {
      using fcs::utils::streamers::operator<<;
      std::cout << "Book after submit:\n" << order_book_ << std::endl;
      std::cout << "Fills:\n" << fills_ << std::endl;
      std::cout << "Prices Affected:\n" << prices_affected_ << std::endl;
    }

    update_active();
    return Submit_result();
  }

  Cancel_result cancel(Order_id_t order_id) {
    ++market_stats_.cancels;
    bool canceled = order_book_.cancel(order_id);
    if(canceled) {
      update_active();
    }
    return canceled? Cancel_succeeded_e : Cancel_invalid_order_e;
  }

  Replace_result replace_order(Order_id_t original, Order const& order) {
    std::cout << "replace being processed:" << original << " replaced with "
              << order << std::endl;
    ++market_stats_.replaces;
    update_active();
    return Replace_result();
  }

  void update_active() { market_stats_.active = order_book_.num_active(); }

  Order_id_t next_order_id() { return ++next_order_id_; }

  // end <ClsPublic Market_exchange>

  //! getter for market_config_ (access is Ro)
  Market_config const& market_config() const { return market_config_; }

  //! getter for market_stats_ (access is Ro)
  Market_stats const& market_stats() const { return market_stats_; }

  //! getter for order_book_ (access is Ro)
  Order_book const& order_book() const { return order_book_; }

  //! getter for fills_ (access is Ro)
  Fill_list_t const& fills() const { return fills_; }

 private:
  Market_config market_config_;
  Market_id_t const market_id_{};
  int next_order_id_{0};
  Market_stats market_stats_;
  Order_book order_book_{};
  Fill_list_t fills_{32};
  Price_list_t prices_affected_{32};
  Managed_order_list_t dead_orders_{};
  Quantity_t net_volume_{};
};

}  // namespace exch
#endif  // __EXCH_MARKET_EXCH_HPP__
