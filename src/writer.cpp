#include "writer.hpp"

static bool make_path(const boost::filesystem::path &p) {
    BOOST_LOG_TRIVIAL(info) << "Trying to make file " << p;
    if (boost::filesystem::exists(p)) {
        if (boost::filesystem::is_directory(p)) {
            BOOST_LOG_TRIVIAL(info) << "file exists and is a directory, we are good";
            return true;
        } else {
            BOOST_LOG_TRIVIAL(error) << "file exists but is a file, cant save new messages!";
            return false;
        }
    } else {
        BOOST_LOG_TRIVIAL(warning) << "directory does not exists, trying to create it";
        return boost::filesystem::create_directories(p);   
    }
}

static std::string getDate() {
   boost::posix_time::ptime now = boost::posix_time::second_clock::universal_time();
   boost::posix_time::time_facet *facet = new boost::posix_time::time_facet("%Y%m%d");
   std::stringstream ss;
   ss.imbue(std::locale(ss.getloc(), facet));
   ss << now;
   std::string retVal(ss.str());
   return retVal;
}

void PersistentDiskWriter::clear() {
    boost::mutex::scoped_lock(guard);
    BOOST_LOG_TRIVIAL(info) << "Reseting file count";
   _count=0; 
}

void WeeklyRotateWriter::clear() {
    boost::mutex::scoped_lock(guard);
    BOOST_LOG_TRIVIAL(info) << "Reseting file count and rotating";
    _count=0; 
    rotate();
}

void WeeklyRotateWriter::rotate() {
    boost::posix_time::ptime mindate(boost::gregorian::day_clock::universal_day()-boost::gregorian::date_duration(7));
    boost::filesystem::directory_iterator end_itr;
    BOOST_LOG_TRIVIAL(info) << "Attempting to delete old/stale messages";
    for (boost::filesystem::directory_iterator itr(_p); itr != end_itr; ++itr) {
        boost::posix_time::ptime pt;
        std::string fname = itr->path().string();
        boost::replace_first(fname, _p.string(), "");
        boost::replace_first(fname,"/","");
        const std::locale loc(std::locale::classic(),new boost::posix_time::time_input_facet("%Y%m%d"));
        std::istringstream is(fname);
        is.imbue(loc);
        is >> pt;
        if (pt != boost::posix_time::ptime()) {
            if (pt < mindate){
                BOOST_LOG_TRIVIAL(warning) << "Removing the file: " << itr->path().string();
                boost::filesystem::remove_all(itr->path());
            } else {
                BOOST_LOG_TRIVIAL(info) << "Keeping the file: " << itr->path().string();
            }
        }

    }

}

int DiskWriter::find_current_count() {
    boost::filesystem::path filename(_p);
    std::string dir=getDate();
    filename /= dir;
    make_path(filename);
    boost::filesystem::directory_iterator end_itr;
    int x(0);
    for (boost::filesystem::directory_iterator itr(filename); itr != end_itr; ++itr) {
        std::string fname = itr->path().string();
        boost::replace_first(fname, filename.string(), "");
        boost::replace_first(fname, _file_prefix,"");
        boost::replace_first(fname,".bin","");
        boost::replace_first(fname,"/","");
        int c = boost::lexical_cast<int>(fname);
        if (c > x) {
            x = c;
        }
    }
    return x;
}

static std::size_t drain_to_mongo(msgArr bson_queue, const boost::filesystem::path &p) {
    FILE * outfile;
    outfile = std::fopen(p.string().c_str(), "w");
    mongo::BSONArrayBuilder b;
    for (std::size_t i=0;i<bson_queue.size();++i) {
        BOOST_LOG_TRIVIAL(info) << "writing " << bson_queue[i]->string();
        //all thats left is to handle the binary (on disk) format for general data types.
        b.append(boost::static_pointer_cast<MongoMessage>(bson_queue[i])->obj());
    }
    mongo::BSONArray arr = b.arr();
    std::size_t x = std::fwrite(arr.objdata(), 1, arr.objsize(), outfile);
    return x;
}

std::size_t DiskWriter::drain_queue_to_file(msgArr bson_queue, const boost::filesystem::path &p) {
    std::size_t x;
    switch(_msg_type) {
        case BSON:
            x=drain_to_mongo(bson_queue, p);
            break;
        case INVALID:
            x = 0;
            break;    
        default:
            x = 0;
    }
    return x;
}

int DiskWriter::drain(msgArr bson_queue) {
    boost::filesystem::path filename(_p);
    std::string file(_file_prefix);
    std::string dir=getDate();
    filename /= dir;
    make_path(filename);
    std::string cntStr = std::to_string(_count);
    std::string padCount=std::string("0",5-cntStr.length()).append(cntStr);
    filename /= file.append(padCount).append(".bin").c_str();
    const int x = drain_queue_to_file(bson_queue, filename);
    _count += 1;
    return x;
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
            retVal = NULL;
    }
    return retVal;
}
int MongoWriter::drain(msgArr msgs) {
    mongo::DBClientConnection c;
    c.connect(_hostname);
    for (std::size_t i=0;i<msgs.size();++i) {
        BOOST_LOG_TRIVIAL(info) << "inserting " << msgs[i]->string();
        c.insert("meteo.measurement", boost::static_pointer_cast<MongoMessage>(msgs[i])->obj());
    }
    return 1;
}

