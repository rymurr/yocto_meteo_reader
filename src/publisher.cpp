
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>

#include "glog/logging.h"
#include "sensor.hpp"
#include "zhelpers.hpp"

class foo {

    private:
        zmq::context_t _context;
        zmq::socket_t _publisher;
        
    public:
        foo():_context(1), _publisher(_context, ZMQ_PUB) {
             _publisher.bind("tcp://*:5563");
             s_sendmore(_publisher, "CONTROL");
             s_send(_publisher, "hello!");
        }
        void callback(meteo::SensorReading& x) {
             s_sendmore(_publisher,x.sensor());
             s_send(_publisher, x.SerializeAsString());
             sleep(1);
        }
};

class bar {
    public:
        void callback(meteo::SensorReading& x, boost::shared_ptr<foo> y) {
            y->callback(x);
        }
};
int main(int argc, const char * argv[])
{
    google::InitGoogleLogging(argv[0]);
    boost::shared_ptr<foo> y = boost::make_shared<foo>(foo());
    bar x;
    boost::function<void (meteo::SensorReading&)> fct = boost::bind<void>(&bar::callback, x, _1, y);
    SensorGroup::getInstance().addCallback(fct);
    SensorGroup::getInstance().start();
}
