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
#include "mongo/client/dbclient.h"
#include "message.hpp"

enum writer_t {MONGO, FILEDIR_WEEKLY, FILEDIR_PERSISTENT};

typedef std::vector<boost::shared_ptr<Message> > msgArr;
class AbstractWriter {
    public:
        virtual void drain(msgArr) = 0;
        virtual void clear() = 0;
};

typedef boost::shared_ptr<AbstractWriter> ptrWriter; 

class WriterBuilder {
    public:
        static ptrWriter create(writer_t writer_type, std::string option, message_type_t msg_type);
};

class MongoWriter: public AbstractWriter {
    private: 
        const std::string _hostname;
        const message_type_t _msg_type;

    public:
        MongoWriter(std::string& hostname, message_type_t msg_type): _hostname(hostname), _msg_type(msg_type){}
    
        virtual void drain(msgArr);
        virtual void clear(){};


};

class DiskWriter: public AbstractWriter {
    private:
        const std::string _file_prefix;
        const message_type_t _msg_type;

        int find_current_count();
        
    protected:    
        std::size_t drain_queue_to_file(msgArr bson_queue, const boost::filesystem::path &p); 
        int _count;
        const boost::filesystem::path _p;

    public:
        DiskWriter(std::string filepath, message_type_t msg_type): _file_prefix("meteo"), _p(filepath), _msg_type(msg_type) {
            _count = find_current_count();
        }    

        virtual void drain(msgArr);
};

class PersistentDiskWriter: public DiskWriter {
    public:
        PersistentDiskWriter(std::string filepath, message_type_t msg_type): DiskWriter(filepath, msg_type){}
        virtual void clear();
};

class WeeklyRotateWriter: public DiskWriter {
    public:
        WeeklyRotateWriter(std::string filepath, message_type_t msg_type): DiskWriter(filepath, msg_type){}
        virtual void clear();
    private:
        void rotate();    
        
};

