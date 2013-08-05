#include "sensor.hpp"

void deviceArrivalDummy(SensorGroup* grp, YModule* m) {
    std::cout << "foo" << std::endl;
    grp->deviceArrival(m);
};

SensorGroup::SensorGroup() {
    YAPI::RegisterLogFunction(log);
    boost::function<void (YModule* x)> devArr = boost::bind(deviceArrivalDummy, this, _1);
    YAPI::RegisterDeviceArrivalCallback(*devArr.target<void(*)(YModule*)>());
    std::cout << "foo" << std::endl;
    YAPI::RegisterDeviceRemovalCallback(this->_deviceRemoval);
    std::cout << "foo" << std::endl;
    YAPI::DisableExceptions();
    std::cout << "foo" << std::endl;

    _sensors = boost::assign::map_list_of("temperature", 1)("humidity", 2)("pressure", 3);
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


