#ifndef __EXCH_MARKET_HPP__
#define __EXCH_MARKET_HPP__

#include "exch/exch.hpp"
#include "exch/order_book.hpp"
#include <iosfwd>
#include <string>

namespace exch {
  class Managed_order;
  using Managed_order_list_t = std::vector< Managed_order >;

  class Market_config
  {
  public:
    Market_config(
      std::string name,
      Timestamp_t start_time,
      Timestamp_t end_time,
      int decimal_shift,
      int shifted_modulus,
      Price_t max_price) :
      name_ { name },
      start_time_ { start_time },
      end_time_ { end_time },
      decimal_shift_ { decimal_shift },
      shifted_modulus_ { shifted_modulus },
      max_price_ { max_price } {
    }

    //! getter for name_ (access is Ro)
    std::string name() const { return name_; }

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
    friend inline
    std::ostream& operator<<(std::ostream& out,
                             Market_config const& item) {
      out << '\n' << "name:" << item.name_;
      out << '\n' << "start_time:" << item.start_time_;
      out << '\n' << "end_time:" << item.end_time_;
      out << '\n' << "decimal_shift:" << item.decimal_shift_;
      out << '\n' << "shifted_modulus:" << item.shifted_modulus_;
      out << '\n' << "max_price:" << item.max_price_;
      return out;
    }

  private:
    std::string const name_;
    Timestamp_t const start_time_;
    Timestamp_t const end_time_;
    int const decimal_shift_;
    int const shifted_modulus_;
    Price_t const max_price_;

  };


  class Managed_order
  {
  public:
    Managed_order(
      Order_id_t order_id,
      Order order) :
      order_id { order_id },
      order { order } {
    }

    // custom <ClsPublic Managed_order>
    // end <ClsPublic Managed_order>

    Order_id_t const order_id {};
    Order const order;
    Order_state order_state { Submitted_e };
    Quantity_t filled { 0 };

  };


  /**
     Responsible for the exchange of a single market (e.g. one market id)
  */
  class Market_exchange
  {
  public:
    Market_exchange(
      Market_config market_config) :
      market_config_ { market_config } {
    }

    virtual ~Market_exchange() {}
    // custom <ClsPublic Market_exchange>

    virtual void submit(Order const& order) = 0;
    void submit(Order_list_t const& orders) {

    }

    virtual void cancel(Order const& order) = 0;
    void cancel(Order_list_t const& orders) {
    }

    virtual void replaceOrder(Order const& order) = 0;
    void replaceOrder(Order_list_t const& orders) {
    }

    // end <ClsPublic Market_exchange>

  private:
    Market_config market_config_;

  };


} // namespace exch
#endif // __EXCH_MARKET_HPP__
