#include <boost/foreach.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/bind.hpp>
#include <signal.h>
#include "logger_config.hpp"
#include "sensor_group.hpp"
#include "pub_param_helper.hpp"
#include "publisher_control.hpp"
#include "publisher.hpp"

class Message;

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
    boost::function<void (boost::shared_ptr<Message>)> fct = boost::bind<void>(&Publisher::callback, y, _1);
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


