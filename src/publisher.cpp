
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>

#include "mongo/client/dbclient.h"
#include "glog/logging.h"
#include "sensor.hpp"
#include "zhelpers.hpp"

//  Sends string as 0MQ string, as multipart non-terminal
static bool
s_sendobj (zmq::socket_t & socket, const mongo::BSONObj& obj) {

    zmq::message_t message(obj.objsize());
    memcpy (message.data(), obj.objdata(), obj.objsize());

    bool rc = socket.send (message);
    return (rc);
}

class foo {

    private:
        zmq::context_t _context;
        zmq::socket_t _publisher;
        
    public:
        foo():_context(1), _publisher(_context, ZMQ_PUB) {
             _publisher.bind("tcp://*:5563");
             s_sendmore(_publisher, "CONTROL");
             s_send(_publisher, "hello!");
        }
        void callback(mongo::BSONObj& x) {
             s_sendmore(_publisher,x.getStringField("sensor"));
             s_sendobj(_publisher, x);
             sleep(1);
        }
};

class bar {
    public:
        void callback(mongo::BSONObj& x, boost::shared_ptr<foo> y) {
            y->callback(x);
        }
};
int main(int argc, const char * argv[])
{
    google::InitGoogleLogging(argv[0]);
    boost::shared_ptr<foo> y = boost::make_shared<foo>(foo());
    bar x;
    boost::function<void (mongo::BSONObj&)> fct = boost::bind<void>(&bar::callback, x, _1, y);
    SensorGroup::getInstance().addCallback(fct);
    SensorGroup::getInstance().start();
}
