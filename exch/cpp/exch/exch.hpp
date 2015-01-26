#ifndef __EXCH_EXCH_HPP__
#define __EXCH_EXCH_HPP__

#include "fcs/timestamp/timestamp.hpp"
#include <cstdint>
#include <limits>
#include <utility>
#include <vector>

namespace exch {
class Order;
class Fill;
using Market_id_t = int32_t;
using User_id_t = int64_t;
using Price_t = int32_t;
using Quantity_t = int32_t;
using Order_id_t = int64_t;
using Fill_id_t = int64_t;
using Order_id_list_t = std::vector<Order_id_t>;
using Order_list_t = std::vector<Order>;
using Fill_list_t = std::vector<Fill>;
using Timestamp_t = fcs::timestamp::Timestamp_t;
using Order_update_t = std::pair<Order_id_t, Order>;
using Order_update_list_t = std::vector<Order_update_t>;
using Req_id_t = int64_t;

enum Side { Bid_side_e, Ask_side_e };

enum Create_market_result { Create_market_succeeded_e, Create_market_failed_e };

enum Submit_result {
  Submit_succeeded_e,
  Submit_invalid_market_e,
  Submit_invalid_order_details_e
};

enum Cancel_result {
  Cancel_succeeded_e,
  Cancel_invalid_market_e,
  Cancel_invalid_order_e
};

enum Replace_result {
  Replace_succeeded_e,
  Replace_invalid_market_e,
  Replace_invalid_order_e,
  Replace_invalid_order_details_e
};

enum Market_details_result {
  Market_details_succeeded_e,
  Market_details_invalid_market_e
};

enum Log_type { Log_book_e };

// custom <FcbEndNamespace exch>

constexpr size_t Market_id_strlen =
    size_t(8 * sizeof(Market_id_t) / 3.1622776601683795) + 1;

// end <FcbEndNamespace exch>

}  // namespace exch
#endif  // __EXCH_EXCH_HPP__
