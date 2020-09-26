#################################
# cmake settings
#################################
cmake_minimum_required(VERSION 3.10)
set(CMAKE_BUILD_TYPE "Debug")   # or "Release"
set(CMAKE_VERBOSE_MAKEFILE OFF) # or ON

#################################
# set up project infomation
#################################
project(watchdog VERSION 0.1 DESCRIPTION "a simple and practical watchdog" LANGUAGES C CXX)

#################################
# set up build options
#################################
option(BUILD_WATCHDOG "Build watchdog or not" OFF)
option(BUILD_WATCHDOG_TESTS "Build test cases or not" OFF)
option(BUILD_THIRD_PARTY "Build third party or not" OFF)
option(WITH_XXX "Option to build with freaure XXX" OFF)
#option(FOR_CI "" OFF)
#set(THIRD_PARTY_MIRROR "" CACHE STRING "")

### macro WITH_XXX in source code can be used as option for some feature
if (WITH_XXX)
  add_definitions(-DWITH_XXX)
endif(WITH_XXX)

enable_testing()

# language standard requirements
# C standard can be overridden when this is used as a sub-project.
if(NOT CMAKE_C_STANDARD)
  # This project requires C11
  set(CMAKE_C_STANDARD 11)
  set(CMAKE_C_STANDARD_REQUIRED ON) #set to OFF when code can gracefully decay down to C89
  set(CMAKE_C_EXTENSIONS OFF)
endif(NOT CMAKE_C_STANDARD)
# C++ standard can be overridden when this is used as a sub-project.
if(NOT CMAKE_CXX_STANDARD)
  # This project requires C++11.
  set(CMAKE_CXX_STANDARD 11)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)
endif(NOT CMAKE_CXX_STANDARD)

#################################
# folders settings
#################################
# source directory
set(SRC_ROOT_DIR  	${CMAKE_CURRENT_LIST_DIR})
set(WATCH_DOG_DIR 	${SRC_ROOT_DIR}/watchdog)
set(THIRD_PARTY_DIR "${PROJECT_SOURCE_DIR}/third_party"
  CACHE PATH "Where the third party headers and libs are put")
set(THIRD_PARTY_SUBMODULE_DIR "${PROJECT_SOURCE_DIR}/build/third_party"
  CACHE PATH "Where the third party submodules are")

# CMake module directory
set(WATCHDOG_CMAKE_PATH ${PROJECT_SOURCE_DIR}/cmake)
# Modules
list(APPEND CMAKE_MODULE_PATH ${WATCHDOG_CMAKE_PATH}/third_party)
list(APPEND CMAKE_MODULE_PATH ${WATCHDOG_CMAKE_PATH})
#include(something_common)

# install directory
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
     set(CMAKE_INSTALL_PREFIX "${PROJECT_BINARY_DIR}" CACHE PATH "Target location for install command." FORCE)
endif()
message(STATUS "install prefix: " ${CMAKE_INSTALL_PREFIX})

#################################
# Compilation settings
#################################

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
if(WIN32)
    message(FATAL_ERROR "not tested on WIN32")
else(WIN32)
  set(EXTRA_CXX_FLAGS "-std=c++11    ${EXTRA_CXX_FLAGS}")
  set(EXTRA_CXX_FLAGS "-Wall -Werror ${EXTRA_CXX_FLAGS}")
  set(EXTRA_CXX_FLAGS "-fPIE -fPIC   ${EXTRA_CXX_FLAGS}")
  set(EXTRA_CXX_FLAGS "-fstack-protector    ${EXTRA_CXX_FLAGS}")
  set(EXTRA_CXX_FLAGS "-DDCHECK_ALWAYS_ON   ${EXTRA_CXX_FLAGS}")
  set(EXTRA_CXX_FLAGS "-Wformat -Wformat-security   ${EXTRA_CXX_FLAGS}")
  set(EXTRA_CXX_FLAGS "-Wno-sign-compare            ${EXTRA_CXX_FLAGS}")
  set(EXTRA_CXX_FLAGS "-Wno-unused-function         ${EXTRA_CXX_FLAGS}")

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EXTRA_CXX_FLAGS}")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${EXTRA_CXX_FLAGS}")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${EXTRA_CXX_FLAGS}")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} ${EXTRA_CXX_FLAGS}")
endif(WIN32)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG(-faligned-new COMPILER_SUPPORTS_ALIGNED_NEW)
if (COMPILER_SUPPORTS_ALIGNED_NEW)
     add_definitions(-faligned-new)
endif()

#code coverage
#include(code-coverage)

###############################
#
###############################
#include(third_party)
#include(watchdog)