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

static std::size_t drain_queue_to_file(std::vector<mongo::BSONObj> &bson_queue, const boost::filesystem::path &p) {
    FILE * outfile;
    outfile = std::fopen(p.string().c_str(), "w");
    mongo::BSONArrayBuilder b;
    for (std::size_t i=0;i<bson_queue.size();++i) {
        std::cout << "writing " << bson_queue[i].jsonString() << std::endl;
        b.append(bson_queue[i]);
    }
    bson_queue.clear();
    mongo::BSONArray arr = b.arr();
    return std::fwrite(arr.objdata(), 1, arr.objsize(), outfile);
}

static int drain_to_file(std::vector<mongo::BSONObj> &bson_queue, const boost::filesystem::path &p, const std::string &file_prefix, const int count = 0) {
    boost::filesystem::path filename(p);
    std::string file(file_prefix);
    filename /= file.append(std::to_string(count)).append(".bin").c_str();
    drain_queue_to_file(bson_queue, filename);
    return count + 1;
}

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
}

void MongoWriter::drain() {
    mongo::DBClientConnection c;
    c.connect(_hostname);
    for (std::size_t i=0;i<_bson_queue.size();++i) {
        std::cout << "inserting " << _bson_queue[i].jsonString() << std::endl;
        c.insert("meteo.measurement", _bson_queue[i]);
    }
    _bson_queue.clear();
}

