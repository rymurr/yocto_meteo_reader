
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

typedef TypedSensor<YTemperature> TemperatureSensor;
typedef TypedSensor<YHumidity> HumiditySensor;
typedef TypedSensor<YPressure> PressureSensor;


static void log(const std::string& msg) {
    LOG(INFO) << msg;
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

template <class T>
boost::shared_ptr<T> sensorHelper(YModule *m, std::string& fctName){
    T x(m->get_serialNumber(), fctName);
    x.start();
    return boost::make_shared<T>(x);
};

class SensorGroup { 

    private:
        static std::map<std::string, int> _sensors;
        static std::set<boost::shared_ptr<Sensor> > _devices;


        static void _deviceArrival(YModule *m) {
            LOG(INFO) << "Device arrival: " << m->describe() ;
            int fctcount = m->functionCount();
        
            for (int i = 0; i < fctcount; i++) {
                std::string fctName = m->functionId(i);
                std::string fctFullName = m->get_serialNumber() + "." + fctName;

                std::map<std::string, int>::const_iterator it = _sensors.find(fctName);
                if (it==_sensors.end()) {
                    LOG(ERROR) << "Could not find sensor! " << fctFullName;
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
                // register call back for anbuttons

                LOG(INFO) << "Callback registered for : " << fctFullName;
            }
        }

        static void _deviceRemoval(YModule *m) {
            LOG(INFO) << "Devince removal: " << m->get_serialNumber();
        }

    public:
        SensorGroup();

        int start();

};

#endif
