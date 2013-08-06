
#include <boost/bind.hpp>

#include "glog/logging.h"
#include "sensor.hpp"

class foo {

    public:
        void callback(meteo::SensorReading& x) {
            std::cout << "foo" << std::endl;
        }
};

int main(int argc, const char * argv[])
{
    google::InitGoogleLogging(argv[0]);
    foo x;
    boost::function<void (meteo::SensorReading&)> fct = boost::bind<void>(&foo::callback, x, _1);
    SensorGroup::getInstance().addCallback(fct);
    SensorGroup::getInstance().start();
}
