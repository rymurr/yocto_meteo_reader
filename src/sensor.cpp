#include "sensor.hpp"

long long return_ms_from_epoch(const boost::posix_time::ptime& pt) {
    return (pt-boost::posix_time::ptime(boost::gregorian::date(1970, boost::gregorian::Dec, 1))).total_milliseconds();
}

template <class T>
void TypedSensor<T>::addToQueue(T *fct, const std::string& value) {
    std::vector<std::string> strs;
    const std::string name = fct->get_friendlyName();
    boost::split(strs, name, boost::is_any_of("."));
    boost::posix_time::ptime now = boost::posix_time::microsec_clock::universal_time();

    boost::shared_ptr<Message> reading = make_message(strs[0], strs[1], return_ms_from_epoch(now), boost::lexical_cast<double>(value), SensorGroup::_msg_type);
    SensorGroup::addToQueue(reading);
}


