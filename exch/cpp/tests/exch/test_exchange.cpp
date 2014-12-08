#include "exch/exchange.hpp"
#include <boost/test/included/unit_test.hpp>

namespace exch {
  void test_exchange() {
    // custom <exchange>
    // end <exchange>
  }

} // namespace exch

boost::unit_test::test_suite* init_unit_test_suite(int , char*[]) {
  using namespace exch;
  using namespace boost::unit_test;
  test_suite* test= BOOST_TEST_SUITE( "<exchange>" );
  test->add( BOOST_TEST_CASE( &test_exchange ) );
  return test;
}
