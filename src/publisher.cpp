#include "publisher.hpp"

static void init() {
    boost::log::add_common_attributes();
    boost::shared_ptr< boost::log::core > core = boost::log::core::get();

    boost::shared_ptr< boost::log::sinks::text_ostream_backend > console_backend =
        boost::make_shared< boost::log::sinks::text_ostream_backend >();
    console_backend->add_stream(
        boost::shared_ptr< std::ostream >(&std::clog, boost::log::empty_deleter()));

    boost::shared_ptr< boost::log::sinks::text_file_backend > file_backend =
        boost::make_shared< boost::log::sinks::text_file_backend >(
            boost::log::keywords::file_name = "publisher_%Y%m%d_%5N.log",
            boost::log::keywords::time_based_rotation = boost::log::sinks::file::rotation_at_time_point(0,0,0)
        );

    console_backend->auto_flush(true);
    file_backend->auto_flush(true);

    typedef boost::log::sinks::synchronous_sink< boost::log::sinks::text_file_backend > sink_tf;
    boost::shared_ptr< sink_tf > sink_file(new sink_tf(file_backend));
    typedef boost::log::sinks::synchronous_sink< boost::log::sinks::text_ostream_backend > sink_ts;
    boost::shared_ptr< sink_ts > sink_console(new sink_ts(console_backend));

    sink_file->set_formatter(boost::log::expressions::format(
                        "{'timestamp':'%1%','severity':'%2%','message':'%3%'}")
                        % boost::log::expressions::attr<boost::posix_time::ptime>("TimeStamp")
                        % boost::log::expressions::attr<boost::log::trivial::severity_level>("Severity")
                        % boost::log::expressions::smessage
                        );
    sink_console->set_formatter(boost::log::expressions::format(
                        "%1% [%2%] %3%")
                        % boost::log::expressions::attr<boost::posix_time::ptime>("TimeStamp")
                        % boost::log::expressions::attr<boost::log::trivial::severity_level>("Severity")
                        % boost::log::expressions::smessage
                        );
    core->add_sink(sink_file);
    core->add_sink(sink_console);
}

int main(int argc, char * argv[])
{
    init();
    PublisherParams pp;
    const int paramRet = pp.parse_options(argc, argv);
    if (paramRet != 0) {
        return 1;
    }
    boost::shared_ptr<Publisher> y = boost::make_shared<Publisher>(Publisher("*", pp.getPort(), "tcp", pp.getMessageType()));
    boost::function<void (Message&)> fct = boost::bind<void>(&Publisher::callback, y, _1);
    BOOST_FOREACH(std::string x, pp.getDevices()) {
        SensorGroup::getInstance().addAllowedDevice(x);
    }
    BOOST_FOREACH(std::string x, pp.getSensorTypes()) {
        SensorGroup::getInstance().addAllowedSensor(x);
    }
    SensorGroup::getInstance().addCallback(fct);
    SensorGroup::getInstance().start();
}

void threaded_rep(std::string hostname, std::string msg){
    zmq::context_t context(1);
    zmq::socket_t sync(context, ZMQ_REP);
    sync.bind(hostname.c_str());
    while(1) {
        std::string rec = s_recv(sync);
        BOOST_LOG_TRIVIAL(info) << "recieved req/rep message from: " << rec;
        s_send(sync, msg);
    }
}

std::string Publisher::make_msg(message_type_t msg_type) {
    boost::property_tree::ptree pt;
    pt.put("type", msg_type);
    std::ostringstream buf;
    boost::property_tree::write_json(buf, pt, false);
    return buf.str();
}

