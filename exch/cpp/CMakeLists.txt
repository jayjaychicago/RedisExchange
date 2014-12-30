cmake_minimum_required (VERSION 3.0)

include(CheckCXXCompilerFlag)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

######################################################################
# Find boost and include desired components
######################################################################
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.56.0 COMPONENTS program_options system thread
# custom <boost lib components>
# end <boost lib components>
)

# custom <misc section>
# end <misc section>


######################################################################
# Add additional link directories
######################################################################
link_directories(
# custom <link directories>
  ~/install/lib
# end <link directories>

)

enable_testing()

######################################################################
# Add additional include directories
######################################################################
include_directories(
  ${CMAKE_CURRENT_LIST_DIR}
  ${Boost_INCLUDE_DIRS}
# custom <include directories>

  ~/dev/open_source/cereal/include
  ~/dev/open_source/fcs/cpp
  ~/dev/open_source/redisclient/src
  ~/install/include

# end <include directories>

)

######################################################################
# Application build directives
######################################################################
add_executable(exch_server
  app/exch_server/exch_server.cpp
)

target_link_libraries(exch_server
# custom <exch_server libs>
  format
# end <exch_server libs>
  ${Boost_PROGRAM_OPTIONS_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
  ${Boost_THREAD_LIBRARY}
)


######################################################################
# Test directives
######################################################################
# test for test_exchange
add_executable(test_exchange
  tests/exch/test_exchange.cpp
)

target_link_libraries(test_exchange
# custom <test_exchange link requirements>
  format
# end <test_exchange link requirements>
  ${Boost_SYSTEM_LIBRARY}
  ${Boost_THREAD_LIBRARY}
)

add_test(test_exchange test_exchange)

# test for test_redis_support
add_executable(test_redis_support
  tests/exch/test_redis_support.cpp
)

target_link_libraries(test_redis_support
# custom <test_redis_support link requirements>
# end <test_redis_support link requirements>
  ${Boost_SYSTEM_LIBRARY}
  ${Boost_THREAD_LIBRARY}
)

add_test(test_redis_support test_redis_support)



