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
        zmq::socket_t _sync;
        std::string _conf_string;

    public:
        Subscriber(std::string protocol="tcp", std::string host="localhost", int port=5563): _context(1), _subscriber(_context, ZMQ_SUB), _sync(_context, ZMQ_REQ) {
            _subscriber.connect(connect_name(protocol, host, port).c_str());
            _subscriber.setsockopt( ZMQ_SUBSCRIBE, "", 0);
            _sync.connect(connect_name(protocol, host, port+1).c_str());

            std::cout << "foo" << std::endl;
            s_send(_sync,"");
            _conf_string = s_recv(_sync);
            std::cout << _conf_string << std::endl;
        }   

        void start() {
            while (1) {
                std::string address = s_recv(_subscriber);
                std::cout << "address" << std::endl;
                mongo::BSONObj r = s_recvobj(_subscriber);
                std::cout << r.jsonString() << std::endl;
            }
        }

};


int main () {
    //  Prepare our context and subscriber
    int x(0);
    boost::filesystem::path p("/data/meteo/storage");
        std::vector<mongo::BSONObj> bson_queue;

    Subscriber s;
    s.start();
/*    while (1) {

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
    }*/
    return 0;
}
