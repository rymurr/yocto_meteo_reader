#include "sensor.hpp"

std::set<boost::shared_ptr<Sensor> > SensorGroup::_devices = std::set<boost::shared_ptr<Sensor> >();
std::map<std::string, int> SensorGroup::_sensors = boost::assign::map_list_of("temperature", 1)("humidity", 2)("pressure", 3);
std::queue<meteo::SensorReading> Sensor::_data = std::queue<meteo::SensorReading>();

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

long return_ms_from_epoch(const boost::posix_time::ptime& pt) {
    return (pt-boost::posix_time::ptime(boost::gregorian::date(1970, boost::gregorian::Dec, 1))).total_milliseconds();
}

