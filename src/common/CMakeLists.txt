cmake_minimum_required( VERSION 2.8 FATAL_ERROR )

include_directories(.)

set(common_src logger_config_common.cpp logger_config_console.cpp logger_config_file.cpp make_msg.cpp file_helpers.cpp)
add_library(common SHARED ${common_src})

