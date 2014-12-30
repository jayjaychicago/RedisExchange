cmake_minimum_required (VERSION 3.0)

include(CheckCXXCompilerFlag)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
# custom <definitions>

# end <definitions>

link_directories(~/install/lib)
# custom <link paths>
# end <link paths>


enable_testing()

include_directories(
  ${CMAKE_CURRENT_LIST_DIR}
  ~/dev/open_source/cereal/include
  ~/dev/open_source/fcs/cpp
  ~/dev/open_source/redisclient/src
  ~/install/include
# custom <additional includes>
# end <additional includes>

)

add_executable(exch_server
  app/exch_server/exch_server.cpp
)

target_link_libraries(exch_server
# custom <exch_server libs>
  boost_program_options-mt
  format
  boost_system-mt
  boost_thread-mt
# end <exch_server libs>
)


# test for test_exchange
add_executable(test_exchange
  tests/exch/test_exchange.cpp)
target_link_libraries(test_exchange
  format
  boost_thread-mt
  boost_system-mt
  )


add_test(test_exchange test_exchange)

# test for test_redis_support
add_executable(test_redis_support
  tests/exch/test_redis_support.cpp)

target_link_libraries(test_redis_support
  boost_system-mt
  )

add_test(test_redis_support test_redis_support)
