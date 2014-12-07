#include "exch/market_mem.hpp"
#include <boost/test/included/unit_test.hpp>

namespace exch {
  void test_market_exchange_mem() {
    // custom <market_exchange_mem>
    // end <market_exchange_mem>
  }

} // namespace exch

boost::unit_test::test_suite* init_unit_test_suite(int , char*[]) {
  using namespace exch;
  using namespace boost::unit_test;
  test_suite* test= BOOST_TEST_SUITE( "<market_mem>" );
  test->add( BOOST_TEST_CASE( &test_market_exchange_mem ) );
  return test;
}
