#include "exch/exchange.hpp"
#include <boost/test/included/unit_test.hpp>

namespace exch {
void test_exchange() {
  // custom <exchange>
  Order o{1, fcs::timestamp::current_time(), Bid_side_e, 200, 300};

  Fill f{123, fcs::timestamp::current_time(), 1, Ask_side_e, 2321, 4123};

  std::cout << f.serialize_to_dsv() << std::endl;
  f.serialize_from_dsv(f.serialize_to_dsv());
  std::cout << f << std::endl;

  std::cout << Market_id_strlen << std::endl;

  std::cout << std::numeric_limits<Market_id_t>::max() << std::endl;

  BOOST_REQUIRE(3 == 3);

  // end <exchange>
}

}  // namespace exch

boost::unit_test::test_suite* init_unit_test_suite(int, char * []) {
  using namespace exch;
  using namespace boost::unit_test;
  test_suite* test = BOOST_TEST_SUITE("<exchange>");
  test->add(BOOST_TEST_CASE(&test_exchange));
  return test;
}
