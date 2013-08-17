#include <vector>
#include <algorithm>
#include <boost/lambda/lambda.hpp>
#include <boost/bind.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/thread.hpp>
#include <signal.h>
#include "mongo/client/dbclient.h"
#include "zhelpers.hpp"
#include "message.hpp"
#include "writer.hpp"


class Subscriber {

    private:
        zmq::context_t _context;
        zmq::socket_t _subscriber;
        zmq::socket_t _sync;
        message_type_t _msg_type;
        std::vector<boost::shared_ptr<Message> > _bson_queue;
        boost::mutex guard;
        boost::shared_ptr<AbstractWriter> _writer;
        std::size_t _max;

    public:
        Subscriber(boost::shared_ptr<AbstractWriter> writer, std::string protocol="tcp", std::string host="localhost", int port=5563): _context(1), _subscriber(_context, ZMQ_SUB), _sync(_context, ZMQ_REQ), _writer(writer), _max(2) {
            _subscriber.connect(connect_name(protocol, host, port).c_str());
            _subscriber.setsockopt( ZMQ_SUBSCRIBE, "", 0);
            _sync.connect(connect_name(protocol, host, port+1).c_str());
            s_send(_sync,"");
            std::string conf_string = s_recv(_sync);
            _msg_type = parse_msg(conf_string);
        }   

        void start() {
            while (1) {
                std::string address = s_recv(_subscriber);
                boost::shared_ptr<Message> r = s_recvobj(_subscriber, _msg_type);
                std::cout << r->string() << std::endl;
                _bson_queue.push_back(r);
                if (_bson_queue.size() > _max) {
                    drain_queue();
                }

            }
        }

        message_type_t parse_msg(std::string& msg) {
            boost::property_tree::ptree pt;
            std::istringstream is(msg);
            boost::property_tree::read_json(is, pt);
            return static_cast<message_type_t>(pt.get<int>("type"));
        }

        void drain_queue() {
            boost::mutex::scoped_lock(guard);
            _writer->drain(_bson_queue);
            _bson_queue.clear();

        }

};


int main () {
    //  Prepare our context and subscriber
    int x(0);
    boost::filesystem::path p("/data/meteo/storage");
        std::vector<mongo::BSONObj> bson_queue;

    Subscriber s(WriterBuilder::create(writer_t::FILEDIR_PERSISTENT, "localhost", message_type_t::BSON));
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
