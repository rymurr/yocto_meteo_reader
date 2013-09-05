#ifndef __SUBSCRIBER_HPP__
#define __SUBSCRIBER_HPP__

#include <vector>
#include <boost/thread.hpp>
#include <boost/log/trivial.hpp>
#include <zmq.hpp>
#include "make_msg.hpp"
#include "base_message.hpp"
#include "base_writer.hpp"

static std::string    
connect_name(std::string protocol, std::string host, int port) {
    return protocol.append("://").append(host).append(":").append(std::to_string(port));
}

class Subscriber {

    private:
        zmq::context_t _context;
        zmq::socket_t _subscriber;
        zmq::socket_t _sync;
        message_type_t _msg_type;
        std::vector<boost::shared_ptr<Message> > _bson_queue;
        boost::mutex guard;
        boost::shared_ptr<AbstractWriter> _writer;
        std::size_t _max;

    public:
        Subscriber(boost::shared_ptr<AbstractWriter> writer, std::string protocol="tcp", std::string host="localhost", int port=5563, int queue=2);

        void operator()(); 


        void drain_queue() ;
        

};

#endif
