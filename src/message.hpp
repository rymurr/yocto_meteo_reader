
#ifndef _MESSAGE_HPP_
#define _MESSAGE_HPP_

#include <boost/shared_ptr.hpp>
#include <mongo/client/dbclient.h>
#include <zhelpers.hpp>

typedef boost::shared_ptr<zmq::message_t> msgPtr;

class Message{
    public:
        virtual msgPtr fillmessage() = 0;
        virtual std::string id() = 0;
};

static void msg_free(void *data, void *hint) {}

class MongoMessage : public Message {
    private:
        mongo::BSONObj _obj;
    public:
        MongoMessage(std::string& device, std::string& sensor, long long timestamp, double value) {
            mongo::BSONObjBuilder b;
            b.append("value", value);
            b.append("device", device);
            b.append("sensor", sensor);
            b.append("timestamp", mongo::Date_t(timestamp));
            _obj = b.obj();
        }

        virtual msgPtr fillmessage() {
            return boost::shared_ptr<zmq::message_t>(new zmq::message_t(const_cast<char*>(_obj.objdata()), _obj.objsize(), msg_free));
        }

        virtual std::string id() {
            return _obj.getStringField("sensor");
        }

};

static bool
s_sendobj (zmq::socket_t & socket, Message& obj) {

    msgPtr message = obj.fillmessage();

    bool rc = socket.send (*message);
    return (rc);
}

static mongo::BSONObj
s_recvobj (zmq::socket_t & socket) {

    zmq::message_t message;
    socket.recv(&message);

    return mongo::BSONObj(static_cast<char*>(message.data()));
}

#endif

/* protobuf stuff
 * reading.set_value(boost::lexical_cast<double>(value)); 
 * reading.set_device(strs[0]);
 * reading.set_sensor(strs[1]);
 * reading.set_timestamp(return_ms_from_epoch(now));
 */
