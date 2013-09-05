#ifndef _MSG_OPERATOR_HPP_
#define _MSG_OPERATOR_HPP_
#include "base_message.hpp"

static std::istream& operator>>(std::istream& in, message_type_t& unit)
{
    std::string token;
    in >> token;
    if (token == "bson")
        unit = BSON;
    else if (token == "json")
        unit = JSON;
    else if (token == "protobuf")
        unit = PROTOBUF;
    return in;
}

#endif
