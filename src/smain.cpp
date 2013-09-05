#include <signal.h>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include "logger_config.hpp"
#include "daily_call.hpp"
#include "sub_param_helper.hpp"
#include "sub_control.hpp"
#include "writer.hpp"
#include "subscriber.hpp"

SubControl sc;
void intHandler(int sig){
    sc.intHandler(sig);
}


int main (int argc, char** argv) {
    init("subscriber");

    SubscriberParams sp;
    const int paramRet = sp.parse_options(argc, argv);
    if (paramRet != 0) {
        return 1;
    }
    boost::shared_ptr<AbstractWriter> w = WriterBuilder::create(sp.getStorageFormat(), sp.getOption());
    w->setMsgTypeStore(sp.getStorageType());
    boost::function<void(void)> f = boost::bind(&AbstractWriter::clear, w);
    boost::shared_ptr<Subscriber> s = boost::shared_ptr<Subscriber>(new Subscriber(w, "tcp", sp.getPublishHostName(), sp.getPublishPort(), sp.getQueueSize())); 
    sc = SubControl(s);
    signal(SIGINT, intHandler);
    signal(SIGILL, intHandler);

    boost::thread subThread(boost::ref(*s));
    start_callbacks(f);
    return 0;
}
