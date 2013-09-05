#include "file_helpers.hpp"
#include <boost/log/trivial.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>

bool make_path(const boost::filesystem::path &p) {
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

std::string getDate() {
   boost::posix_time::ptime now = boost::posix_time::second_clock::universal_time();
   boost::posix_time::time_facet *facet = new boost::posix_time::time_facet("%Y%m%d");
   std::stringstream ss;
   ss.imbue(std::locale(ss.getloc(), facet));
   ss << now;
   std::string retVal(ss.str());
   return retVal;
}


