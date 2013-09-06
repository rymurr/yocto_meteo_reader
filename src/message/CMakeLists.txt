cmake_minimum_required( VERSION 2.8 FATAL_ERROR )

include_directories(.)

if(PROTOBUF_FOUND)
  PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS meteo.proto)
endif(PROTOBUF_FOUND)
  
set(message_src message.cpp json_message.cpp ${PROTO_SRCS} ${PROTO_HDRS})

#if(${MongoDB_FOUND})
#    link_libraries(${MongoDB_LIBRARIES})
#endif()    
add_library(message SHARED ${message_src})

