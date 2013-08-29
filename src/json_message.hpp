#ifndef _JSON_MESSAGE_HPP_
#define _JSON_MESSAGE_HPP_

#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/shared_ptr.hpp>
#include "base_message.hpp"

class JSONMessage: public Message {
    private:
        boost::property_tree::ptree _obj;
        
    public:
        JSONMessage(std::string& device, std::string& sensor, long long timestamp, double value) {
            _obj.put("device",device);
            _obj.put("sensor",sensor);
            _obj.put("value",value);
            _obj.put("timestamp",timestamp);
        }

        JSONMessage(char* data){
            std::istringstream is(data);
            boost::property_tree::read_json(is, _obj);
        }

        virtual msgPtr fillmessage() {
            return boost::shared_ptr<zmq::message_t>(new zmq::message_t(const_cast<char*>(this->string().c_str()), this->string().size(), msg_free));
        }

        virtual std::string id() {
            return _obj.get<std::string>("sensor");
        }

        std::string device() {
            return _obj.get<std::string>("device");
        }

        double value() {
            return _obj.get<double>("value");
        }

        long long timestamp() {
            return _obj.get<long long>("timestamp");
        }

        virtual std::string string() {
            std::ostringstream buf;
            boost::property_tree::write_json(buf,_obj, false);
            return buf.str();
        }
        
};

#endif
