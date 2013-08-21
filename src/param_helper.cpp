
#include "param_helper.hpp"

std::istream& operator>>(std::istream& in, writer_t& unit)
{
    std::string token;
    in >> token;
    if (token == "mongo")
        unit = MONGO;
    else if (token == "weekly")
        unit = FILEDIR_WEEKLY;
    else if (token == "persistent")
        unit = FILEDIR_PERSISTENT;        
    return in;
}

std::istream& operator>>(std::istream& in, message_type_t& unit)
{
    std::string token;
    in >> token;
    if (token == "bson")
        unit = BSON;
    return in;
}

void PublisherParams::set_options() {

}

void SubscriberParams::set_options() {
    //int for number in queue
    //enum for message type
    //enum for storage type
    _desc.add_options()
        ("help", "produce help message")
        ("queue-size,q", po::value<int>()->default_value(100), "set size of in memory queue")
//        ("msg-type,m", po::value<message_type_t>()->default_value(BSON), "message type")
        ("storage-type,t", po::value<writer_t>()->default_value(FILEDIR_PERSISTENT), "stroage format")
        ("storage-directory,s", po::value<std::string>()->default_value("."), "on disk storage location")
        ("mongo-host", po::value<std::string>()->default_value("localhost"), "Mongodb server location")
        ("mongo-port", po::value<std::string>()->default_value("27017"), "Mongo server port")
        ("port,p", po::value<std::string>()->default_value("5563"), "Server port")
        ("hostname,h", po::value<std::string>()->default_value("localhost"), "Server host")
    ;
}

int PublisherParams::verify() {
    return 0;
}

int SubscriberParams::verify() {
    return 0;
}

int SubscriberParams::parse_options(int argc, char** argv) {
    po::store(po::parse_command_line(argc, argv, _desc), _vm);
    po::notify(_vm);

    if (_vm.count("help")) {
        std::cout << _desc << std::endl;
        return 1;
    }
    return verify();
}

int PublisherParams::parse_options(int argc, char** argv) {
    po::store(po::parse_command_line(argc, argv, _desc), _vm);
    po::notify(_vm);

    if (_vm.count("help")) {
        std::cout << _desc << std::endl;
        return 1;
    }
    return 0;

}
