cmake_minimum_required(VERSION 2.8.3)
project(prolog_msgs)

find_package(
  catkin
  REQUIRED
    message_generation
    std_msgs
)

add_service_files(
  FILES
    CloseQuery.srv
    GetAllSolutions.srv
    GetNextSolution.srv
    HasSolution.srv
    OpenQuery.srv
)

generate_messages(
  DEPENDENCIES std_msgs
)

catkin_package(
  DEPENDS message_runtime
)
