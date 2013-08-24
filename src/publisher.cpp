#include "publisher.hpp"

int main(int argc, char * argv[])
{
    google::InitGoogleLogging(argv[0]);
    PublisherParams pp;
    const int paramRet = pp.parse_options(argc, argv);
    if (paramRet != 0) {
        return 1;
    }
    boost::shared_ptr<Publisher> y = boost::make_shared<Publisher>(Publisher("*", pp.getPort(), "tcp", pp.getMessageType()));
    boost::function<void (Message&)> fct = boost::bind<void>(&Publisher::callback, y, _1);
    BOOST_FOREACH(std::string x, pp.getDevices()) {
        SensorGroup::getInstance().addAllowedDevice(x);
    }
    BOOST_FOREACH(std::string x, pp.getSensorTypes()) {
        SensorGroup::getInstance().addAllowedSensor(x);
    }
    SensorGroup::getInstance().addCallback(fct);
    SensorGroup::getInstance().start();
}

void threaded_rep(std::string hostname, std::string msg){
    zmq::context_t context(1);
    zmq::socket_t sync(context, ZMQ_REP);
    sync.bind(hostname.c_str());
    while(1) {
        s_recv(sync);
        s_send(sync, msg);
    }
}

std::string Publisher::make_msg(message_type_t msg_type) {
    boost::property_tree::ptree pt;
    pt.put("type", msg_type);
    std::ostringstream buf;
    boost::property_tree::write_json(buf, pt, false);
    return buf.str();
}

