
#include "sensor_group.hpp"
message_type_t SensorGroup::_msg_type;
std::set<boost::shared_ptr<Sensor> > SensorGroup::_devices = std::set<boost::shared_ptr<Sensor> >();
std::set<std::string> SensorGroup::_allowed_devices = std::set<std::string>();
std::set<std::string> SensorGroup::_allowed_sensors = std::set<std::string>();
std::map<std::string, int> SensorGroup::_sensors = boost::assign::map_list_of("temperature", 1)("humidity", 2)("pressure", 3);
std::vector<readingCallback> SensorGroup::_callbacks = std::vector<readingCallback>();

static void log(const std::string& msg) {
    BOOST_LOG_TRIVIAL(info) << msg;
}

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

void SensorGroup::addToQueue(boost::shared_ptr<Message> r) {
    boost::mutex::scoped_lock(guard);
    //for_each(_callbacks.begin(), _callbacks.end(), boost::bind<void>(&readingCallback::operator(),_1,r));
    for (int i=0;i<_callbacks.size();++i) {
        _callbacks[i](*r);
    }
}

