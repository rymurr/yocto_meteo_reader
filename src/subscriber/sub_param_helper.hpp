#ifndef __SUB_PARAM_HELPER_HPP__
#define __SUB_PARAM_HELPER_HPP__

#include <boost/program_options.hpp>
#include <boost/log/trivial.hpp>
#include "writer.hpp"
#include "base_message.hpp"
#include "msg_operator.hpp"


namespace po = boost::program_options;

class SubscriberParams {
    private:
        po::options_description _desc;
        po::variables_map _vm;

        void set_options();
        int verify();

    public:
        SubscriberParams(): _desc("Weather Subscriber options") {
            set_options();
        }    

        int parse_options(int argc, char** argv); 
        std::string getOption();
        std::string getStorageDir();
        std::string getMongoDbHost();
        std::string getPublishHost();
        std::string getPublishHostName();
        writer_t getStorageFormat();
        message_type_t getStorageType();
        int getQueueSize();
        int getPublishPort();

};

#endif
