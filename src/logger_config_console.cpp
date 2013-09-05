#ifndef ARMV6_BUILD
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/log/core.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/common.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/empty_deleter.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/trivial.hpp>
#endif
#include "logger_config.hpp"

void init_console() {
    #ifndef ARMV6_BUILD
    boost::shared_ptr< boost::log::core > core = boost::log::core::get();

    boost::shared_ptr< boost::log::sinks::text_ostream_backend > console_backend =
        boost::make_shared< boost::log::sinks::text_ostream_backend >();
    console_backend->add_stream(
        boost::shared_ptr< std::ostream >(&std::clog, boost::log::empty_deleter()));

    console_backend->auto_flush(true);

    typedef boost::log::sinks::synchronous_sink< boost::log::sinks::text_ostream_backend > sink_ts;
    boost::shared_ptr< sink_ts > sink_console(new sink_ts(console_backend));

    sink_console->set_formatter(boost::log::expressions::format(
                        "%1% [%2%] %3%")
                        % boost::log::expressions::attr<boost::posix_time::ptime>("TimeStamp")
                        % boost::log::expressions::attr<boost::log::trivial::severity_level>("Severity")
                        % boost::log::expressions::smessage
                        );
    core->add_sink(sink_console);
    #endif
}

