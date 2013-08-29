
#ifndef _MESSAGE_HPP_
#define _MESSAGE_HPP_

#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/shared_ptr.hpp>
#include "meteo.pb.h"
#include "mongo_message.hpp"
#include "protobuf_message.hpp"
#include "json_message.hpp"

static bool
s_sendobj (zmq::socket_t & socket, Message& obj) {

    msgPtr message = obj.fillmessage();

    bool rc = socket.send (*message);
    return (rc);
}

static boost::shared_ptr<Message>
s_recvobj (zmq::socket_t & socket, message_type_t msg_type) {

    zmq::message_t message;
    socket.recv(&message);

    boost::shared_ptr<Message> m;
    switch(msg_type) {
        case BSON:
            m = boost::make_shared<MongoMessage>(MongoMessage(static_cast<char*>(message.data())));
            break;
        case PROTOBUF:
            m = boost::make_shared<ProtoBufMessage>(ProtoBufMessage(static_cast<char*>(message.data())));
            break;
        case JSON:
            m = boost::make_shared<JSONMessage>(JSONMessage(static_cast<char*>(message.data())));
            break;
        default:
            m = NULL;    
    }
    return m;
}

boost::shared_ptr<Message> make_message(std::string sensor, std::string device, long long timestamp, double value, message_type_t msg_type);

boost::shared_ptr<Message> convert(boost::shared_ptr<Message> m, message_type_t intype, message_type_t outtype);
boost::shared_ptr<Message> convertToMongo(boost::shared_ptr<Message> m, message_type_t mtype) ;
boost::shared_ptr<Message> convertToJSON(boost::shared_ptr<Message> m, message_type_t mtype) ;
boost::shared_ptr<Message> convertToProtoBuf(boost::shared_ptr<Message> m, message_type_t mtype) ;
#endif

