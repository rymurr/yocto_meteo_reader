#include <queue>
#include "mongo/client/dbclient.h"
#include "zhelpers.hpp"

enum {WEEKLY, PERSISTENT};

static mongo::BSONObj
s_recvobj (zmq::socket_t & socket) {

    zmq::message_t message;
    socket.recv(&message);

    return mongo::BSONObj(static_cast<char*>(message.data()));
}

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
        mongo::BSONObj r  = s_recvobj (subscriber);
        std::cout << r.jsonString() << std::endl;
    }
    return 0;
}
