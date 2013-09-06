#include "json_message.hpp"
#include <sstream>
#include <boost/property_tree/json_parser.hpp>
#include <boost/shared_ptr.hpp>

JSONMessage::JSONMessage(std::string& device, std::string& sensor, long long timestamp, double value) {
    _obj.put("device",device);
    _obj.put("sensor",sensor);
    _obj.put("value",value);
    _obj.put("timestamp",timestamp);
}

JSONMessage::JSONMessage(char* data){
    std::istringstream is(data);
    boost::property_tree::read_json(is, _obj);
}

msgPtr JSONMessage::fillmessage() {
    return boost::shared_ptr<zmq::message_t>(new zmq::message_t(const_cast<char*>(this->string().c_str()), this->string().size(), msg_free));
}

std::string JSONMessage::id() {
    return _obj.get<std::string>("sensor");
}

std::string JSONMessage::device() {
    return _obj.get<std::string>("device");
}

double JSONMessage::value() {
    return _obj.get<double>("value");
}

long long JSONMessage::timestamp() {
    return _obj.get<long long>("timestamp");
}

std::string JSONMessage::string() {
    std::ostringstream buf;
    boost::property_tree::write_json(buf,_obj, false);
    return buf.str();
}
        


