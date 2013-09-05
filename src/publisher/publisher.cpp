#include "publisher.hpp"

static bool
s_send (zmq::socket_t & socket, const std::string & string) {

    zmq::message_t message(string.size());
    memcpy (message.data(), string.data(), string.size());

    bool rc = socket.send (message);
    return (rc);
}

static bool
s_sendmore (zmq::socket_t & socket, const std::string & string) {

    zmq::message_t message(string.size());
    memcpy (message.data(), string.data(), string.size());

    bool rc = socket.send (message, ZMQ_SNDMORE);
    return (rc);
}

static std::string
s_recv (zmq::socket_t & socket) {

    zmq::message_t message;
    socket.recv(&message);

    return std::string(static_cast<char*>(message.data()), message.size());
}

static bool
s_sendobj (zmq::socket_t & socket, Message& obj) {

    msgPtr message = obj.fillmessage();

    bool rc = socket.send (*message);
    return (rc);
}

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

Publisher::Publisher(std::string hostname, int port, std::string protocol, message_type_t msg_type):_context(1),
                     _publisher(_context, ZMQ_PUB){
     BOOST_LOG_TRIVIAL(info) << "Starting Publusher on port " << port;                                
     _publisher.bind(connect_name(protocol, hostname, port).c_str());
     std::string msg = make_msg(msg_type);
     startThread(protocol, hostname, port+1, msg);
}
void Publisher::callback(boost::shared_ptr<Message> x) {
     s_sendmore(_publisher,x->id());
     s_sendobj(_publisher, *x);
     sleep(1);
}

void Publisher::startThread(std::string& protocol, std::string& hostname, int port, std::string msg) {
    BOOST_LOG_TRIVIAL(info) << "Starting Req/Rep listener with reply message: " << msg << " on port " << port;
    _rep_thread = boost::thread(threaded_rep, connect_name(protocol, hostname, port), msg);         
    _rep_thread.detach();
}


