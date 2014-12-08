#ifndef __EXCH_EXCH_HPP__
#define __EXCH_EXCH_HPP__

#include "fcs/timestamp/timestamp.hpp"
#include <cstdint>
#include <utility>
#include <vector>

namespace exch {
  class Order;
  using Market_id_t = int32_t;
  using User_id_t = int64_t;
  using Price_t = int32_t;
  using Quantity_t = int32_t;
  using Order_id_t = int64_t;
  using Order_id_list_t = std::vector< Order_id_t >;
  using Order_list_t = std::vector< Order >;
  using Timestamp_t = fcs::timestamp::Timestamp_t;
  using Order_update_t = std::pair< Order_id_t, Order >;
  using Order_update_list_t = std::vector< Order_update_t >;

} // namespace exch
#endif // __EXCH_EXCH_HPP__
