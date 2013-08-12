#include <vector>
#include <algorithm>
#include <boost/lambda/lambda.hpp>
#include <boost/bind.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <signal.h>
#include "mongo/client/dbclient.h"
#include "zhelpers.hpp"
#include "message.hpp"


class Subscriber {

    private:
        zmq::context_t _context;
        zmq::socket_t _subscriber;

    public:
        Subscriber(std::string protocol="tcp://", std::string host="localhost:5563"): _context(1), _subscriber(_context, ZMQ_SUB) {
            _subscriber.connect(protocol.append(host).c_str());
            _subscriber.setsockopt( ZMQ_SUBSCRIBE, "", 0);
        }   

};


int main () {
    //  Prepare our context and subscriber
    int x(0);
    boost::filesystem::path p("/data/meteo/storage");
        std::vector<mongo::BSONObj> bson_queue;

    while (1) {

        //  Read envelope with address
        //std::string address = s_recv (subscriber);
        //  Read message contents
        //mongo::BSONObj r  = s_recvobj (subscriber);
        //bson_queue.push_back(r.copy());
        //std::cout << r.jsonString() << std::endl;
//        if (bson_queue.size() > 2) {
//            x = drain_to_file(bson_queue, boost::filesystem::path("data"), "meteo", x);
//            drain_to_mongo(bson_queue);
//        }
    }
    return 0;
}
