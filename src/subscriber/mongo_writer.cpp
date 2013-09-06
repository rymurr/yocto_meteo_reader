
#include "mongo_writer.hpp"
#ifdef MONGO_AVAIL
#include <boost/shared_ptr.hpp>
#include <boost/log/trivial.hpp>
#include "mongo/client/dbclient.h"
#include "message.hpp"
#include "mongo_message.hpp"
#endif

int MongoWriter::drain(msgArr msgs) {
    #ifdef MONGO_AVAIL
    mongo::DBClientConnection c;
    c.connect(_hostname);
    for (std::size_t i=0;i<msgs.size();++i) {
        BOOST_LOG_TRIVIAL(info) << "inserting " << msgs[i]->string();
        c.insert("meteo.measurement", boost::static_pointer_cast<MongoMessage>(convert(msgs[i],_msg_type,BSON))->obj());
    }
    #endif
    return 1;
}

