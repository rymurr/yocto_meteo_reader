cmake_minimum_required( VERSION 2.8 FATAL_ERROR )

include_directories(.)

set(subscriber_src daily_call.cpp  sub_param_helper.cpp  subscriber.cpp  writer.cpp)
add_library(subscriber SHARED ${subscriber_src})

