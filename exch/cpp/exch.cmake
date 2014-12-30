cmake_minimum_required (VERSION 3.0)

include(CheckCXXCompilerFlag)

LINK_DIRECTORIES(~/install/lib)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(
  ../..
  ~/dev/open_source/cereal/include
  ~/dev/open_source/fcs/cpp
  ~/dev/open_source/redisclient/src
  ~/install/include
)
