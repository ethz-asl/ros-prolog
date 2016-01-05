cmake_minimum_required(VERSION 2.8.3)
project(prolog)

find_package(
  catkin
  REQUIRED
    roscpp
)

find_package(PkgConfig)
pkg_check_modules(JSON_GLIB REQUIRED json-glib-1.0)

catkin_package(
  DEPENDS
    roscpp
)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
  ${JSON_GLIB_INCLUDE_DIRS}
)

add_library(
  prolog
    src/Bindings.cpp
    src/Exceptions.cpp
    src/Parser.cpp
    src/Term.cpp
    src/Value.cpp
)

target_link_libraries(
  prolog
    ${catkin_LIBRARIES}
    ${JSON_GLIB_LIBRARIES}
)
