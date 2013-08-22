
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
    _desc.add_options()
        ("help", "produce help message")
        ("queue-size,q", po::value<int>()->default_value(100), "set size of in memory queue")
//        ("msg-type,m", po::value<message_type_t>()->default_value(BSON), "message type")
        ("storage-type,t", po::value<writer_t>()->default_value(FILEDIR_PERSISTENT), "stroage format")
        ("storage-directory,s", po::value<std::string>()->default_value("."), "on disk storage location")
        ("mongo-host", po::value<std::string>()->default_value("localhost"), "Mongodb server location")
        ("mongo-port", po::value<int>()->default_value(27017), "Mongo server port")
        ("port,p", po::value<int>()->default_value(5563), "Server port")
        ("hostname,h", po::value<std::string>()->default_value("localhost"), "Server host")
    ;
}

int PublisherParams::verify() {
    return 0;
}

int SubscriberParams::verify() {
    return 0;
}

int SubscriberParams::getQueueSize() {
    return _vm["queue-size"].as<int>();
}

int SubscriberParams::getPublishPort(){
    return _vm["port"].as<int>();
}

std::string SubscriberParams::getPublishHostName(){
    return _vm["hostname"].as<std::string>();
}

std::string SubscriberParams::getOption(){
    if (_vm["storage-type"].as<writer_t>() == MONGO) {
        return getMongoDbHost();
    } else {
        return getStorageDir();
    }
}
std::string SubscriberParams::getPublishHost() {
    std::string host = _vm["hostname"].as<std::string>();
    int port = _vm["port"].as<int>();
    return host.append(":").append(std::to_string(port));
}

std::string SubscriberParams::getStorageDir() {
    return _vm["storage-directory"].as<std::string>();
}

writer_t SubscriberParams::getStorageFormat() {
    return _vm["storage-type"].as<writer_t>();
}

std::string SubscriberParams::getMongoDbHost() {
    std::string host = _vm["mongo-host"].as<std::string>();
    int port = _vm["mongo-port"].as<int>();
    return host.append(":").append(std::to_string(port));
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
