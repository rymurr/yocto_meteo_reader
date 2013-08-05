cmake_minimum_required( VERSION 2.8 FATAL_ERROR )

include_directories (.)
include_directories(../yocto)

PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS meteo.proto)
link_libraries(${LIBS} -lpthread )
set(EXECUTABLE_OUTPUT_PATH "../")
add_executable(meteo main.cpp sensor.cpp ${PROTO_SRCS} ${PROTO_HDRS})
target_link_libraries(meteo yocto ${LIBUSB_1_LIBRARIES})
