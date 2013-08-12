#include <vector>
#include <algorithm>
#include <boost/lambda/lambda.hpp>
#include <boost/bind.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include "mongo/client/dbclient.h"

enum writer_t {MONGO, FILEDIR_WEEKLY, FILEDIR_PERSISTENT};
class AbstractWriter {
    public:
        virtual void drain() = 0;
        virtual void addToQueue(mongo::BSONObj&);
};

typedef boost::shared_ptr<AbstractWriter> ptrWriter; 
typedef std::vector<mongo::BSONObj> objVec;

class WriterBuilder {
    public:
        ptrWriter create(writer_t writer_type, std::string& option);
};

class MongoWriter: public AbstractWriter {
    private: 
        const std::string _hostname;
        objVec _bson_queue;

    public:
        MongoWriter(std::string& hostname): _hostname(hostname){}
    
        virtual void drain() ;

        virtual void addToQueue(mongo::BSONObj& x){_bson_queue.push_back(x);}

};
