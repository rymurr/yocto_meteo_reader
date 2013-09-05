#ifndef _MAKE_MSG_HPP_
#define _MAKE_MSG_HPP_

#include <string>
#include "base_message.hpp"
message_type_t parse_msg(std::string& msg) ;

std::string make_msg(message_type_t);

#endif
