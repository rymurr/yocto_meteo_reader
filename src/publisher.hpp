#ifndef _PUBLISHER_HPP_
#define _PUBLISHER_HPP_

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

#include "glog/logging.h"
#include "sensor.hpp"
#include "zhelpers.hpp"
#include "message.hpp"

enum message_type_t {BSON,JSON,PROTOBUF,PLAINTEXT};

void threaded_rep(boost::shared_ptr<zmq::socket_t>, std::string&);

class Publisher{

    private:
        zmq::context_t _context;
        zmq::socket_t _publisher;
        zmq::socket_t _sync;
        std::string _msg;
        boost::thread _rep_thread;
        
    public:
        Publisher(std::string hostname="*", int port=5563, std::string protocol="tcp://", message_type_t msg_type=BSON):_context(1),
                             _publisher(_context, ZMQ_PUB),
                             _sync(_context, ZMQ_REP) {
             _publisher.bind(protocol.append(hostname).append(":").append(std::to_string(port)).c_str());
             _sync.bind(protocol.append(hostname).append(std::to_string(port+1)).c_str());
             std::string msg = "message_";
             _msg = msg.append(std::to_string(msg_type));    
             //s_sendmore(_publisher, "CONTROL");
             //s_send(_publisher, msg.append(std::to_string(mgs_type)));
        }
        void callback(Message& x) {
             s_sendmore(_publisher,x.id());
             s_sendobj(_publisher, x);
             sleep(1);
        }

        void startThread() {
            _rep_thread = boost::thread(threaded_rep, _sync, _msg);         
        }

};

void threaded_rep(boost::shared_ptr<zmq::socket_t> sync, std::string& msg) {
    while(1) {
        s_recv(sync);
        s_send(sync, msg);
    }
}


#endif 
