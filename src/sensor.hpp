
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
#include <boost/log/trivial.hpp>

#include "yocto_api.h"
#include "yocto_temperature.h"
#include "yocto_humidity.h"
#include "yocto_pressure.h"

#include "message.hpp"

#define BOOST_FILESYSTEM_VERSION 3

static void log(const std::string& msg) {
    BOOST_LOG_TRIVIAL(info) << msg;
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
            BOOST_LOG_TRIVIAL(info) << fct->get_friendlyName() << " == " << value;
            addToQueue(fct, value);
        }

    public:
        TypedSensor(const std::string& device, const std::string& function): _device(device), _function(function), _fullName(device + "." + function) {
        }
        virtual void start(){
            _sensor = boost::shared_ptr<T>(T::Find(_fullName));
            _sensor -> registerValueCallback(this->_callback);
           BOOST_LOG_TRIVIAL(info) << "Callback registered for: " << _fullName ;
        } 

        /*  TODO
         *  this is the only function where MongoMessage is directly called on publish side.
         *  Have to add config/overload to TypedSensor to make message type more pluggabled$a"message_"
         */
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
        static std::set<std::string> _allowed_devices;
        static std::set<std::string> _allowed_sensors;
        static boost::mutex guard;

        static void _deviceArrival(YModule *m) ;

        static void _deviceRemoval(YModule *m) {
            BOOST_LOG_TRIVIAL(info) << "Device removal: " << m->get_serialNumber();
        } 

        SensorGroup();
        SensorGroup(const SensorGroup&);
        void operator=(const SensorGroup&);

        static std::vector<readingCallback> _callbacks;
    public:
        static message_type_t _msg_type;
        static SensorGroup& getInstance() {
            static SensorGroup s;
            return s;
        }

        static void setMsgType(message_type_t m) { _msg_type = m;}
        void deviceRemoval(YModule *m) {
            BOOST_LOG_TRIVIAL(info) << "Device removal: " << m->get_serialNumber();
        }
        static void addToQueue(boost::shared_ptr<Message> r) {
            boost::mutex::scoped_lock(guard);
            //for_each(_callbacks.begin(), _callbacks.end(), boost::bind<void>(&readingCallback::operator(),_1,r));
            for (int i=0;i<_callbacks.size();++i) {
                _callbacks[i](*r);
            }
        }

        int start();

        void addCallback(readingCallback &x) {
            _callbacks.push_back(x);
        }

        void addAllowedSensor(std::string x) {
            _allowed_sensors.insert(x);
        }

        void addAllowedDevice(std::string x) {
            _allowed_devices.insert(x);
        }


};

#endif
