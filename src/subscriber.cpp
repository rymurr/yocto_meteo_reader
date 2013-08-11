#include <vector>
#include <algorithm>
#include <boost/lambda/lambda.hpp>
#include <boost/bind.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <signal.h>
#include "mongo/client/dbclient.h"
#include "zhelpers.hpp"

enum {WEEKLY, PERSISTENT};

void handler(int s) {
    std::cout << "Oh no!" << std::endl;
    exit(1);
}


static mongo::BSONObj
s_recvobj (zmq::socket_t & socket) {

    zmq::message_t message;
    socket.recv(&message);

    return mongo::BSONObj(static_cast<char*>(message.data()));
}

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
static void drain_to_mongo(std::vector<mongo::BSONObj> &bson_queue, const std::string host = "localhost") {
    mongo::DBClientConnection c;
    c.connect(host);
    for (std::size_t i=0;i<bson_queue.size();++i) {
        std::cout << "inserting " << bson_queue[i].jsonString() << std::endl;
        c.insert("meteo.measurement", bson_queue[i]);
    }
    bson_queue.clear();

    return;
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

int main () {
    //  Prepare our context and subscriber
    int x(0);
    boost::filesystem::path p("/data/meteo/storage");
    zmq::context_t context(1);
    zmq::socket_t subscriber (context, ZMQ_SUB);
    subscriber.connect("tcp://localhost:5563");
    subscriber.setsockopt( ZMQ_SUBSCRIBE, "", 0);
    std::vector<mongo::BSONObj> bson_queue;

    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;

     sigaction(SIGINT, &sigIntHandler, NULL);
    while (1) {

        //  Read envelope with address
        std::string address = s_recv (subscriber);
        //  Read message contents
        mongo::BSONObj r  = s_recvobj (subscriber);
        bson_queue.push_back(r.copy());
        std::cout << r.jsonString() << std::endl;
        if (bson_queue.size() > 2) {
//            x = drain_to_file(bson_queue, boost::filesystem::path("data"), "meteo", x);
            drain_to_mongo(bson_queue);
        }
    }
    return 0;
}
