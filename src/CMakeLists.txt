cmake_minimum_required( VERSION 2.8 FATAL_ERROR )

include_directories (.)
include_directories(../yocto)

link_libraries(${LIBS} -lpthread )
PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS meteo.proto)
set(EXECUTABLE_OUTPUT_PATH "../")
add_executable(pub_meteo publisher.cpp sensor.cpp param_helper.cpp message.cpp ${PROTO_SRCS} ${PROTO_HDRS})
add_executable(sub_meteo subscriber.cpp writer.cpp daily_call.cpp param_helper.cpp message.cpp ${PROTO_SRCS} ${PROTO_HDRS})
target_link_libraries(sub_meteo yocto ${LIBUSB_1_LIBRARIES})
target_link_libraries(pub_meteo yocto ${LIBUSB_1_LIBRARIES})
