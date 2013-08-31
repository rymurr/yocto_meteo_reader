#include "threaded_reqrep.hpp"

void threaded_rep(std::string hostname, std::string msg){
    zmq::context_t context(1);
    zmq::socket_t sync(context, ZMQ_REP);
    sync.bind(hostname.c_str());
    while(1) {
        std::string rec = s_recv(sync);
        BOOST_LOG_TRIVIAL(info) << "recieved req/rep message from: " << rec;
        s_send(sync, msg);
    }
}

