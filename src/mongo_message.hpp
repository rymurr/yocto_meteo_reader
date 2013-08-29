#ifndef _MONGO_MESSAGE_HPP_
#define _MONGO_MESSAGE_HPP_

#include <mongo/client/dbclient.h>
#include "base_message.hpp"

class MongoMessage : public Message {
    private:
        mongo::BSONObj _obj;
    public:
        MongoMessage(std::string device, std::string sensor, long long timestamp, double value) {
            mongo::BSONObjBuilder b;
            b.append("value", value);
            b.append("device", device);
            b.append("sensor", sensor);
            b.append("timestamp", mongo::Date_t(timestamp));
            _obj = b.obj();
        }

        MongoMessage(char* data){
            _obj = mongo::BSONObj(data).copy();
        }

        virtual msgPtr fillmessage() {
            return boost::shared_ptr<zmq::message_t>(new zmq::message_t(const_cast<char*>(_obj.objdata()), _obj.objsize(), msg_free));
        }

        virtual std::string id() {
            return _obj.getStringField("sensor");
        }

        virtual std::string string() {
            return _obj.jsonString();
        }

        const mongo::BSONObj obj(){return _obj;}
};

#endif
