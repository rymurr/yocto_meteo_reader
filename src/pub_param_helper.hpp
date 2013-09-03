#ifndef __PUB_PARAM_HELPER_HPP__
#define __PUB_PARAM_HELPER_HPP__

#include <boost/program_options.hpp>
#include <boost/log/trivial.hpp>
#include <vector>
#include "base_message.hpp"
#include "msg_operator.hpp"


namespace po = boost::program_options;

class PublisherParams {
    private:
        po::options_description _desc;
        po::variables_map _vm;

        void set_options();
        int verify();

    public:
        PublisherParams(): _desc("Weather Publisher options") {
            set_options();
        }    

        int parse_options(int argc, char** argv); 
        std::vector<std::string> getDevices();
        std::vector<std::string> getSensorTypes();
        message_type_t getMessageType();
        int getPort();

};

#endif
