cmake_minimum_required( VERSION 2.8 FATAL_ERROR )

include_directories (common)
include_directories (message)
include_directories (publisher)
include_directories (subscriber)
include_directories(../yoctolib_cpp/Sources)

add_subdirectory(message)
add_subdirectory(common)
add_subdirectory(publisher)
add_subdirectory(subscriber)

link_libraries(${LIBS} -lpthread )

set(EXECUTABLE_OUTPUT_PATH "../")
if(NOT SKIP_PUB)
    add_executable(pub_meteo pmain.cpp)
    target_link_libraries(pub_meteo yocto ${LIBUSB_1_LIBRARIES} common message publisher)
endif()

if(NOT SKIP_SUB)
    add_executable(sub_meteo smain.cpp)
    target_link_libraries(sub_meteo yocto common message subscriber)
endif()
