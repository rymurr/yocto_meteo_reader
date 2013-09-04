cmake_minimum_required( VERSION 2.8 FATAL_ERROR )

include_directories (.)
include_directories(../yoctolib_cpp/Sources)

link_libraries(${LIBS} -lpthread )
if(PROTOBUF_FOUND)
    PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS meteo.proto)
endif(PROTOBUF_FOUND)

set(EXECUTABLE_OUTPUT_PATH "../")
add_executable(pub_meteo pmain.cpp publisher_control.cpp threaded_reqrep.cpp publisher_make_msg.cpp publisher.cpp sensor.cpp sensor_group.cpp pub_param_helper.cpp message.cpp logger_config_file.cpp  logger_config_console.cpp logger_config_common.cpp ${PROTO_SRCS} ${PROTO_HDRS})
add_executable(sub_meteo subscriber.cpp writer.cpp daily_call.cpp sub_param_helper.cpp message.cpp logger_config_file.cpp logger_config_console.cpp logger_config_common.cpp ${PROTO_SRCS} ${PROTO_HDRS})
target_link_libraries(sub_meteo yocto ${LIBUSB_1_LIBRARIES})
target_link_libraries(pub_meteo yocto ${LIBUSB_1_LIBRARIES})
