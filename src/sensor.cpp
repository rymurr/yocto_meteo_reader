#include "sensor.hpp"

std::set<boost::shared_ptr<Sensor> > SensorGroup::_devices = std::set<boost::shared_ptr<Sensor> >();
std::map<std::string, int> SensorGroup::_sensors = boost::assign::map_list_of("temperature", 1)("humidity", 2)("pressure", 3);
std::queue<mongo::BSONObj> SensorGroup::_data = std::queue<mongo::BSONObj>();
std::vector<readingCallback> SensorGroup::_callbacks = std::vector<readingCallback>();

SensorGroup::SensorGroup() {
    YAPI::RegisterLogFunction(log);
    YAPI::RegisterDeviceArrivalCallback(this->_deviceArrival);
    YAPI::RegisterDeviceRemovalCallback(this->_deviceRemoval);
    YAPI::DisableExceptions();

};

int SensorGroup::start() {
    std::string errmsg;
    if (YAPI::RegisterHub("usb", errmsg) != YAPI::SUCCESS) {
        LOG(ERROR) << "RegisterHub error : " << errmsg;
        return 1;
    }

    while (true) {
        YAPI::UpdateDeviceList(errmsg); // traps plug/unplug events
        YAPI::Sleep(500, errmsg);   // traps others events
    } 
    return 0;
}

double return_ms_from_epoch(const boost::posix_time::ptime& pt) {
    return ((double)(pt-boost::posix_time::ptime(boost::gregorian::date(1970, boost::gregorian::Dec, 1))).total_milliseconds())/1000.;
}

template <class T>
void TypedSensor<T>::addToQueue(T *fct, const std::string& value) {
    std::vector<std::string> strs;
    const std::string name = fct->get_friendlyName();
    boost::split(strs, name, boost::is_any_of("."));
    boost::posix_time::ptime now = boost::posix_time::microsec_clock::universal_time();
    /*meteo::SensorReading reading;
    reading.set_value(boost::lexical_cast<double>(value)); 
    reading.set_device(strs[0]);
    reading.set_sensor(strs[1]);
    reading.set_timestamp(return_ms_from_epoch(now));*/

    mongo::BSONObjBuilder b;
    b.append("value", boost::lexical_cast<double>(value));
    b.append("device", strs[0]);
    b.append("sensor", strs[1]);
    b.append("timestamp", return_ms_from_epoch(now));
    mongo::BSONObj reading = b.obj();
    SensorGroup::addToQueue(reading);
}

