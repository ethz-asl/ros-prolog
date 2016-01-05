cmake_minimum_required(VERSION 2.8.3)
project(prolog_server)

find_package(
  catkin
  REQUIRED
    prolog_msgs
    roscpp
    roscpp_nodewrap
)

catkin_package(
  DEPENDS
    prolog_msgs
    roscpp
    roscpp_nodewrap
)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

add_library(
  prolog_server
    src/ServerNode.cpp
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
    ${catkin_LIBRARIES}
)
