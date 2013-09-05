#include <boost/log/trivial.hpp>
#include "sub_control.hpp"
#include "subscriber.hpp"

void SubControl::sigint(){
    BOOST_LOG_TRIVIAL(fatal) << "Recieved sigint! Goodbye!";
    _s->drain_queue();
    exit(1);
};

void SubControl::intHandler(int sig) {
    switch(sig) {
        case 2:
            sigint();
            break;
        default:
            BOOST_LOG_TRIVIAL(error) << "Unable to handle signal!";
    }
};

