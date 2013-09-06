#ifndef _PROTOBUF_MESSAGE_HPP_
#define _PROTOBUF_MESSAGE_HPP_

#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/shared_ptr.hpp>
#include <algorithm>
#include "base_message.hpp"

#ifdef PROTOBUF_AVAIL
#include "meteo.pb.h"

class ProtoBufMessage: public Message {
    private:
        meteo::SensorReading _obj;
        boost::shared_ptr<void> _msg;

    public:
        ProtoBufMessage(std::string device, std::string sensor, long long timestamp, double value) {
            _obj.set_value(value); 
            std::cout << "value is: " << _obj.value() << std::endl;
            _obj.set_device(device);
            _obj.set_sensor(sensor);
            _obj.set_timestamp(timestamp);
        }

        ProtoBufMessage(void* data){
            int size = strlen(static_cast<char*>(data));
            int rsize = std::max(size,39);
            _obj.ParseFromArray(data, rsize);
        }

        virtual msgPtr fillmessage() {
            int size = _obj.ByteSize();
            _msg = boost::shared_ptr<void>(malloc(size));
            _obj.SerializeToArray(_msg.get(), size);
            return boost::shared_ptr<zmq::message_t>(new zmq::message_t(_msg.get(), size, msg_free));
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
