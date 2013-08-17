#include "writer.hpp"

static bool make_path(const boost::filesystem::path &p) {
    if (boost::filesystem::exists(p)) {
        if (boost::filesystem::is_directory(p)) {
            return true;
        } else {
            return false;
        }
    } else {
        return boost::filesystem::create_directories(p);   
    }
}

int DiskWriter::find_current_count() {
    return 0;
}

static std::size_t drain_to_mongo(msgArr bson_queue, const boost::filesystem::path &p) {
    make_path(p);
    FILE * outfile;
    outfile = std::fopen(p.string().c_str(), "w");
    mongo::BSONArrayBuilder b;
    for (std::size_t i=0;i<bson_queue.size();++i) {
        std::cout << "writing " << bson_queue[i]->string() << std::endl;
        //all thats left is to handle the binary (on disk) format for general data types.
        //b.append(bson_queue[i]);
    }
    mongo::BSONArray arr = b.arr();
    return std::fwrite(arr.objdata(), 1, arr.objsize(), outfile);
}

std::size_t DiskWriter::drain_queue_to_file(msgArr bson_queue, const boost::filesystem::path &p) {
    switch(_msg_type) {
        case BSON:
            drain_to_mongo(bson_queue, p);
            break;
        default:
            std::cout << " FOO !" << std::endl;

    }
}

static std::string getDate() {
   boost::posix_time::ptime now = boost::posix_time::second_clock::universal_time();
   boost::posix_time::time_facet *facet = new boost::posix_time::time_facet("%Y%b%d");
   std::stringstream ss;
   ss.imbue(std::locale(ss.getloc(), facet));
   ss << now;
   free(facet);
   return ss.str();
}

void DiskWriter::drain(msgArr bson_queue) {
    boost::filesystem::path filename(_p);
    std::string file(_file_prefix);
    std::string dir(getDate());
    filename /= dir;
    filename /= file.append(std::to_string(_count)).append(".bin").c_str();
    drain_queue_to_file(bson_queue, filename);
    _count += 1;
    return ;
}

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

ptrWriter WriterBuilder::create(writer_t writer_type, std::string option, message_type_t msg_type) {
    ptrWriter retVal;
    switch(writer_type) {
        case MONGO:
            retVal = ptrWriter(new MongoWriter(option, msg_type));
            break;
        case FILEDIR_PERSISTENT:
            retVal = ptrWriter(new PersistentDiskWriter(option, msg_type));
            break;    
        default:
            retVal = NULL;
    }
    return retVal;
}
void MongoWriter::drain(msgArr msgs) {
    mongo::DBClientConnection c;
    c.connect(_hostname);
    for (std::size_t i=0;i<msgs.size();++i) {
        std::cout << "inserting " << msgs[i]->string() << std::endl;
        c.insert("meteo.measurement", boost::static_pointer_cast<MongoMessage>(msgs[i])->obj());
    }
}
