#ifndef __EXCH_MARKET_EXCH_HPP__
#define __EXCH_MARKET_EXCH_HPP__

#include "exch/exch.hpp"
#include "exch/order_book.hpp"
#include <iosfwd>
#include <string>

namespace exch {
class Market_config {
public:
  Market_config(std::string const &name, Timestamp_t start_time,
                Timestamp_t end_time, int decimal_shift, int tick_size)
      : name_{ name }, start_time_{ start_time }, end_time_{ end_time },
        decimal_shift_{ decimal_shift }, tick_size_{ tick_size } {}

  //! getter for name_ (access is Ro)
  std::string const &name() const { return name_; }

  //! getter for start_time_ (access is Ro)
  Timestamp_t start_time() const { return start_time_; }

  //! getter for end_time_ (access is Ro)
  Timestamp_t end_time() const { return end_time_; }

  //! getter for decimal_shift_ (access is Ro)
  int decimal_shift() const { return decimal_shift_; }

  //! getter for tick_size_ (access is Ro)
  int tick_size() const { return tick_size_; }
  friend inline std::ostream &operator<<(std::ostream &out,
                                         Market_config const &item) {
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

class Managed_order {
public:
  Managed_order(Order const &order) : order{ order } {}

    // custom <ClsPublic Managed_order>
    // end <ClsPublic Managed_order>

  Order const order;
  Order_state order_state{ Submitted_e };
  Quantity_t filled{ 0 };
};

/**
 Responsible for the exchange of a single market (e.g. one market id)
*/
class Market_exchange {
public:
  using Managed_order_list_t = std::vector<Managed_order>;

  Market_exchange(Market_config const &market_config, Market_id_t market_id)
      : market_config_{ market_config }, market_id_{ market_id } {}

    // custom <ClsPublic Market_exchange>

    Submit_result submit(Order const& order) {
      return Submit_result();
    }

    Cancel_result cancel(Order_id_t const& order_id) {
      return Cancel_result();
    }

    Replace_result replace_order(Order_id_t original, Order const& order) {
      return Replace_result();
    }

    Order_id_t next_order_id() { return ++next_order_id_; }

    // end <ClsPublic Market_exchange>

private:
  Market_config market_config_;
  Market_id_t const market_id_{};
  int next_order_id_{ 0 };
  Managed_order_list_t active_orders_{};
  Managed_order_list_t dead_orders_{};
  Quantity_t net_volume_{};
};

} // namespace exch
#endif // __EXCH_MARKET_EXCH_HPP__