set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
cmake_minimum_required( VERSION 2.8 FATAL_ERROR )
project(yocto-meteo)

if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    set ( CMAKE_CXX_FLAGS_DEBUG "-O0 -g3 -pg -p -Wall -march=armv6 -marm -mfloat-abi=hard")
    set ( CMAKE_CXX_FLAGS_RELEASE  "-O3 " )
elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
    set ( CMAKE_CXX_FLAGS_DEBUG "-O0 -g3 -pg -p -Wall -march=armv6 -marm -mfloat-abi=hard ")    
    set ( CMAKE_CXX_FLAGS_RELEASE  "-O3 " )
endif()    

SET( CMAKE_BUILD_TYPE "${CMAKE_BUILD_TYPE}" CACHE STRING "Choose the type of build, options are: None Debug Release ." FORCE)

find_package(Glog)
include_directories(${Glog_INCLUDE_DIRS})
set(LIBS ${LIBS} ${Glog_LIBRARIES} -lglog)

find_package(libusb-1.0)
set(LIBS ${LIBS} ${LIBUSB_1_LIBRARIES})

add_subdirectory(src)
#add_subdirectory(test)
add_subdirectory(yocto)
enable_testing()


