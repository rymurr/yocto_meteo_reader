#include "publisher_control.hpp"

void PubControl::sigint(){
    BOOST_LOG_TRIVIAL(fatal) << "Recieved sigint! Goodbye!";
    exit(1);
};

void PubControl::intHandler(int sig) {
    switch(sig) {
        case 2:
            sigint();
            break;
        default:
            BOOST_LOG_TRIVIAL(error) << "Unable to handle signal!";
    }
};


