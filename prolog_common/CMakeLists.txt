cmake_minimum_required(VERSION 2.8.3)
project(prolog_common)

set(CMAKE_CXX_FLAGS -std=c++11)

find_package(
  catkin
  REQUIRED
    roscpp
)

catkin_package(
  INCLUDE_DIRS
    include
  LIBRARIES
    prolog_common
  DEPENDS
    roscpp
)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

add_library(
  prolog_common
    src/Atom.cpp
    src/Bindings.cpp
    src/Clause.cpp
    src/Compound.cpp
    src/Fact.cpp
    src/Float.cpp
    src/Integer.cpp
    src/List.cpp
    src/Number.cpp
    src/Program.cpp
    src/Query.cpp
    src/Rule.cpp
    src/Solution.cpp
    src/Term.cpp
    src/Variable.cpp
)

target_link_libraries(
  prolog_common
    ${catkin_LIBRARIES}
)
