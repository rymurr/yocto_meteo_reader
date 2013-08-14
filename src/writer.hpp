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

    public:
        DiskWriter(std::string filepath): _p(filepath) {
        }    

        virtual void drain(msgArr);
};
