#ifndef _PROTOBUF_MESSAGE_HPP_
#define _PROTOBUF_MESSAGE_HPP_

#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/shared_ptr.hpp>
#include "base_message.hpp"

#ifdef PROTOBUF_AVAIL
#include "meteo.pb.h"

class ProtoBufMessage: public Message {
    private:
        meteo::SensorReading _obj;
    public:
        ProtoBufMessage(std::string device, std::string sensor, long long timestamp, double value) {
            _obj.set_value(value); 
            _obj.set_device(device);
            _obj.set_sensor(sensor);
            _obj.set_timestamp(timestamp);
        }

        ProtoBufMessage(char* data){
            _obj.ParseFromString(std::string(data));           
        }

        virtual msgPtr fillmessage() {
            std::string msg = _obj.SerializeAsString();
            return boost::shared_ptr<zmq::message_t>(new zmq::message_t(const_cast<char*>(msg.c_str()), msg.size(), msg_free));
        }

        virtual std::string id() {
            return _obj.sensor();
        }

        virtual std::string string() {
            boost::property_tree::ptree pt;
            pt.put("device",_obj.device());
            pt.put("sensor",_obj.sensor());
            pt.put("value",_obj.value());
            pt.put("timestamp",_obj.timestamp());
            std::ostringstream buf;
            boost::property_tree::write_json(buf,pt, false);
            return buf.str();
        }
        
};
#else
class ProtoBufMessage: public Message {
    public:
        ProtoBufMessage(std::string device, std::string sensor, long long timestamp, double value) {};
        ProtoBufMessage(char* data){};
        virtual msgPtr fillmessage(){ return NULL;};
        virtual std::string id() { return "";};
        virtual std::string string() {return "";};
};
#endif

#endif
