cmake_minimum_required (VERSION 3.0)
project (exch_server)
include(../../exch.cmake)

add_executable(exch_server exch_server.cpp)

TARGET_LINK_LIBRARIES(exch_server
  boost_system-mt
  boost_thread-mt
  boost_program_options-mt
  format)
