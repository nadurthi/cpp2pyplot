
#
# Locate example sources
#
cmake_minimum_required(VERSION 3.1)
project(benchmark)

find_package(Threads)
find_package(benchmark)

FILE (GLOB_RECURSE bench_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
message("-------------- benchmark -------------")
message(${CMAKE_CURRENT_SOURCE_DIR})
message(${bench_SRCS})
message("======================================")




FOREACH (T_FILE ${bench_SRCS})
  GET_FILENAME_COMPONENT (T_NAME ${T_FILE} NAME_WE)
  ADD_EXECUTABLE (${T_NAME} ${T_FILE})
  # set(CMAKE_CXX_FLAGS "-O3 -Wall -std=c++1y -lbenchmark ${CMAKE_CXX_FLAGS}")
  TARGET_LINK_LIBRARIES(${T_NAME} benchmark ${CMAKE_THREAD_LIBS_INIT})
  set(EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_LIST_DIR}")
ENDFOREACH (T_FILE ${bench_SRCS})
