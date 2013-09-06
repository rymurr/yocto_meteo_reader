#include "mongo_message.hpp"
#include "protobuf_message.hpp"
#include "json_message.hpp"
#include "message.hpp"

boost::shared_ptr<Message> convert(boost::shared_ptr<Message> m, message_type_t intype, message_type_t outtype) {
    boost::shared_ptr<Message> msg;
    switch(outtype) {
        case JSON:
            msg = convertToJSON(m, intype);
            break;
        case BSON:
            msg = convertToMongo(m, intype);
            break;
        case PROTOBUF:
            msg = convertToProtoBuf(m, intype);
            break;
        default:
            msg = boost::shared_ptr<Message>();
    }
    return msg;
}

boost::shared_ptr<Message> convertToMongo(boost::shared_ptr<Message> m, message_type_t mtype) {
    boost::shared_ptr<Message> msg;
    boost::shared_ptr<JSONMessage> x;
    switch (mtype) {
        case JSON:
            x = boost::static_pointer_cast<JSONMessage>(m);
            msg = boost::shared_ptr<Message>(new MongoMessage(x->id(), x->device(), x->timestamp(), x->value()));
            break;
        case PROTOBUF:
            x = boost::make_shared<JSONMessage>(JSONMessage(const_cast<char*>(m->string().c_str())));
            msg = boost::shared_ptr<Message>(new MongoMessage(x->id(), x->device(), x->timestamp(), x->value()));
            break;
        case BSON:
            msg = m;
            break;
        default:
            msg = boost::shared_ptr<Message>();
    }
    return msg;    
}

boost::shared_ptr<Message> convertToJSON(boost::shared_ptr<Message> m, message_type_t mtype) {
    boost::shared_ptr<Message> msg;
    switch (mtype) {
        case JSON:
            msg = m;
            break;
        case PROTOBUF:
            msg = boost::make_shared<JSONMessage>(JSONMessage(const_cast<char*>(m->string().c_str())));
            break;
        case BSON:
            msg = boost::make_shared<JSONMessage>(JSONMessage(const_cast<char*>(m->string().c_str())));
            break;
        default:
            msg = boost::shared_ptr<Message>();
    }
    return msg;    
}

boost::shared_ptr<Message> convertToProtoBuf(boost::shared_ptr<Message> m, message_type_t mtype) {
    boost::shared_ptr<Message> msg;
    boost::shared_ptr<JSONMessage> x;
    switch (mtype) {
        case JSON:
            x = boost::static_pointer_cast<JSONMessage>(m);
            msg = boost::shared_ptr<Message>(new ProtoBufMessage(x->id(), x->device(), x->timestamp(), x->value()));
            break;
        case PROTOBUF:
            msg = m;
            break;
        case BSON:
            x = boost::make_shared<JSONMessage>(JSONMessage(const_cast<char*>(m->string().c_str())));
            msg = boost::shared_ptr<Message>(new ProtoBufMessage(x->id(), x->device(), x->timestamp(), x->value()));
            break;
        default:
            msg = boost::shared_ptr<Message>();
    }
    return msg;    
}

boost::shared_ptr<Message> make_message(std::string sensor, std::string device, long long timestamp, double value, message_type_t msg_type) {
    boost::shared_ptr<Message> msg;
    switch(msg_type) {
        case BSON:
            msg = boost::shared_ptr<Message>(new MongoMessage(sensor, device, timestamp, value));
            break;
        case JSON:
            msg = boost::shared_ptr<Message>(new JSONMessage(sensor, device, timestamp, value));
            break;
        case PROTOBUF:
            msg = boost::shared_ptr<Message>(new ProtoBufMessage(sensor, device, timestamp, value));
            break;
        default:
            msg = boost::shared_ptr<Message>();    
    }
    return msg;
}



boost::shared_ptr<Message> s_recvobj (zmq::socket_t & socket, message_type_t msg_type) {
    zmq::message_t message;
    socket.recv(&message);

    boost::shared_ptr<Message> m;
    switch(msg_type) {
        case BSON:
            m = boost::make_shared<MongoMessage>(MongoMessage(static_cast<char*>(message.data())));
            break;
        case PROTOBUF:
            m = boost::make_shared<ProtoBufMessage>(ProtoBufMessage(static_cast<void*>(message.data()), message.size()));
            break;
        case JSON:
            m = boost::make_shared<JSONMessage>(JSONMessage(static_cast<char*>(message.data())));
            break;
        default:
            m = boost::shared_ptr<Message>();    
    }
    return m;
}


