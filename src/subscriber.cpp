#include <queue>
#include "zhelpers.hpp"
#include "meteo.pb.h"

enum {WEEKLY, PERSISTENT};

int main () {
    //  Prepare our context and subscriber
    zmq::context_t context(1);
    zmq::socket_t subscriber (context, ZMQ_SUB);
    subscriber.connect("tcp://localhost:5563");
    subscriber.setsockopt( ZMQ_SUBSCRIBE, "", 0);

    while (1) {

        //  Read envelope with address
        std::string address = s_recv (subscriber);
        //  Read message contents
        std::string contents = s_recv (subscriber);
        meteo::SensorReading r;
        r.ParseFromString(contents);
        std::cout << r.device() << " " << r.sensor() << " " << r.value() << " " << r.timestamp() << std::endl;
    }
    return 0;
}
