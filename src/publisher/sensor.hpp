
#ifndef _SENSOR_HPP_
#define _SENSOR_HPP_

#include <boost/shared_ptr.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/log/trivial.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/thread.hpp>

#include "yocto_temperature.h"
#include "yocto_humidity.h"
#include "yocto_pressure.h"

#include "message.hpp"

#define BOOST_FILESYSTEM_VERSION 3

long long return_ms_from_epoch(const boost::posix_time::ptime&); 

typedef boost::function<void (boost::shared_ptr<Message>)> readingCallback;

class Sensor {
    protected:
        static message_type_t _msg_type;
        static std::vector<readingCallback> _callbacks;
        static boost::mutex guard;
    public:
        virtual void start() {};
        static void msg_type(message_type_t msg) {_msg_type = msg;};
        static void addCallback(readingCallback &x) {
            _callbacks.push_back(x);
        }
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

        static void addToQueue(T *fct, const std::string& value) {
            std::vector<std::string> strs;
            const std::string name = fct->get_friendlyName();
            boost::split(strs, name, boost::is_any_of("."));
            boost::posix_time::ptime now = boost::posix_time::microsec_clock::universal_time();

            long long msnow = return_ms_from_epoch(now);
            boost::shared_ptr<Message> reading = make_message(strs[0], strs[1], msnow, boost::lexical_cast<double>(value), Sensor::_msg_type);
            boost::mutex::scoped_lock(guard);
            //for_each(_callbacks.begin(), _callbacks.end(), boost::bind<void>(&readingCallback::operator(),_1,r));
            for (int i=0;i<_callbacks.size();++i) {
                _callbacks[i](reading);
            }
        }


};

typedef TypedSensor<YTemperature> TemperatureSensor;
typedef TypedSensor<YHumidity> HumiditySensor;
typedef TypedSensor<YPressure> PressureSensor;


#endif
