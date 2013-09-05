set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
cmake_minimum_required( VERSION 2.8 FATAL_ERROR )
project(yocto-meteo)

if(NOT DEFINED SKIP_PUB)
    set(SKIP_PUB false)
else()
    set(SKIP_PUB true)
endif()    

if(NOT DEFINED SKIP_SUB)
    set(SKIP_SUB false)
else()
    set(SKIP_SUB true)
endif()    

#set default compile flags regardless of arch
set ( CMAKE_CXX_FLAGS "-O2 -Wall -std=c++11")
set ( CMAKE_C_FLAGS "-O2 -Wall ")
set ( CMAKE_CXX_FLAGS_RELEASE  "-O3 -std=c++11" )
set ( CMAKE_C_FLAGS_RELEASE  "-O3" )

#set compiler specific debug flags
if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    set ( CMAKE_CXX_FLAGS_DEBUG "-O0 -g -debug -Wall -std=c++11")
    set (CMAKE_C_FLAGS_DEBUG "-O0 -g -debug -Wall")
elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
    set ( CMAKE_CXX_FLAGS_DEBUG "-O0 -g3 -pg -p -Wall -std=c++11")    
    set ( CMAKE_C_FLAGS_DEBUG "-O0 -g3 -pg -p -Wall ")    
endif()    

#set arm specific build options
if ("${CMAKE_SYSTEM_PROCESSOR}" MATCHES "armv6l")
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-march=armv6 -marm -mfloat-abi=hard")
    set(CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE} "-march=armv6 -marm -mfloat-abi=hard")
    set(CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG} "-march=armv6 -marm -mfloat-abi=hard")
    set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-march=armv6 -marm -mfloat-abi=hard")
    set(CMAKE_C_FLAGS_RELEASE ${CMAKE_C_FLAGS_RELEASE} "-march=armv6 -marm -mfloat-abi=hard")
    set(CMAKE_C_FLAGS_DEBUG ${CMAKE_C_FLAGS_DEBUG} "-march=armv6 -marm -mfloat-abi=hard")
    ADD_DEFINITIONS(-DARMV6_BUILD)
    set(LIBS ${LIBS} "-lm -lstdc++")
endif()    
#nothing specific to set on x86

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
if(LIBUSB_1_FOUND)
    include_directories(${LIBUSB_1_INCLUDE_DIRS})
    set(LIBS ${LIBS} ${LIBUSB_1_LIBRARIES})
else()
    message("Did not find libusb skipping publisher build")
    set(SKIP_PUB true)    
endif(LIBUSB_1_FOUND)    

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


