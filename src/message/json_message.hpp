#ifndef _JSON_MESSAGE_HPP_
#define _JSON_MESSAGE_HPP_

#include "base_message.hpp"
#include <boost/property_tree/ptree.hpp>

class JSONMessage: public Message {
    private:
        boost::property_tree::ptree _obj;
        
    public:
        JSONMessage(std::string& device, std::string& sensor, long long timestamp, double value) ;

        JSONMessage(char* data);

        virtual msgPtr fillmessage() ;

        virtual std::string id() ;

        std::string device() ;

        double value() ;

        long long timestamp() ;

        virtual std::string string() ;
        
};

#endif
