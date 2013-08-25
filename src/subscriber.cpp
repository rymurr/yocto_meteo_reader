#include "subscriber.hpp"

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

message_type_t Subscriber::parse_msg(std::string& msg) {
    boost::property_tree::ptree pt;
    std::istringstream is(msg);
    boost::property_tree::read_json(is, pt);
    return static_cast<message_type_t>(pt.get<int>("type"));
}

void Subscriber::drain_queue() {
    boost::mutex::scoped_lock(guard);
    const int x = _writer->drain(_bson_queue);
    if (x<0)
        _bson_queue.clear();

}

void SubControl::sigill(){
    //BOOST_LOG_TRIVIAL(warning) << "Attempting to run a heap check";
    //HeapLeakChecker::NoGlobalLeaks();    
    return;
};

void SubControl::sigint(){
    BOOST_LOG_TRIVIAL(fatal) << "Recieved sigint! Goodbye!";
    _s->drain_queue();
    exit(1);
};

void SubControl::intHandler(int sig) {
    switch(sig) {
        case 2:
            sigint();
            break;
        case 4:
            sigill();
            break;
        default:
            BOOST_LOG_TRIVIAL(error) << "Unable to handle signal!";
    }
};

SubControl sc;
void intHandler(int sig){
    sc.intHandler(sig);
}

int main (int argc, char** argv) {
    init("subscriber");

    SubscriberParams sp;
    const int paramRet = sp.parse_options(argc, argv);
    if (paramRet != 0) {
        return 1;
    }
    boost::shared_ptr<AbstractWriter> w = WriterBuilder::create(sp.getStorageFormat(), sp.getOption());
    boost::function<void(void)> f = boost::bind(&AbstractWriter::clear, w);
    boost::shared_ptr<Subscriber> s = boost::shared_ptr<Subscriber>(new Subscriber(w, "tcp", sp.getPublishHostName(), sp.getPublishPort(), sp.getQueueSize())); 
    sc = SubControl(s);
    signal(SIGINT, intHandler);
    signal(SIGILL, intHandler);

    boost::thread subThread(boost::ref(*s));
    start_callbacks(f);
    return 0;
}
