#ifndef _BASE_MESSAGE_HPP_
#define _BASE_MESSAGE_HPP_

#include <string>
#include <boost/shared_ptr.hpp>
#include <zmq.hpp>

enum message_type_t {BSON,JSON,PROTOBUF,PLAINTEXT,INVALID};
typedef boost::shared_ptr<zmq::message_t> msgPtr;

class Message{
    public:
        virtual msgPtr fillmessage() = 0;
        virtual std::string id() = 0;
        virtual std::string string() = 0;
        
};

static void msg_free(void *data, void *hint) {}

#endif
