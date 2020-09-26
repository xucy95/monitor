# shared library settings
#set(LIBXXX_LD_FLAGS "-fPIE -fPIC         ${LIBXXX_LD_FLAGS}")
#set(LIBXXX_LD_FLAGS "-D_FORTIFY_SOURCE=2 ${LIBXXX_LD_FLAGS}")
#set(LIBXXX_LD_FLAGS "-Wformat -Wformat-security ${LIBXXX_LD_FLAGS}")
#set(LIBXXX_LD_FLAGS "-Wl,-z,relro ${LIBXXX_LD_FLAGS}")
#set(LIBXXX_LD_FLAGS "-Wl,-z,now ${LIBXXX_LD_FLAGS}")
#set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${LIBCAMHAL_LD_FLAGS}")

#################################
# Link settings
#################################
target_link_libraries(xx ${CMAKE_DL_LIBS})
target_link_libraries(xx_static ${CMAKE_DL_LIBS})

target_link_libraries(xx expat rt)
target_link_libraries(xx_static expat rt)

#################################
# Test settings
#################################
if (BUILD_CAMHAL_TESTS)
     add_subdirectory(test/linux)
 endif()


#################################
# install settings
#################################

#install header files
#install configure files
#install libraries
#install package config files


#################################
# pack settings
#################################
set(CPACK_GENERATOR "DEB")
include(CPack)


set(CMAKE_SYSTEM_NAME Linux)

set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_C_COMPILER "arm-linux-gnueabihf-gcc")
set(CMAKE_CXX_COMPILER "arm-linux-gnueabihf-g++")

find_package(OpenCV REQUIRED)
if(OpenCV_FOUND OR OpenCV_CXX_FOUND)
    include_directories(${OpenCV_INCLUDE_DIRS})
    message(STATUS "OpenCV library status:")
    message(STATUS "    version: ${OpenCV_VERSION}")
    message(STATUS "    libraries: ${OpenCV_LIBS}")
    message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")
else()
    message(FATAL_ERROR "Could not found OpenCV!")
    return()
endif()

aux_source_directory(watchdog sourceList)
add_executable(main ${sourceList})
target_link_libraries(main ${OpenCV_LIBS})

#-rpath
#-Wall
#clang-format
#doxygen
# build target 
# build test 
# find gtest