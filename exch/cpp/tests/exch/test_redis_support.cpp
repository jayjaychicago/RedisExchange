#include "exch/redis_support.hpp"
#include <boost/test/included/unit_test.hpp>

namespace exch {
void test_redis_listener() {
  // custom <redis_listener>

  Create_market_req req{
      1,                              2,
      "some-market",                  fcs::timestamp::current_time(),
      fcs::timestamp::current_time(), 2,
      5};

  std::ostringstream out;
  req.serialize_to_json(out);
  std::istringstream in{out.str()};

  Create_market_req req2;
  req2.serialize_from_json(in);

  BOOST_REQUIRE(req2 == req);

  // end <redis_listener>
}

}  // namespace exch

boost::unit_test::test_suite* init_unit_test_suite(int, char * []) {
  using namespace exch;
  using namespace boost::unit_test;
  test_suite* test = BOOST_TEST_SUITE("<redis_support>");
  test->add(BOOST_TEST_CASE(&test_redis_listener));
  return test;
}
