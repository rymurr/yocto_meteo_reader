#include <vector>
#include <algorithm>
#include <boost/lambda/lambda.hpp>
#include <boost/bind.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include "mongo/client/dbclient.h"
#include "message.hpp"

enum writer_t {MONGO, FILEDIR_WEEKLY, FILEDIR_PERSISTENT};

typedef std::vector<boost::shared_ptr<Message> > msgArr;
class AbstractWriter {
    public:
        virtual void drain(msgArr) = 0;
};

typedef boost::shared_ptr<AbstractWriter> ptrWriter; 

class WriterBuilder {
    public:
        static ptrWriter create(writer_t writer_type, std::string option);
};

class MongoWriter: public AbstractWriter {
    private: 
        const std::string _hostname;

    public:
        MongoWriter(std::string& hostname): _hostname(hostname){}
    
        virtual void drain(msgArr);


};

class DiskWriter: public AbstractWriter {
    private:
        const boost::filesystem::path _p;
        const std::string _file_prefix;
        int _count;

        int find_current_count();
        std::size_t drain_queue_to_file(msgArr &bson_queue, const boost::filesystem::path &p); 

    public:
        DiskWriter(std::string filepath): _p(filepath), _file_prefix("meteo") {
            _count = find_current_count();
        }    

        virtual void drain(msgArr);
};

class PersistentDiskWriter: public DiskWriter {
    public:
        PersistentDiskWriter(std::string filepath): DiskWriter(filepath){}
};

class WeeklyRotateWriter: public DiskWriter {
        WeeklyRotateWriter(std::string filepath): DiskWriter(filepath){}
};

