set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_C_COMPILER "arm-linux-gnueabihf-gcc")
set(CMAKE_CXX_COMPILER "arm-linux-gnueabihf-g++")

set(CMAKE_CXX_FLAGS "-O3 -march=armv7-a -mfloat-abi=hard -mfpu=neon-vfpv4 ${CMAKE_CXX_FLAGS}")
set(CMAKE_C_FLAGS "-O3 -march=armv7-a -mfloat-abi=hard -mfpu=neon-vfpv4 ${CMAKE_C_FLAGS}" )

set(PADDLE_LITE_DIR "${THIRD_PARTY_DIR}/paddile_lite_demo/src/Paddle-Lite-Demo/PaddleLite-armlinux-demo/Paddle-Lite")
message(STATUS "PADDLE LITE DIR: ${PADDLE_LITE_DIR}")

include_directories(${PADDLE_LITE_DIR}/include)
link_directories(${PADDLE_LITE_DIR}/libs/armv7hf)

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
find_package(OpenMP REQUIRED)
if(OpenMP_FOUND OR OpenMP_CXX_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    message(STATUS "Found OpenMP ${OpenMP_VERSION} ${OpenMP_CXX_VERSION}")
    message(STATUS "OpenMP C flags:  ${OpenMP_C_FLAGS}")
    message(STATUS "OpenMP CXX flags:  ${OpenMP_CXX_FLAGS}")
    message(STATUS "OpenMP OpenMP_CXX_LIB_NAMES:  ${OpenMP_CXX_LIB_NAMES}")
    message(STATUS "OpenMP OpenMP_CXX_LIBRARIES:  ${OpenMP_CXX_LIBRARIES}")
else()
    message(FATAL_ERROR "Could not found OpenMP!")
    return()
endif()

add_executable(watchdog "${WATCH_DOG_DIR}/main.cc"
                    "${WATCH_DOG_DIR}/object_detector.cc"
                    "${WATCH_DOG_DIR}/watch_dog.cc"
                    "${WATCH_DOG_DIR}/motor.cc"
)
target_link_libraries(watchdog pthread paddle_light_api_shared ${OpenCV_LIBS})

#aux_source_directory(watchdog sourceList)
#add_executable(main ${sourceList})

#################################
# shared library settings
#################################
#set(LIBXXX_LD_FLAGS "-fPIE -fPIC         ${LIBXXX_LD_FLAGS}")
#set(LIBXXX_LD_FLAGS "-D_FORTIFY_SOURCE=2 ${LIBXXX_LD_FLAGS}")
#set(LIBXXX_LD_FLAGS "-Wformat -Wformat-security ${LIBXXX_LD_FLAGS}")
#set(LIBXXX_LD_FLAGS "-Wl,-z,relro ${LIBXXX_LD_FLAGS}")
#set(LIBXXX_LD_FLAGS "-Wl,-z,now ${LIBXXX_LD_FLAGS}")
#set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${LIBCAMHAL_LD_FLAGS}")

#################################
# Link settings
#################################
#target_link_libraries(xx ${CMAKE_DL_LIBS})
#target_link_libraries(xx_static ${CMAKE_DL_LIBS})

#target_link_libraries(xx expat rt)
#target_link_libraries(xx_static expat rt)

#################################
# Test settings
#################################
if (BUILD_CAMHAL_TESTS)
     add_subdirectory("${SRC_ROOT_DIR}/tests")
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

#-rpath
#-Wall
#clang-format
#doxygen
# build target 
# build test 
# find gtest