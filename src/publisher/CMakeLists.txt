cmake_minimum_required( VERSION 2.8 FATAL_ERROR )

include_directories(.)

set(publisher_src publisher_control.cpp  publisher.cpp  publisher_make_msg.cpp  pub_param_helper.cpp  sensor.cpp  sensor_group.cpp  threaded_reqrep.cpp )
add_library(publisher SHARED ${publisher_src})

