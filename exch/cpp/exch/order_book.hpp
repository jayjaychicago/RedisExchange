#ifndef __EXCH_ORDER_BOOK_HPP__
#define __EXCH_ORDER_BOOK_HPP__

#include "exch/exch.hpp"
#include <iosfwd>

namespace exch {
  enum Side {
    Bid_side_e,
    Ask_side_e
  };

  enum Order_state {
    Submitted_e,
    Active_e,
    Canceled_e,
    Filled_e
  };

  struct Book_entry
  {
    Book_entry(
      Price_t price,
      Quantity_t quantity) :
      price { price },
      quantity { quantity } {
    }

    Book_entry() = default;
    Price_t price;
    Quantity_t quantity;

    friend inline
    std::ostream& operator<<(std::ostream& out,
                             Book_entry const& item) {
      out << '\n' << "price:" << item.price;
      out << '\n' << "quantity:" << item.quantity;
      return out;
    }

  };


  struct Market
  {
    Market(
      Book_entry bid,
      Book_entry ask) :
      bid { bid },
      ask { ask } {
    }

    Market() = default;
    Book_entry bid;
    Book_entry ask;

    friend inline
    std::ostream& operator<<(std::ostream& out,
                             Market const& item) {
      out << '\n' << "bid:" << item.bid;
      out << '\n' << "ask:" << item.ask;
      return out;
    }

  };


  class Order
  {
  public:
    Order(
      Order_id_t order_id,
      Timestamp_t timestamp,
      Side side,
      Quantity_t quantity,
      Price_t price) :
      order_id_ { order_id },
      timestamp_ { timestamp },
      side_ { side },
      quantity_ { quantity },
      price_ { price } {
    }

    //! getter for order_id_ (access is Ro)
    Order_id_t order_id() const { return order_id_; }

    //! getter for timestamp_ (access is Ro)
    Timestamp_t timestamp() const { return timestamp_; }

    //! getter for side_ (access is Ro)
    Side side() const { return side_; }

    //! getter for quantity_ (access is Ro)
    Quantity_t quantity() const { return quantity_; }

    //! getter for price_ (access is Ro)
    Price_t price() const { return price_; }
    friend inline
    std::ostream& operator<<(std::ostream& out,
                             Order const& item) {
      out << '\n' << "order_id:" << item.order_id_;
      out << '\n' << "timestamp:" << item.timestamp_;
      out << '\n' << "side:" << item.side_;
      out << '\n' << "quantity:" << item.quantity_;
      out << '\n' << "price:" << item.price_;
      return out;
    }

  private:
    Order_id_t const order_id_;
    Timestamp_t const timestamp_;
    Side const side_;
    Quantity_t const quantity_;
    Price_t const price_;

  };


  class Fill
  {
  public:
    Fill(
      Timestamp_t timestamp,
      Order_id_t order,
      Price_t price,
      Quantity_t quantity) :
      timestamp_ { timestamp },
      order_ { order },
      price_ { price },
      quantity_ { quantity } {
    }

    //! getter for timestamp_ (access is Ro)
    Timestamp_t timestamp() const { return timestamp_; }

    //! getter for order_ (access is Ro)
    Order_id_t order() const { return order_; }

    //! getter for price_ (access is Ro)
    Price_t price() const { return price_; }

    //! getter for quantity_ (access is Ro)
    Quantity_t quantity() const { return quantity_; }
    friend inline
    std::ostream& operator<<(std::ostream& out,
                             Fill const& item) {
      out << '\n' << "timestamp:" << item.timestamp_;
      out << '\n' << "order:" << item.order_;
      out << '\n' << "price:" << item.price_;
      out << '\n' << "quantity:" << item.quantity_;
      return out;
    }

  private:
    Timestamp_t const timestamp_;
    Order_id_t const order_;
    Price_t const price_;
    Quantity_t const quantity_;

  };


} // namespace exch
#endif // __EXCH_ORDER_BOOK_HPP__
