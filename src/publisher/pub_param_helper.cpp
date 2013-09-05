
#include "pub_param_helper.hpp"

void PublisherParams::set_options() {
    _desc.add_options()
        ("help", "produce help message")
        ("msg-type,m", po::value<message_type_t>()->default_value(BSON), "message type")
        ("port,p", po::value<int>()->default_value(5563), "Publish port")
        ("types,t", po::value<std::vector<std::string> >()->composing(), "sensor types")
        ("devices,d", po::value<std::vector<std::string> >()->composing(), "device names")
        ;
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

int PublisherParams::parse_options(int argc, char** argv) {
    po::store(po::parse_command_line(argc, argv, _desc), _vm);
    po::notify(_vm);

    if (_vm.count("help")) {
        std::cout << _desc << std::endl;
        return 1;
    }
    return 0;

}
