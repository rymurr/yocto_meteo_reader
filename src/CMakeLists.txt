cmake_minimum_required( VERSION 2.8 FATAL_ERROR )

include_directories (.)
include_directories(../yocto)

link_libraries(${LIBS} -lpthread )
set(EXECUTABLE_OUTPUT_PATH "../")
add_executable(meteo main.cpp sensor.cpp)
target_link_libraries(meteo yocto ${LIBUSB_1_LIBRARIES})
