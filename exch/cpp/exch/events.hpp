#ifndef __EXCH_EVENTS_HPP__
#define __EXCH_EVENTS_HPP__

#include "exch/exch.hpp"

namespace exch {
  class Market_created_evt
  {
  public:
    Market_created_evt(
      Market_id_t market_id) :
      market_id_ { market_id } {
    }

    //! getter for market_id_ (access is Ro)
    Market_id_t market_id() const { return market_id_; }
  private:
    Market_id_t const market_id_;

  };


  class Top_of_book_evt
  {
  public:
    Top_of_book_evt(
      Market_id_t market_id) :
      market_id_ { market_id } {
    }

    //! getter for market_id_ (access is Ro)
    Market_id_t market_id() const { return market_id_; }
  private:
    Market_id_t const market_id_;

  };


  class Book_update_evt
  {
  public:
    Book_update_evt(
      Market_id_t market_id,
      Side side,
      Price_t price,
      Quantity_t quantity,
      Price_t top_price) :
      market_id_ { market_id },
      side_ { side },
      price_ { price },
      quantity_ { quantity },
      top_price_ { top_price } {
    }

    //! getter for market_id_ (access is Ro)
    Market_id_t market_id() const { return market_id_; }

    //! getter for side_ (access is Ro)
    Side side() const { return side_; }

    //! getter for price_ (access is Ro)
    Price_t price() const { return price_; }

    //! getter for quantity_ (access is Ro)
    Quantity_t quantity() const { return quantity_; }

    //! getter for top_price_ (access is Ro)
    Price_t top_price() const { return top_price_; }
  private:
    Market_id_t const market_id_;
    Side const side_;
    Price_t const price_;
    Quantity_t const quantity_;
    Price_t const top_price_;

  };


  class Trade_evt
  {
  public:
    Trade_evt(
      Market_id_t market_id,
      Side side,
      Quantity_t quantity,
      Price_t price,
      Quantity_t net_volume) :
      market_id_ { market_id },
      side_ { side },
      quantity_ { quantity },
      price_ { price },
      net_volume_ { net_volume } {
    }

    //! getter for market_id_ (access is Ro)
    Market_id_t market_id() const { return market_id_; }

    //! getter for side_ (access is Ro)
    Side side() const { return side_; }

    //! getter for quantity_ (access is Ro)
    Quantity_t quantity() const { return quantity_; }

    //! getter for price_ (access is Ro)
    Price_t price() const { return price_; }

    //! getter for net_volume_ (access is Ro)
    Quantity_t net_volume() const { return net_volume_; }
  private:
    Market_id_t const market_id_;
    Side const side_;
    Quantity_t const quantity_;
    Price_t const price_;
    Quantity_t const net_volume_;

  };


} // namespace exch
#endif // __EXCH_EVENTS_HPP__
