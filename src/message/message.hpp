
#ifndef _MESSAGE_HPP_
#define _MESSAGE_HPP_

#include <sstream>
#include <zmq.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include "base_message.hpp"
boost::shared_ptr<Message> s_recvobj (zmq::socket_t & socket, message_type_t msg_type) ;
boost::shared_ptr<Message> make_message(std::string sensor, std::string device, long long timestamp, double value, message_type_t msg_type);

boost::shared_ptr<Message> convert(boost::shared_ptr<Message> m, message_type_t intype, message_type_t outtype);
boost::shared_ptr<Message> convertToMongo(boost::shared_ptr<Message> m, message_type_t mtype) ;
boost::shared_ptr<Message> convertToJSON(boost::shared_ptr<Message> m, message_type_t mtype) ;
boost::shared_ptr<Message> convertToProtoBuf(boost::shared_ptr<Message> m, message_type_t mtype) ;
#endif

