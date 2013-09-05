#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "base_message.hpp"

std::string make_msg(message_type_t msg_type) {
    boost::property_tree::ptree pt;
    pt.put("type", msg_type);
    std::ostringstream buf;
    boost::property_tree::write_json(buf, pt, false);
    return buf.str();
}

message_type_t parse_msg(std::string& msg) {
    boost::property_tree::ptree pt;
    std::istringstream is(msg);
    boost::property_tree::read_json(is, pt);
    return static_cast<message_type_t>(pt.get<int>("type"));
}


