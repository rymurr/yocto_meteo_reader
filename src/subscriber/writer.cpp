#include "writer.hpp"
#include "weekly_disk_writer.hpp"
#include "disk_writer.hpp"
#include "mongo_writer.hpp"


/*
static std::vector<mongo::BSONObj> read_file(const boost::filesystem::path &p) {
    std::vector<mongo::BSONObj> bson_queue;
    FILE * pFile;
    pFile = std::fopen(p.string().c_str(), "r");
    fseek(pFile, 0, SEEK_END);
    std::size_t fsize = ftell(pFile);
    std::rewind(pFile);

    char* buffer = (char*) malloc(sizeof(char)*fsize);
    fread(buffer,1,fsize,pFile);
    mongo::BSONObj arr(buffer);

    mongo::BSONObjIterator it(arr);
    while ( it.more() ) {
        mongo::BSONElement el = it.next();
        bson_queue.push_back(el.Obj().copy());
    }
    return bson_queue;
}
static std::vector<mongo::BSONObj> read_dir(const boost::filesystem::path &p) {
    boost::filesystem::ifstream infile(p);
    std::vector<mongo::BSONObj> bson_queue;
    boost::filesystem::directory_iterator end_itr;
    for (boost::filesystem::directory_iterator itr(p); itr != end_itr; ++itr) {
        std::vector<mongo::BSONObj> bson_queue2 = read_file(itr->path());
        bson_queue.insert(bson_queue.end(), bson_queue2.begin(), bson_queue2.end());
    }
    return bson_queue;
}*/

ptrWriter WriterBuilder::create(writer_t writer_type, std::string option) {
    ptrWriter retVal;
    switch(writer_type) {
        case MONGO:
            retVal = ptrWriter(new MongoWriter(option));
            break;
        case FILEDIR_PERSISTENT:
            retVal = ptrWriter(new PersistentDiskWriter(option));
            break;    
        case FILEDIR_WEEKLY:
            retVal = ptrWriter(new WeeklyRotateWriter(option));
            break;    
        default:
            retVal = ptrWriter();
    }
    return retVal;
}

