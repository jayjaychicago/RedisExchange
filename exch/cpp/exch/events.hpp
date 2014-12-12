#ifndef __EXCH_EVENTS_HPP__
#define __EXCH_EVENTS_HPP__

#include "cereal/archives/json.hpp"
#include "cereal/cereal.hpp"
#include "exch/exch.hpp"
#include "fcs/timestamp/cereal.hpp"
#include <iosfwd>

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
    friend inline
    std::ostream& operator<<(std::ostream& out,
                             Market_created_evt const& item) {
      out << '\n' << "market_id:" << item.market_id_;
      return out;
    }

    template <class Archive>
    void serialize(Archive &ar__) {
      ar__(cereal::make_nvp("market_id", market_id_));
    }

    void serialize_to_json(std::ostream & out__) const {
      cereal::JSONOutputArchive ar__(out__);
      const_cast<Market_created_evt*>(this)->serialize(ar__);
    }

    void serialize_from_json(std::istream & in__) {
      cereal::JSONInputArchive ar__ { in__ };
      serialize(ar__);
    }


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
    friend inline
    std::ostream& operator<<(std::ostream& out,
                             Top_of_book_evt const& item) {
      out << '\n' << "market_id:" << item.market_id_;
      return out;
    }

    template <class Archive>
    void serialize(Archive &ar__) {
      ar__(cereal::make_nvp("market_id", market_id_));
    }

    void serialize_to_json(std::ostream & out__) const {
      cereal::JSONOutputArchive ar__(out__);
      const_cast<Top_of_book_evt*>(this)->serialize(ar__);
    }

    void serialize_from_json(std::istream & in__) {
      cereal::JSONInputArchive ar__ { in__ };
      serialize(ar__);
    }


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
    friend inline
    std::ostream& operator<<(std::ostream& out,
                             Book_update_evt const& item) {
      out << '\n' << "market_id:" << item.market_id_;
      out << '\n' << "side:" << item.side_;
      out << '\n' << "price:" << item.price_;
      out << '\n' << "quantity:" << item.quantity_;
      out << '\n' << "top_price:" << item.top_price_;
      return out;
    }

    template <class Archive>
    void serialize(Archive &ar__) {
      ar__(cereal::make_nvp("market_id", market_id_));
      ar__(cereal::make_nvp("side", side_));
      ar__(cereal::make_nvp("price", price_));
      ar__(cereal::make_nvp("quantity", quantity_));
      ar__(cereal::make_nvp("top_price", top_price_));
    }

    void serialize_to_json(std::ostream & out__) const {
      cereal::JSONOutputArchive ar__(out__);
      const_cast<Book_update_evt*>(this)->serialize(ar__);
    }

    void serialize_from_json(std::istream & in__) {
      cereal::JSONInputArchive ar__ { in__ };
      serialize(ar__);
    }


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
    friend inline
    std::ostream& operator<<(std::ostream& out,
                             Trade_evt const& item) {
      out << '\n' << "market_id:" << item.market_id_;
      out << '\n' << "side:" << item.side_;
      out << '\n' << "quantity:" << item.quantity_;
      out << '\n' << "price:" << item.price_;
      out << '\n' << "net_volume:" << item.net_volume_;
      return out;
    }

    template <class Archive>
    void serialize(Archive &ar__) {
      ar__(cereal::make_nvp("market_id", market_id_));
      ar__(cereal::make_nvp("side", side_));
      ar__(cereal::make_nvp("quantity", quantity_));
      ar__(cereal::make_nvp("price", price_));
      ar__(cereal::make_nvp("net_volume", net_volume_));
    }

    void serialize_to_json(std::ostream & out__) const {
      cereal::JSONOutputArchive ar__(out__);
      const_cast<Trade_evt*>(this)->serialize(ar__);
    }

    void serialize_from_json(std::istream & in__) {
      cereal::JSONInputArchive ar__ { in__ };
      serialize(ar__);
    }


  private:
    Market_id_t const market_id_;
    Side const side_;
    Quantity_t const quantity_;
    Price_t const price_;
    Quantity_t const net_volume_;

  };


} // namespace exch
#endif // __EXCH_EVENTS_HPP__
