#ifndef __SUBSCRIBER_HPP__
#define __SUBSCRIBER_HPP__

#include <signal.h>
#include <vector>
#include <algorithm>
#include <boost/lambda/lambda.hpp>
#include <boost/bind.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/thread.hpp>
#include <boost/log/trivial.hpp>
#include <boost/function.hpp>
#include <signal.h>
//#include "google/heap-checker.h"
#include "mongo/client/dbclient.h"
#include "mongo/client/dbclient.h"
#include "zhelpers.hpp"
#include "message.hpp"
#include "writer.hpp"
#include "daily_call.hpp"
#include "param_helper.hpp"
#include "logger_config.hpp"


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
        Subscriber(boost::shared_ptr<AbstractWriter> writer, std::string protocol="tcp", std::string host="localhost", int port=5563, int queue=2);

        void operator()(); 

        message_type_t parse_msg(std::string& msg) ;

        void drain_queue() ;
        

};

class SubControl {
    private:
        boost::shared_ptr<Subscriber> _s;
        void sigill();
        void sigint();

    public:
        SubControl(boost::shared_ptr<Subscriber> s): _s(s) {}
        SubControl() {}

        void intHandler(int sig);

};

#endif
