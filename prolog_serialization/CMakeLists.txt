cmake_minimum_required(VERSION 2.8.3)
project(prolog_serialization)

set(CMAKE_CXX_FLAGS -std=c++11)

find_package(
  catkin
  REQUIRED
    prolog_common
    roscpp
)

find_package(PkgConfig)
pkg_check_modules(JSONCPP REQUIRED jsoncpp)

catkin_package(
  INCLUDE_DIRS
    include
  LIBRARIES
    prolog_serialization
  DEPENDS
    prolog_common
    roscpp
)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
  ${JSONCPP_INCLUDE_DIRS}
)

add_library(
  prolog_serialization
    src/Deserializer.cpp
    src/JSONDeserializer.cpp
    src/JSONSerializer.cpp
    src/PrologSerializer.cpp
    src/Serializer.cpp
)

target_link_libraries(
  prolog_serialization
    ${catkin_LIBRARIES}
    ${JSONCPP_LIBRARIES}
)
