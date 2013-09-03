set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
cmake_minimum_required( VERSION 2.8 FATAL_ERROR )
project(yocto-meteo)

if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    set ( CMAKE_CXX_FLAGS "-O0 -g3 -Wall -std=c++11")
    #set ( CMAKE_CXX_FLAGS_DEBUG "-O0 -g3 -mllvm -debug -Wall -march=armv6 -marm -mfloat-abi=hard -std=c++11")
    #set ( CMAKE_CXX_FLAGS_RELEASE  "-O3 " )
elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
    set ( CMAKE_CXX_FLAGS "-O3 -Wall -march=armv6 -marm -mfloat-abi=hard -std=c++11"
    set ( CMAKE_C_FLAGS "-O3 -Wall -march=armv6 -marm -mfloat-abi=hard ")
    #set ( CMAKE_CXX_FLAGS_DEBUG "-O0 -g3 -pg -p -Wall -march=armv6 -marm -mfloat-abi=hard -std=c++11")    
    #set ( CMAKE_CXX_FLAGS_RELEASE  "-O3 " )
endif()    

SET( CMAKE_BUILD_TYPE "${CMAKE_BUILD_TYPE}" CACHE STRING "Choose the type of build, options are: None Debug Release ." FORCE)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
ADD_DEFINITIONS(-DBOOST_ALL_DYN_LINK)
find_package(Boost 1.54.0 COMPONENTS log log_setup system filesystem program_options thread)
include_directories(${Boost_INCLUDE_DIRS})
set(LIBS ${LIBS} ${Boost_LIBRARIES})

find_package(GooglePerfTools)
if(GOOGLE_PERFTOOLS_FOUND)
    include_directories(${GOOGLE_PERFTOOLS_INCLUDE_DIR})
    set(LIBS ${LIBS} ${TCMALLOC_LIBRARIES} ${STACKTRACE_LIBRARIES} ${PROFILER_LIBRARIES})
endif(GOOGLE_PERFTOOLS_FOUND)

find_package(libusb-1.0)
set(LIBS ${LIBS} ${LIBUSB_1_LIBRARIES})

find_package(MongoDB)
if(MongoDB_FOUND)
    include_directories(${MongoDB_INCLUDE_DIR})
    set(LIBS ${LIBS} ${MongoDB_LIBRARIES})
    ADD_DEFINITIONS(-DMONGO_AVAIL)
endif(MongoDB_FOUND)    

find_package(ZMQ)
include_directories(${ZMQ_INCLUDE_DIRS})
set(LIBS ${LIBS} ${ZMQ_LIBRARIES})

find_package(Protobuf)
if(PROTOBUF_FOUND)
    include_directories(${PROTOBUF_INCLUDE_DIRS})
    set(LIBS ${LIBS} ${PROTOBUF_LIBRARIES})
    ADD_DEFINITIONS(-DPROTOBUF_AVAIL)
endif(PROTOBUF_FOUND)    

add_subdirectory(src)
#add_subdirectory(test)
add_subdirectory(yoctolib_cpp/Sources)
enable_testing()


