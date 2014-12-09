#include "exch/market_redis.hpp"
#include <boost/test/included/unit_test.hpp>

namespace exch {
  void test_market_redis() {
    // custom <market_redis>
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
