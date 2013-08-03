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

#find_package(GooglePerfTools)
#if(GOOGLE_PERFTOOLS_FOUND)
#    include_directories(${GOOGLE_PERFTOOLS_INCLUDE_DIR})
#    set(LIBS ${LIBS} ${TCMALLOC_LIBRARIES} ${STACKTRACE_LIBRARIES} ${PROFILER_LIBRARIES})
#endif(GOOGLE_PERFTOOLS_FOUND)

#find_package(GFlags)
#if(GFlags_FOUND)
#    find_package(Glog)
#    if(Glog_FOUND)
#        include_directories(${GFlags_INCLUDE_DIRS})
#        include_directories(${Glog_INCLUDE_DIRS})
#        link_directories(${GFlags_LIBRARY_DIRS})
#        set(LIBS ${LIBS} ${GFlags_LIBS})
#        set(LIBS ${LIBS} ${Glog_LIBRARIES} -lglog)
#    endif(Glog_FOUND)
#endif(GFlags_FOUND)

find_package(libusb-1.0)
set(LIBS ${LIBS} ${LIBUSB_1_LIBRARIES})

add_subdirectory(src)
#add_subdirectory(test)
add_subdirectory(yocto)
enable_testing()


