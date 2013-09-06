#include "sensor.hpp"

message_type_t Sensor::_msg_type;
std::vector<readingCallback> Sensor::_callbacks = std::vector<readingCallback>();

long long return_ms_from_epoch(const boost::posix_time::ptime& pt) {
    return (pt-boost::posix_time::ptime(boost::gregorian::date(1970, boost::gregorian::Jan, 1))).total_milliseconds();
}


