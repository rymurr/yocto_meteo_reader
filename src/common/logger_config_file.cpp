#ifndef ARMV6_BUILD
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/common.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/sinks.hpp>
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#endif
#include "logger_config.hpp"

void init_file(std::string fname) {
    #ifndef ARMV6_BUILD
    boost::shared_ptr< boost::log::core > core = boost::log::core::get();

    boost::shared_ptr< boost::log::sinks::text_file_backend > file_backend =
        boost::make_shared< boost::log::sinks::text_file_backend >(
            boost::log::keywords::file_name = fname.append("_%Y%m%d_%5N.log"),
            boost::log::keywords::time_based_rotation = boost::log::sinks::file::rotation_at_time_point(0,0,0)
        );

    file_backend->auto_flush(true);

    typedef boost::log::sinks::synchronous_sink< boost::log::sinks::text_file_backend > sink_tf;
    boost::shared_ptr< sink_tf > sink_file(new sink_tf(file_backend));

    sink_file->set_formatter(boost::log::expressions::format(
                        "{'timestamp':'%1%','severity':'%2%','message':'%3%'}")
                        % boost::log::expressions::attr<boost::posix_time::ptime>("TimeStamp")
                        % boost::log::expressions::attr<boost::log::trivial::severity_level>("Severity")
                        % boost::log::expressions::smessage
                        );
    core->add_sink(sink_file);
    #endif
}

