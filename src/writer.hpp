#ifndef __WRITER_HPP__
#define __WRITER_HPP__
#include <vector>
#include <algorithm>
#include <boost/lambda/lambda.hpp>
#include <boost/bind.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/log/trivial.hpp>
#include <boost/thread.hpp>
#ifdef MONGO
#include "mongo/client/dbclient.h"
#endif
#include "message.hpp"

enum writer_t {MONGO, FILEDIR_WEEKLY, FILEDIR_PERSISTENT};

typedef std::vector<boost::shared_ptr<Message> > msgArr;
class AbstractWriter {
    public:
        virtual int drain(msgArr) = 0;
        virtual void clear() = 0;
        virtual void setMsgType(message_type_t) = 0;
};

typedef boost::shared_ptr<AbstractWriter> ptrWriter; 

class WriterBuilder {
    public:
        static ptrWriter create(writer_t writer_type, std::string option);
};

class MongoWriter: public AbstractWriter {
    private: 
        const std::string _hostname;
        message_type_t _msg_type;
        boost::mutex guard;

    public:
        MongoWriter(std::string& hostname): _hostname(hostname), _msg_type(INVALID){}
    
        virtual void setMsgType(message_type_t msg_type) { _msg_type = msg_type;}
        virtual int drain(msgArr);
        virtual void clear(){};


};

class DiskWriter: public AbstractWriter {
    private:
        const std::string _file_prefix;
        message_type_t _msg_type;

        int find_current_count();
        
    protected:    
        std::size_t drain_queue_to_file(msgArr bson_queue, const boost::filesystem::path &p); 
        int _count;
        const boost::filesystem::path _p;
        boost::mutex guard;

    public:
        DiskWriter(std::string filepath): _file_prefix("meteo"), _msg_type(INVALID), _p(filepath) {
            _count = find_current_count();
        }    

        virtual void setMsgType(message_type_t t) { 
            _msg_type = t;
            BOOST_LOG_TRIVIAL(info) << "Message type set to: " << t;
        }
        virtual int drain(msgArr);
};

class PersistentDiskWriter: public DiskWriter {
    public:
        PersistentDiskWriter(std::string filepath): DiskWriter(filepath){}
        virtual void clear();
};

class WeeklyRotateWriter: public DiskWriter {
    public:
        WeeklyRotateWriter(std::string filepath): DiskWriter(filepath){}
        virtual void clear();
    private:
        void rotate();    
        
};

#endif
