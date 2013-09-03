#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/common.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/utility/empty_deleter.hpp>
#include "logger_config.cpp"

static void init(std::string fname) {
    boost::log::add_common_attributes();
    boost::shared_ptr< boost::log::core > core = boost::log::core::get();

    boost::shared_ptr< boost::log::sinks::text_ostream_backend > console_backend =
        boost::make_shared< boost::log::sinks::text_ostream_backend >();
    console_backend->add_stream(
        boost::shared_ptr< std::ostream >(&std::clog, boost::log::empty_deleter()));

    boost::shared_ptr< boost::log::sinks::text_file_backend > file_backend =
        boost::make_shared< boost::log::sinks::text_file_backend >(
            boost::log::keywords::file_name = fname.append("_%Y%m%d_%5N.log"),
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

