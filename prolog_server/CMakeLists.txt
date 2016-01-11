cmake_minimum_required(VERSION 2.8.3)
project(prolog_server)

find_package(
  catkin
  REQUIRED
    prolog_common
    prolog_msgs
    roscpp
    roscpp_nodewrap
)

find_package(PkgConfig)
pkg_check_modules(SWIPL REQUIRED swipl)

catkin_package(
  INCLUDE_DIRS
    include
  LIBRARIES
    prolog_server
  DEPENDS
    prolog_common
    prolog_msgs
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
    src/Server.cpp
    src/ServiceServer.cpp
)

target_link_libraries(
  prolog_server
    ${catkin_LIBRARIES}
)

add_executable(
  server_node
    src/server_node.cpp
)

target_link_libraries(
  server_node
    prolog_server
    ${SWIPL_LIBRARIES}
    ${catkin_LIBRARIES}
)
