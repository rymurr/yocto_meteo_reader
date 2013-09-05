#include "subscriber.hpp"
#include "message.hpp"

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

Subscriber::Subscriber(boost::shared_ptr<AbstractWriter> writer, std::string protocol, std::string host, int port, int max): _context(1), _subscriber(_context, ZMQ_SUB), _sync(_context, ZMQ_REQ), _writer(writer), _max(max) {
    BOOST_LOG_TRIVIAL(info) << "Subscribing to: " << connect_name(protocol, host, port);
    _subscriber.connect(connect_name(protocol, host, port).c_str());
    _subscriber.setsockopt( ZMQ_SUBSCRIBE, "", 0);
    _sync.connect(connect_name(protocol, host, port+1).c_str());
    BOOST_LOG_TRIVIAL(info) << "Asking " << connect_name(protocol, host, port+1) << " for configuration";
    s_send(_sync,"");
    std::string conf_string = s_recv(_sync);
    BOOST_LOG_TRIVIAL(info) << "Recieved: " << conf_string;
    _msg_type = parse_msg(conf_string);
    writer->setMsgType(_msg_type);
}   

void Subscriber::operator()() {
    while (1) {
        std::string address = s_recv(_subscriber);
        boost::shared_ptr<Message> r = s_recvobj(_subscriber, _msg_type);
        BOOST_LOG_TRIVIAL(info) << "Recieved: " << r->string();
        _bson_queue.push_back(r);
        if (_bson_queue.size() > _max) {
            BOOST_LOG_TRIVIAL(info) << "Queue is full! Draining now";
            drain_queue();
        }

    }
}

void Subscriber::drain_queue() {
    boost::mutex::scoped_lock(guard);
    const int x = _writer->drain(_bson_queue);
    if (x<0)
        _bson_queue.clear();

}


