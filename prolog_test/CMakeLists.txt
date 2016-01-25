cmake_minimum_required(VERSION 2.8.3)
project(prolog_test)

set(CMAKE_CXX_FLAGS -std=c++11)

find_package(
  catkin
  REQUIRED
    prolog_common
    prolog_serialization
    prolog_swi
    roscpp
)

catkin_package(
  INCLUDE_DIRS
    include
  LIBRARIES
    prolog_test
  DEPENDS
    prolog_common
    prolog_serialization
    prolog_swi
    roscpp
)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

add_library(
  prolog_test
    src/CurrentAtomQuery.cpp
    src/FileSearchPathQuery.cpp
)

target_link_libraries(
  prolog_test
    ${catkin_LIBRARIES}
)

catkin_add_gtest(
  prolog_tests
    test/prolog_tests.cpp
    test/ContextTest.cpp
    test/EngineTest.cpp
    test/ExceptionTest.cpp
    test/FrameTest.cpp
    test/QueryTest.cpp
    test/SerializationTest.cpp
    test/SolutionTest.cpp
)

target_link_libraries(
  prolog_tests
    prolog_test
    ${catkin_LIBRARIES}
)
