
#ifndef _SENSOR_GROUP_HPP_
#define _SENSOR_GROUP_HPP_

#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/assign/list_of.hpp>
#include <set>
#include <map>
#include <string>
#include <boost/log/trivial.hpp>

#include "base_message.hpp"
#include "yocto_api.h"


#define BOOST_FILESYSTEM_VERSION 3

class Message;
class Sensor;

typedef boost::function<void (boost::shared_ptr<Message>)> readingCallback;

template <class T>
boost::shared_ptr<T> sensorHelper(YModule *m, std::string& fctName){
    boost::shared_ptr<T> x(new T(m->get_serialNumber(), fctName));
    x->start();
    return x;
};


class SensorGroup { 

    private:
        static std::map<std::string, int> _sensors;
        static std::set<boost::shared_ptr<Sensor> > _devices;
        static std::set<std::string> _allowed_devices;
        static std::set<std::string> _allowed_sensors;

        static void _deviceArrival(YModule *m) ;

        static void _deviceRemoval(YModule *m) {
            BOOST_LOG_TRIVIAL(info) << "Device removal: " << m->get_serialNumber();
        } 

        SensorGroup();
        SensorGroup(const SensorGroup&);
        void operator=(const SensorGroup&);

    public:
        static SensorGroup& getInstance() {
            static SensorGroup s;
            return s;
        }

        static void setMsgType(message_type_t m) ;
        void deviceRemoval(YModule *m) {
            BOOST_LOG_TRIVIAL(info) << "Device removal: " << m->get_serialNumber();
        }

        int start();

        void addCallback(readingCallback &x) ;

        void addAllowedSensor(std::string x) {
            _allowed_sensors.insert(x);
        }

        void addAllowedDevice(std::string x) {
            _allowed_devices.insert(x);
        }


};

#endif 
