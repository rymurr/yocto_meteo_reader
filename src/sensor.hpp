
#ifndef _SENSOR_HPP_
#define _SENSOR_HPP_

#include "yocto_api.h"
#include "yocto_temperature.h"
#include "yocto_humidity.h"
#include "yocto_pressure.h"

#include "glog/logging.h"

static void log(const std::string& msg) {
    LOG(INFO) << msg;
}

static void anButtonChangeCallBack(YHumidity *fct, const string& value)
{
    LOG(INFO) << "Humidity change    :" << fct->describe() << " = " <<value;
}

static void temperatureChangeCallBack(YTemperature *fct, const string& value)
{
    LOG(INFO) << "Temperature change :" << fct->describe() << " = " <<value ;
}

static void lightSensorChangeCallBack(YPressure *fct, const string& value)
{
    LOG(INFO) << "Pressure change       :" << fct->describe() << " = " << value << "lx";
}


static void deviceArrival(YModule *m)
{
    LOG(INFO) << "Device arrival          : " << m->describe() ;
    int fctcount = m->functionCount();
    string fctName, fctFullName;

    for (int i = 0; i < fctcount; i++) {
        fctName = m->functionId(i);
        fctFullName = m->get_serialNumber() + "." + fctName;

        // register call back for anbuttons
        if (fctName.find("humidity")==0) { 
            YHumidity *t = YHumidity::FindHumidity(fctFullName);
            t->registerValueCallback(anButtonChangeCallBack);
            LOG(INFO) << "Callback registered for : " << fctFullName;
        }

        // register call back for temperature sensors
        if (fctName.find("temperature")==0) { 
            YTemperature *t = YTemperature::FindTemperature(fctFullName);
            t->registerValueCallback(temperatureChangeCallBack);
            LOG(INFO) << "Callback registered for : " << fctFullName ;
        }

        // register call back for light sensors
        if (fctName.find("pressure")==0) { 
            YPressure *l = YPressure::FindPressure(fctFullName);
            l->registerValueCallback(lightSensorChangeCallBack);
            LOG(INFO) << "Callback registered for : " << fctFullName;
        }
        // and so on for other sensor type.....
    }
}

static void deviceRemoval(YModule *m)
{
  LOG(INFO) << "Device removal: " << m->get_serialNumber();
}

class SensorGroup { 

    public:
        SensorGroup() {
            YAPI::RegisterLogFunction(log);
            YAPI::RegisterDeviceArrivalCallback(deviceArrival);
            YAPI::RegisterDeviceRemovalCallback(deviceRemoval);
            YAPI::DisableExceptions();
    

        }

        int start() {
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

};
#endif
