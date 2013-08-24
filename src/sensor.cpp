#include "sensor.hpp"

std::set<boost::shared_ptr<Sensor> > SensorGroup::_devices = std::set<boost::shared_ptr<Sensor> >();
std::set<std::string> SensorGroup::_allowed_devices = std::set<std::string>();
std::set<std::string> SensorGroup::_allowed_sensors = std::set<std::string>();
std::map<std::string, int> SensorGroup::_sensors = boost::assign::map_list_of("temperature", 1)("humidity", 2)("pressure", 3);
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
        BOOST_LOG_TRIVIAL(error) << "RegisterHub error : " << errmsg;
        return 1;
    }

    while (true) {
        YAPI::UpdateDeviceList(errmsg); // traps plug/unplug events
        YAPI::Sleep(500, errmsg);   // traps others events
    } 
    return 0;
}

long long return_ms_from_epoch(const boost::posix_time::ptime& pt) {
    return (pt-boost::posix_time::ptime(boost::gregorian::date(1970, boost::gregorian::Dec, 1))).total_milliseconds();
}

template <class T>
void TypedSensor<T>::addToQueue(T *fct, const std::string& value) {
    std::vector<std::string> strs;
    const std::string name = fct->get_friendlyName();
    boost::split(strs, name, boost::is_any_of("."));
    boost::posix_time::ptime now = boost::posix_time::microsec_clock::universal_time();

    MongoMessage reading(strs[0], strs[1], return_ms_from_epoch(now), boost::lexical_cast<double>(value));
    SensorGroup::addToQueue(reading);
}

void SensorGroup::_deviceArrival(YModule *m) {
    BOOST_LOG_TRIVIAL(info) << "Device arrival: " << m->describe() ;
    int fctcount = m->functionCount();

    std::set<std::string>::const_iterator dit = _allowed_devices.find(m->get_serialNumber());
    if (!_allowed_devices.empty() && dit==_allowed_devices.end()) {
        BOOST_LOG_TRIVIAL(warning) << "Device not in list! Ignoring: " << m->get_serialNumber();            
        return;
    }
    for (int i = 0; i < fctcount; i++) {
        std::string fctName = m->functionId(i);
        std::string fctFullName = m->get_serialNumber() + "." + fctName;

        std::map<std::string, int>::const_iterator it = _sensors.find(fctName);
        if (it==_sensors.end()) {
            BOOST_LOG_TRIVIAL(error) << "Could not find sensor! " << fctFullName;
            continue;
        }
        std::set<std::string>::const_iterator sit = _allowed_sensors.find(fctName);
        if (!_allowed_sensors.empty() && sit==_allowed_sensors.end()) {
            BOOST_LOG_TRIVIAL(warning) << "Sensor not in list! Ignoring: " << fctName;
            continue;
        }

        const int sensorVal = _sensors[fctName];
        switch(sensorVal) {
            case 1:
                _devices.insert(sensorHelper<TemperatureSensor>(m, fctName));
                break;
            case 2:
                _devices.insert(sensorHelper<HumiditySensor>(m, fctName));
                break;
            case 3:
                 _devices.insert(sensorHelper<PressureSensor>(m, fctName));
                break;
        }
    }
}

