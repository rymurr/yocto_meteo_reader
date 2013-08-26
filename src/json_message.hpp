#ifndef _JSON_MESSAGE_HPP_
#define _JSON_MESSAGE_HPP_

#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/shared_ptr.hpp>
#include "base_message.hpp"

class JSONMessage: public Message {
    private:
        std::string _obj;
        std::string _sensor;
    public:
        JSONMessage(std::string& device, std::string& sensor, long long timestamp, double value) {
            boost::property_tree::ptree pt;
            pt.put("device",device);
            pt.put("sensor",sensor);
            pt.put("value",value);
            pt.put("timestamp",timestamp);
            std::ostringstream buf;
            boost::property_tree::write_json(buf,pt, false);
            _obj = buf.str();
            _sensor = sensor;
        }

        JSONMessage(char* data){
            _obj=data;           
        }

        virtual msgPtr fillmessage() {
            return boost::shared_ptr<zmq::message_t>(new zmq::message_t(const_cast<char*>(_obj.c_str()), _obj.size(), msg_free));
        }

        virtual std::string id() {
            return _sensor;
        }

        virtual std::string string() {
            return _obj;
        }
        
};

#endif
