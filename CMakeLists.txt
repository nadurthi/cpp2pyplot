#-------------------------------------------------------------------------------------------
# Copyright (C) Electronic Arts Inc.  All rights reserved.
#-------------------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.1)
project(myproject)

#---------------------------------------------------------------------------------------
# compiler config
#---------------------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "-O3 -Wall -std=c++1y ${CMAKE_CXX_FLAGS}")
endif()


#-------------------------------------------------------------------------------------------
# Options
#-------------------------------------------------------------------------------------------
option(myproject_BUILD_BENCHMARK "Enable generation of build files for benchmark" ON)
option(myproject_BUILD_TESTS "Enable generation of build files for tests" ON)
option(myproject_BUILD_EXAMPLES "Enable generation of build files for examples" ON)

#-------------------------------------------------------
# header and myproject_SOURCES
#-------------------------------------------------------
set(HEADER_BASE "${CMAKE_CURRENT_SOURCE_DIR}/include")

#---------------------------------------------------------------------------------------
# essential libraries and variables
#---------------------------------------------------------------------------------------
find_package (Threads)

find_package (Boost)
# 
find_library(UQ
    NAMES UQ libUQ
)

# find_package(Eigen3 REQUIRED)
# find_package(spdlog REQUIRED)
# 
#-------------------------------------------------------------------------------------------
# Sub-projects
#-------------------------------------------------------------------------------------------
# add_subdirectory(test/packages/UQBase)

# if(UQ_BUILD_TESTS OR UQ_BUILD_BENCHMARK)
#     add_subdirectory(test/packages/UQAssert)
# endif()

if(myproject_BUILD_BENCHMARK)
    add_subdirectory(benchmark)
endif()

if(myproject_BUILD_TESTS)
    add_subdirectory(test)
endif()

#-------------------------------------------------------------------------------------------
# Defines
#-------------------------------------------------------------------------------------------
# add_definitions(-D_RELEASE)

#-------------------------------------------------------------------------------------------
# Library definition
#-------------------------------------------------------------------------------------------
file(GLOB myproject_SOURCES "src/*.cpp" "include/myproject/*.h")
add_library(myproject ${myproject_SOURCES})
include_directories("include")
target_link_libraries(myproject ${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES})
# target_link_libraries(myproject ${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES})


#-------------------------------------------------------------------------------------------
# Include dirs
#-------------------------------------------------------------------------------------------
target_include_directories(myproject PUBLIC include UQ)

#-------------------------------------------------------------------------------------------
# Installation
#-------------------------------------------------------------------------------------------
install(TARGETS myproject DESTINATION lib)
install(DIRECTORY include/myproject DESTINATION include)

