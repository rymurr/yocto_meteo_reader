#ifndef _MONGO_WRITER_HPP_
#define _MONGO_WRITER_HPP_


#include <string>
#include "base_writer.hpp"
#include "base_message.hpp"
#ifdef MONGO_AVAIL
#include <boost/thread.hpp>

class MongoWriter: public AbstractWriter {
    private: 
        const std::string _hostname;
        message_type_t _msg_type;
        boost::mutex guard;

    public:
        MongoWriter(std::string& hostname): _hostname(hostname), _msg_type(INVALID){}
    
        virtual void setMsgType(message_type_t msg_type) { _msg_type = msg_type;}
        virtual void setMsgTypeStore(message_type_t msg_type) {}
        virtual int drain(msgArr);
        virtual void clear(){};


};
#else
class MongoWriter: public AbstractWriter {
    public:
        MongoWriter(std::string& hostname){};
        virtual void setMsgType(message_type_t){};
        virtual void setMsgTypeStore(message_type_t){};
        virtual int drain(msgArr){return -1;};
        virtual void clear(){};
};

#endif
#endif
