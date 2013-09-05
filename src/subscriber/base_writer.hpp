#ifndef _BASE_WRITER_HPP_
#define _BASE_WRITER_HPP_

#include <vector>
#include "base_message.hpp"
#include <string>
#include <boost/shared_ptr.hpp>

enum writer_t {MONGO, FILEDIR_WEEKLY, FILEDIR_PERSISTENT};

typedef std::vector<boost::shared_ptr<Message> > msgArr;
class AbstractWriter {
    public:
        virtual int drain(msgArr) = 0;
        virtual void clear() = 0;
        virtual void setMsgType(message_type_t) = 0;
        virtual void setMsgTypeStore(message_type_t) =0;
};

typedef boost::shared_ptr<AbstractWriter> ptrWriter; 

#endif 
