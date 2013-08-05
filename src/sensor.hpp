
#ifndef _SENSOR_HPP_
#define _SENSOR_HPP_

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <set>
#include <map>
#include <string>

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

static void lightSensorChangeCallBack(YPressure *fct, const string& value)
{
    LOG(INFO) << "Pressure change       :" << fct->describe() << " = " << value << "lx";
}

class Sensor {
    public:
        virtual void start() = 0;
};

template <class T>
class TypedSensor:public Sensor {
    private:
        boost::shared_ptr<T> _sensor;
        const std::string _device, _function, _fullName;
        
        static void _callback(T *fct, const std::string& value) {
            LOG(INFO) << "Temp: " << fct->describe() << " == " << value;
        }

    public:
        TypedSensor(const std::string& device, const std::string& function): _device(device), _function(function), _fullName(device + "." + function) {
        }
        virtual void start(){
            _sensor = boost::shared_ptr<T>(T::Find(_fullName));
            _sensor -> registerValueCallback(this->_callback);
           LOG(INFO) << "Callback registered for: " << _fullName ;
        } 

};

class TemperatureSensor: public TypedSensor<YTemperature> {
    public:
        TemperatureSensor(const std::string& device, const std::string& function):TypedSensor(device, function){}
};

class SensorGroup { 

    private:
        std::map<std::string, int> _sensors;
        static std::set<boost::shared_ptr<Sensor> > _devices;

        static void _deviceArrival(YModule *m) {
            LOG(INFO) << "Device arrival: " << m->describe() ;
            int fctcount = m->functionCount();
            std::string fctName, fctFullName;
        
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
                    TemperatureSensor x(m->get_serialNumber(), fctName);
                    x.start();
                    _devices.insert(boost::make_shared<TemperatureSensor>(x));

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

        static void _deviceRemoval(YModule *m) {
            LOG(INFO) << "Devince removal: " << m->get_serialNumber();
        }

    public:
        SensorGroup();

        int start();

};

void deviceArrivalDummy(SensorGroup*, YModule*) ;


#endif
