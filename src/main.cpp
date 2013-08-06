
#include "glog/logging.h"
#include "sensor.hpp"

int main(int argc, const char * argv[])
{
    google::InitGoogleLogging(argv[0]);
    return SensorGroup::getInstance().start();
}
