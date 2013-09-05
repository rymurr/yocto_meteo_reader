#ifndef _DISK_WRITER_HPP_
#define _DISK_WRITER_HPP_

#include <string>
#include <boost/filesystem.hpp>
#include <boost/log/trivial.hpp>
#include <boost/thread.hpp>
#include "base_writer.hpp"
#include "base_message.hpp"

class DiskWriter: public AbstractWriter {
    private:
        const std::string _file_prefix;
        message_type_t _msg_type;
        message_type_t _msg_type_store;

        
    protected:    
        int find_current_count();
        std::size_t drain_queue_to_file(msgArr bson_queue, const boost::filesystem::path &p); 
        int _count;
        const boost::filesystem::path _p;
        boost::mutex guard;

    public:
        DiskWriter(std::string filepath): _file_prefix("meteo"), _msg_type(INVALID), _p(filepath) {
            _count = find_current_count();
        }    

        virtual void setMsgTypeStore(message_type_t msg_type) { _msg_type_store = msg_type;}
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


#endif 

