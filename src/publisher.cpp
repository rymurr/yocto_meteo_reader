#include "publisher.hpp"

int main(int argc, const char * argv[])
{
    google::InitGoogleLogging(argv[0]);
    boost::shared_ptr<Publisher> y = boost::make_shared<Publisher>(Publisher());
    boost::function<void (Message&)> fct = boost::bind<void>(&Publisher::callback, y, _1);
    SensorGroup::getInstance().addCallback(fct);
    SensorGroup::getInstance().start();
}
