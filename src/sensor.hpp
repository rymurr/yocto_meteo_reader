
#ifndef _SENSOR_HPP_
#define _SENSOR_HPP_

#include <boost/shared_ptr.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/log/trivial.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

#include "yocto_temperature.h"
#include "yocto_humidity.h"
#include "yocto_pressure.h"

#include "message.hpp"

#define BOOST_FILESYSTEM_VERSION 3

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


#endif
