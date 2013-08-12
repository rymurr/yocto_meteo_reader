
#ifndef _SENSOR_HPP_
#define _SENSOR_HPP_

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/function.hpp>
#include <algorithm>
#include <set>
#include <map>
#include <string>
#include <boost/thread.hpp>

#include "yocto_api.h"
#include "yocto_temperature.h"
#include "yocto_humidity.h"
#include "yocto_pressure.h"

#include "message.hpp"
#include "glog/logging.h"

#define BOOST_FILESYSTEM_VERSION 3

static void log(const std::string& msg) {
    LOG(INFO) << msg;
}

long long return_ms_from_epoch(const boost::posix_time::ptime&); 

class Sensor {
    public:
        virtual void start() {};
};

template <class T>
class TypedSensor:public Sensor {
    private:
        boost::shared_ptr<T> _sensor;
        const std::string _device, _function, _fullName;
        
        static void _callback(T *fct, const std::string& value) {
            LOG(INFO) << fct->get_friendlyName() << " == " << value;
            addToQueue(fct, value);
        }

    public:
        TypedSensor(const std::string& device, const std::string& function): _device(device), _function(function), _fullName(device + "." + function) {
        }
        virtual void start(){
            _sensor = boost::shared_ptr<T>(T::Find(_fullName));
            _sensor -> registerValueCallback(this->_callback);
           LOG(INFO) << "Callback registered for: " << _fullName ;
        } 
        static void addToQueue(T *fct, const std::string& value);
};

typedef TypedSensor<YTemperature> TemperatureSensor;
typedef TypedSensor<YHumidity> HumiditySensor;
typedef TypedSensor<YPressure> PressureSensor;

template <class T>
boost::shared_ptr<T> sensorHelper(YModule *m, std::string& fctName){
    T x(m->get_serialNumber(), fctName);
    x.start();
    return boost::make_shared<T>(x);
};

typedef boost::function<void (Message&)> readingCallback;
class SensorGroup { 

    private:
        static std::map<std::string, int> _sensors;
        static std::set<boost::shared_ptr<Sensor> > _devices;
        static boost::mutex guard;

        static void _deviceArrival(YModule *m) ;

        static void _deviceRemoval(YModule *m) {
            LOG(INFO) << "Devince removal: " << m->get_serialNumber();
        } 

        SensorGroup();
        SensorGroup(const SensorGroup&);
        void operator=(const SensorGroup&);

        static std::vector<readingCallback> _callbacks;
    public:
        static SensorGroup& getInstance() {
            static SensorGroup s;
            return s;
        }

        void deviceRemoval(YModule *m) {
            LOG(INFO) << "Devince removal: " << m->get_serialNumber();
        }
        static void addToQueue(Message &r) {
            boost::mutex::scoped_lock(guard);
            //for_each(_callbacks.begin(), _callbacks.end(), boost::bind<void>(&readingCallback::operator(),_1,r));
            for (int i=0;i<_callbacks.size();++i) {
                _callbacks[i](r);
            }
        }

        int start();

        void addCallback(readingCallback &x) {
            _callbacks.push_back(x);
        }


};

#endif
