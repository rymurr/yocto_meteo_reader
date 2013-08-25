#include "publisher.hpp"

void PubControl::sigill(){
    //BOOST_LOG_TRIVIAL(warning) << "Attempting to run a heap check";
    //HeapLeakChecker::NoGlobalLeaks();    
    return;
};

void PubControl::sigint(){
    BOOST_LOG_TRIVIAL(fatal) << "Recieved sigint! Goodbye!";
    exit(1);
};

void PubControl::intHandler(int sig) {
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
        std::string rec = s_recv(sync);
        BOOST_LOG_TRIVIAL(info) << "recieved req/rep message from: " << rec;
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

