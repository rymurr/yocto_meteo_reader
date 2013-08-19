#include "subscriber.hpp"

Subscriber::Subscriber(boost::shared_ptr<AbstractWriter> writer, std::string protocol, std::string host, int port): _context(1), _subscriber(_context, ZMQ_SUB), _sync(_context, ZMQ_REQ), _writer(writer), _max(2) {
    _subscriber.connect(connect_name(protocol, host, port).c_str());
    _subscriber.setsockopt( ZMQ_SUBSCRIBE, "", 0);
    _sync.connect(connect_name(protocol, host, port+1).c_str());
    s_send(_sync,"");
    std::string conf_string = s_recv(_sync);
    _msg_type = parse_msg(conf_string);
}   

void Subscriber::operator()() {
    while (1) {
        std::string address = s_recv(_subscriber);
        boost::shared_ptr<Message> r = s_recvobj(_subscriber, _msg_type);
        std::cout << r->string() << std::endl;
        _bson_queue.push_back(r);
        if (_bson_queue.size() > _max) {
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
    _writer->drain(_bson_queue);
    _bson_queue.clear();

}

int main () {
    std::string dir("/data/meteo/storage");
    boost::shared_ptr<AbstractWriter> w = WriterBuilder::create(writer_t::FILEDIR_WEEKLY, dir, message_type_t::BSON);
    boost::function<void(void)> f = boost::bind(&AbstractWriter::clear, w);
    Subscriber s(w); 
    boost::thread subThread(boost::ref(s));
    start_callbacks(f);

    return 0;
}
