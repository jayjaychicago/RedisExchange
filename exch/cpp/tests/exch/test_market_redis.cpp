#include "exch/market_redis.hpp"
#include <boost/test/included/unit_test.hpp>
#include <iostream>

namespace exch {
  void test_market_redis() {
    // custom <market_redis>
    Order o {
      1, fcs::timestamp::current_time(), Bid_side_e, 200, 300
    };

    std::cout << o.to_tuple() << std::endl;
    std::cout << o.from_tuple(o.to_tuple()) << std::endl;

    Fill f {
      123, fcs::timestamp::current_time(), 1, Ask_side_e, 2321, 4123
    };

    std::cout << f.to_tuple() << std::endl;
    std::cout << f.from_tuple(f.to_tuple()) << std::endl;


    // end <market_redis>
  }

} // namespace exch

boost::unit_test::test_suite* init_unit_test_suite(int , char*[]) {
  using namespace exch;
  using namespace boost::unit_test;
  test_suite* test= BOOST_TEST_SUITE( "<market_redis>" );
  test->add( BOOST_TEST_CASE( &test_market_redis ) );
  return test;
}
