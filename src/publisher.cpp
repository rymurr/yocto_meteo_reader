#include "publisher.hpp"


PubControl sc;
void intHandler(int sig){
    sc.intHandler(sig);
}

int main(int argc, char * argv[])
{
    init("publisher");
    PublisherParams pp;
    const int paramRet = pp.parse_options(argc, argv);
    if (paramRet != 0) {
        return 1;
    }
    boost::shared_ptr<Publisher> y = boost::make_shared<Publisher>(Publisher("*", pp.getPort(), "tcp", pp.getMessageType()));
    boost::function<void (Message&)> fct = boost::bind<void>(&Publisher::callback, y, _1);
    sc = PubControl(y);
    signal(SIGINT, intHandler);
    signal(SIGILL, intHandler);
    SensorGroup::getInstance().setMsgType(pp.getMessageType());
    BOOST_FOREACH(std::string x, pp.getDevices()) {
        SensorGroup::getInstance().addAllowedDevice(x);
    }
    BOOST_FOREACH(std::string x, pp.getSensorTypes()) {
        SensorGroup::getInstance().addAllowedSensor(x);
    }
    SensorGroup::getInstance().addCallback(fct);
    SensorGroup::getInstance().start();
}

std::string Publisher::make_msg(message_type_t msg_type) {
    boost::property_tree::ptree pt;
    pt.put("type", msg_type);
    std::ostringstream buf;
    boost::property_tree::write_json(buf, pt, false);
    return buf.str();
}

Publisher::Publisher(std::string hostname="*", int port=5563, std::string protocol="tcp", message_type_t msg_type=BSON):_context(1),
                     _publisher(_context, ZMQ_PUB){
     BOOST_LOG_TRIVIAL(info) << "Starting Publusher on port " << port;                                
     _publisher.bind(connect_name(protocol, hostname, port).c_str());
     std::string msg = make_msg(msg_type);
     startThread(protocol, hostname, port+1, msg);
}
void Publisher::callback(Message& x) {
     s_sendmore(_publisher,x.id());
     s_sendobj(_publisher, x);
     sleep(1);
}

void Publisher::startThread(std::string& protocol, std::string& hostname, int port, std::string msg) {
    BOOST_LOG_TRIVIAL(info) << "Starting Req/Rep listener with reply message: " << msg << " on port " << port;
    _rep_thread = boost::thread(threaded_rep, connect_name(protocol, hostname, port), msg);         
    _rep_thread.detach();
}


