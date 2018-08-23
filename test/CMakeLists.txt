
#
# Locate example sources
#
cmake_minimum_required(VERSION 3.1)
project(Testing)

find_package(Threads)

FILE (GLOB_RECURSE test_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
message("-------------- testing -------------")
message(${CMAKE_CURRENT_SOURCE_DIR})
message(${test_SRCS})
message("======================================")

FOREACH (T_FILE ${test_SRCS})
  GET_FILENAME_COMPONENT (T_NAME ${T_FILE} NAME_WE)
  ADD_EXECUTABLE (${T_NAME} ${T_FILE})
  TARGET_LINK_LIBRARIES(${T_NAME} myproject ${CMAKE_THREAD_LIBS_INIT})
  set(EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_LIST_DIR}")
ENDFOREACH (T_FILE ${test_SRCS})
