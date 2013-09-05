#ifndef _PUBLISHER_HPP_
#define _PUBLISHER_HPP_

#include <string>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/log/trivial.hpp>
#include <zmq.hpp>

#include "base_message.hpp"
#include "make_msg.hpp"
#include <string>

std::string make_msg(message_type_t);

void threaded_rep(std::string, std::string);

static std::string    
connect_name(std::string protocol, std::string host, int port) {
    return protocol.append("://").append(host).append(":").append(std::to_string(port));
}

class Publisher{

    private:
        zmq::context_t _context;
        zmq::socket_t _publisher;
        boost::thread _rep_thread;
        
    public:
        Publisher(std::string hostname="*", int port=5563, std::string protocol="tcp", message_type_t msg_type=BSON);
        void callback(boost::shared_ptr<Message> x) ;

        void startThread(std::string& protocol, std::string& hostname, int port, std::string msg) ;


};

#endif 
