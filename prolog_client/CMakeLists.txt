cmake_minimum_required(VERSION 2.8.3)
project(prolog_client)

find_package(
  catkin
  REQUIRED
    prolog
    prolog_msgs
    roscpp
    roscpp_nodewrap
)

catkin_package(
  DEPENDS
    prolog
    prolog_msgs
    roscpp
    roscpp_nodewrap
)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

add_library(
  prolog_client
    src/ClientNode.cpp
)

target_link_libraries(
  prolog_client
    ${catkin_LIBRARIES}
)
