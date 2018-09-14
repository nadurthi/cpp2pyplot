#-------------------------------------------------------------------------------------------
# Copyright (C) Electronic Arts Inc.  All rights reserved.
#-------------------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.1)
project(cpputility CXX)

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
option(cpputility_BUILD_BENCHMARK "Enable generation of build files for benchmark" ON)
option(cpputility_BUILD_TESTS "Enable generation of build files for tests" ON)
option(cpputility_BUILD_EXAMPLES "Enable generation of build files for examples" ON)

#-------------------------------------------------------
# header and cpputility_SOURCES
#-------------------------------------------------------
set(HEADER_BASE "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(HEADER_BASE_EXT "${CMAKE_CURRENT_SOURCE_DIR}/ext/include")


#---------------------------------------------------------------------------------------
# essential libraries and variables
#---------------------------------------------------------------------------------------
find_package (Threads)

find_package (Boost)
# 
# find_library(UQ
#     NAMES UQ libUQ
# )
add_library(spdlog INTERFACE)
target_include_directories(
    spdlog
    INTERFACE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/ext/spdlog_logger/include>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)


add_library(json INTERFACE)
target_include_directories(
    json
    INTERFACE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/ext/json_cpp/include>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

add_library(eigen INTERFACE)
target_include_directories(
    eigen
    INTERFACE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/ext/eigen/include>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

add_library(catch INTERFACE)
target_include_directories(
    catch
    INTERFACE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/ext/catch2/include>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)


#-------------------------------------------------------------------------------------------
# Sub-projects
#-------------------------------------------------------------------------------------------
# add_subdirectory(test/packages/UQBase)

# if(UQ_BUILD_TESTS OR UQ_BUILD_BENCHMARK)
#     add_subdirectory(test/packages/UQAssert)
# endif()

if(cpputility_BUILD_BENCHMARK)
    add_subdirectory(benchmark)
endif()

if(cpputility_BUILD_TESTS)
    add_subdirectory(test)
endif()

if(cpputility_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

#-------------------------------------------------------------------------------------------
# Defines
#-------------------------------------------------------------------------------------------
# add_definitions(-D_RELEASE)

#-------------------------------------------------------------------------------------------
# Library definition
#-------------------------------------------------------------------------------------------
file(GLOB cpputility_SOURCES "src/*.cpp" "include/*.h")
add_library(cpputility ${cpputility_SOURCES})
target_include_directories(cpputility PUBLIC ${HEADER_BASE} ${Boost_INCLUDE_DIRS})
# include_directories(cpputility ${HEADER_BASE})
target_link_libraries(cpputility ${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES})
set_target_properties(cpputility PROPERTIES LINKER_LANGUAGE CXX)
# target_link_libraries(cpputility ${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES})


#-------------------------------------------------------------------------------------------
# Include dirs
#-------------------------------------------------------------------------------------------
# target_include_directories(cpputility PUBLIC include UQ)

#-------------------------------------------------------------------------------------------
# Installation
#-------------------------------------------------------------------------------------------
install(TARGETS cpputility DESTINATION lib)
install(DIRECTORY include/cpputility DESTINATION include)

