
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
    else if (token == "json")
        unit = JSON;
    else if (token == "protobuf")
        unit = PROTOBUF;
    return in;
}

void PublisherParams::set_options() {
    _desc.add_options()
        ("help", "produce help message")
        ("msg-type,m", po::value<message_type_t>()->default_value(BSON), "message type")
        ("port,p", po::value<int>()->default_value(5563), "Publish port")
        ("types,t", po::value<std::vector<std::string> >()->composing(), "sensor types")
        ("devices,d", po::value<std::vector<std::string> >()->composing(), "device names")
        ;
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
        ("storage-format,f", po::value<message_type_t>()->default_value(JSON))
    ;
}

message_type_t SubscriberParams::getStorageType() {
    return _vm["storage-format"].as<message_type_t>();
}

message_type_t PublisherParams::getMessageType(){
    return _vm["msg-type"].as<message_type_t>();
}

int PublisherParams::getPort(){
    return _vm["port"].as<int>();
}

std::vector<std::string> PublisherParams::getSensorTypes() {
    if (_vm.count("types"))
        return _vm["types"].as<std::vector<std::string> >();
    return std::vector<std::string>();
}

std::vector<std::string> PublisherParams::getDevices() {
    if (_vm.count("devices"))
        return _vm["devices"].as<std::vector<std::string> >();
    return std::vector<std::string>();
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
