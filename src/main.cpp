
#include "glog/logging.h"
#include "sensor.hpp"

using namespace std;


int main(int argc, const char * argv[])
{
    google::InitGoogleLogging(argv[0]);
    return SensorGroup().start();
}
