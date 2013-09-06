#include <fstream>
#include <boost/algorithm/string.hpp>
#include "disk_writer.hpp"
#include "file_helpers.hpp"
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
#include "message.hpp"
#include "mongo_message.hpp"
#ifdef MONGO_AVAIL
#include "mongo/client/dbclient.h"
#endif


int DiskWriter::find_current_count() {
    boost::filesystem::path filename(_p);
    std::string dir=getDate();
    filename /= dir;
    make_path(filename);
    boost::filesystem::directory_iterator end_itr;
    int x(-1);
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
    x += 1;
    BOOST_LOG_TRIVIAL(info) << "Starting files from " << x;
    return x;
}

static std::size_t drain_to_mongo(msgArr bson_queue, const boost::filesystem::path &p, message_type_t msg_type) {
    std::size_t x = -1;
    #ifdef MONGO_AVAIL
    FILE * outfile;
    outfile = std::fopen(p.string().c_str(), "w");
    mongo::BSONArrayBuilder b;
    for (std::size_t i=0;i<bson_queue.size();++i) {
        BOOST_LOG_TRIVIAL(info) << "writing " << bson_queue[i]->string();
        //all thats left is to handle the binary (on disk) format for general data types.
        b.append(boost::static_pointer_cast<MongoMessage>(convert(bson_queue[i],msg_type, BSON))->obj());
    }
    mongo::BSONArray arr = b.arr();
    x = std::fwrite(arr.objdata(), 1, arr.objsize(), outfile);
    #endif
    return x;
}

static std::size_t drain_to_json(msgArr bson_queue, const boost::filesystem::path &p, message_type_t msg_type) {
    std::size_t x = 0;
    std::ofstream fout;
    fout.open(p.string().c_str());
    for (std::size_t i=0;i<bson_queue.size();++i) {
        BOOST_LOG_TRIVIAL(info) << "writing " << convert(bson_queue[i],msg_type,JSON)->string();
        fout << bson_queue[i]->string() << "\n";
        x+=1;
    }
    fout.close();
    return x;
}

std::size_t DiskWriter::drain_queue_to_file(msgArr bson_queue, const boost::filesystem::path &p) {
    std::size_t x;
    switch(_msg_type_store) {
        case BSON:
            x=drain_to_mongo(bson_queue, p, _msg_type);
            break;
        case JSON:
            x=drain_to_json(bson_queue, p, _msg_type);
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
    std::stringstream ss;
    ss << boost::format("%05i") % _count;
    std::string cntStr = ss.str();
    filename /= file.append(cntStr).append(".bin");
    const int x = drain_queue_to_file(bson_queue, filename);
    _count += 1;
    return x;
}

void PersistentDiskWriter::clear() {
    boost::mutex::scoped_lock(guard);
    BOOST_LOG_TRIVIAL(info) << "Reseting file count";
    find_current_count(); 
}


