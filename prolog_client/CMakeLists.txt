cmake_minimum_required(VERSION 2.8.3)
project(prolog_client)

set(CMAKE_CXX_FLAGS -std=c++11)

find_package(
  catkin
  REQUIRED
    prolog_common
    prolog_msgs
    prolog_serialization
    roscpp
    roscpp_nodewrap
)

catkin_package(
  INCLUDE_DIRS
    include
  LIBRARIES
    prolog_client
  DEPENDS
    prolog_common
    prolog_msgs
    prolog_serialization
    roscpp
    roscpp_nodewrap
)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

add_library(
  prolog_client
    src/Client.cpp
    src/InteractiveClient.cpp
    src/Query.cpp
    src/QueryProxy.cpp
    src/ServiceClient.cpp
)

target_link_libraries(
  prolog_client
    ${catkin_LIBRARIES}
)

add_executable(
  interactive_client_node
    src/interactive_client_node.cpp
)

target_link_libraries(
  interactive_client_node
    prolog_client
    ${catkin_LIBRARIES}
)
