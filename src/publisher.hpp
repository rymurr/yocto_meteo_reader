#ifndef _PUBLISHER_HPP_
#define _PUBLISHER_HPP_

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <sstream>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/common.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/utility/empty_deleter.hpp>

#include "sensor.hpp"
#include "zhelpers.hpp"
#include "message.hpp"
#include "param_helper.hpp"


void threaded_rep(std::string, std::string);

class Publisher{

    private:
        zmq::context_t _context;
        zmq::socket_t _publisher;
        boost::thread _rep_thread;
        
    public:
        Publisher(std::string hostname="*", int port=5563, std::string protocol="tcp", message_type_t msg_type=BSON):_context(1),
                             _publisher(_context, ZMQ_PUB){
             BOOST_LOG_TRIVIAL(info) << "Starting Publusher on port " << port;                                
             _publisher.bind(connect_name(protocol, hostname, port).c_str());
             std::string msg = make_msg(msg_type);
             startThread(protocol, hostname, port+1, msg);
        }
        void callback(Message& x) {
             s_sendmore(_publisher,x.id());
             s_sendobj(_publisher, x);
             sleep(1);
        }

        void startThread(std::string& protocol, std::string& hostname, int port, std::string msg) {
            BOOST_LOG_TRIVIAL(info) << "Starting Req/Rep listener with reply message: " << msg << " on port " << port;
            _rep_thread = boost::thread(threaded_rep, connect_name(protocol, hostname, port), msg);         
            _rep_thread.detach();
        }

        std::string make_msg(message_type_t);

};


#endif 
