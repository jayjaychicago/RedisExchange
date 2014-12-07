#ifndef __EXCH_MARKET_MEM_HPP__
#define __EXCH_MARKET_MEM_HPP__

#include "exch/exch.hpp"
#include "exch/market.hpp"
#include "exch/publish.hpp"

namespace exch {
  class Market_exchange_mem :
    public Market_exchange
  {
  public:
    Market_exchange_mem(
      Market_publisher & market_publisher) :
      market_publisher_ { market_publisher } {
    }

    // custom <ClsPublic Market_exchange_mem>
    // end <ClsPublic Market_exchange_mem>

  private:
    Market_publisher & market_publisher_;
    Managed_order_list_t active_orders_ {};
    Managed_order_list_t dead_orders_ {};
    Quantity_t net_volume_ {};

  };


} // namespace exch
#endif // __EXCH_MARKET_MEM_HPP__
