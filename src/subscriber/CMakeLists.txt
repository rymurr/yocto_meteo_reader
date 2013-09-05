cmake_minimum_required( VERSION 2.8 FATAL_ERROR )
include_directories(.)

#if(${MongoDB_FOUND})
#    link_libraries(${MongoDB_LIBRARIES})
#endif()    

set(subscriber_src daily_call.cpp  sub_param_helper.cpp  subscriber.cpp  writer.cpp sub_control.cpp disk_writer.cpp mongo_writer.cpp weekly_disk_writer.cpp)
add_library(subscriber SHARED ${subscriber_src})

