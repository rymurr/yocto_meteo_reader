
#include "message.hpp"
boost::shared_ptr<Message> convertToMongo(boost::shared_ptr<Message> m, message_type_t intype, message_type_t outtype) {
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
            msg = NULL;
    }
    return msg;
}

boost::shared_ptr<Message> convertToMongo(boost::shared_ptr<Message> m, message_type_t mtype) {
    boost::shared_ptr<Message> msg;
    switch (mtype) {
        case JSON:
            break;
        case PROTOBUF:
            break;
        case BSON:
            msg = m;
            break;
        default:
            msg = NULL;
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
            break;
        case BSON:
            break;
        default:
            msg = NULL;
    }
    return msg;    
}

boost::shared_ptr<Message> convertToProtoBuf(boost::shared_ptr<Message> m, message_type_t mtype) {
    boost::shared_ptr<Message> msg;
    switch (mtype) {
        case JSON:
            break;
        case PROTOBUF:
            msg = m;
            break;
        case BSON:
            break;
        default:
            msg = NULL;
    }
    return msg;    
}
