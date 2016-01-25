cmake_minimum_required(VERSION 2.8.3)
project(prolog_server)

set(CMAKE_CXX_FLAGS -std=c++11)

find_package(
  catkin
  REQUIRED
    nodelet
    prolog_common
    prolog_msgs
    prolog_serialization
    prolog_swi
    roscpp
    roscpp_nodewrap
)

find_package(PkgConfig)
pkg_check_modules(SWIPL REQUIRED swipl)

find_program(SWIPL_EXECUTABLE swipl)
add_definitions(-DSWIPL_EXECUTABLE="\\"${SWIPL_EXECUTABLE}\\"")

catkin_package(
  INCLUDE_DIRS
    include
  LIBRARIES
    prolog_server
  DEPENDS
    nodelet
    prolog_common
    prolog_msgs
    prolog_serialization
    prolog_swi
    roscpp
    roscpp_nodewrap
)

include_directories(
  include
  ${SWIPL_INCLUDE_DIRS}
  ${catkin_INCLUDE_DIRS}
)

add_library(
  prolog_server
    src/MultiThreadedServer.cpp
    src/Server.cpp
    src/ServiceServer.cpp
    src/ThreadedQuery.cpp
)

target_link_libraries(
  prolog_server
    ${catkin_LIBRARIES}
)

add_executable(
  mt_server_node
    src/mt_server_node.cpp
)

target_link_libraries(
  mt_server_node
    prolog_server
    ${SWIPL_LIBRARIES}
    ${catkin_LIBRARIES}
)
