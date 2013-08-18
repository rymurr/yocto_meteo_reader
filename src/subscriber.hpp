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
#include "daily_call.hpp"


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
        Subscriber(boost::shared_ptr<AbstractWriter> writer, std::string protocol="tcp", std::string host="localhost", int port=5563);

        void operator()(); 

        message_type_t parse_msg(std::string& msg) ;

        void drain_queue() ;
        

};


