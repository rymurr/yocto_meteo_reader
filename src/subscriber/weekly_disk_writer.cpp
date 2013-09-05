#include "file_helpers.hpp"
#include "weekly_disk_writer.hpp"
#include <boost/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/filesystem.hpp>
#include <string>

void WeeklyRotateWriter::clear() {
    boost::mutex::scoped_lock(guard);
    BOOST_LOG_TRIVIAL(info) << "Reseting file count and rotating";
    find_current_count(); 
    rotate();
}

void WeeklyRotateWriter::rotate() {
    boost::posix_time::ptime mindate(boost::gregorian::day_clock::universal_day()-boost::gregorian::date_duration(7));
    boost::filesystem::directory_iterator end_itr;
    BOOST_LOG_TRIVIAL(info) << "Attempting to delete old/stale messages";
    for (boost::filesystem::directory_iterator itr(_p); itr != end_itr; ++itr) {
        boost::posix_time::ptime pt;
        std::string fname = itr->path().string();
        boost::replace_first(fname, _p.string(), "");
        boost::replace_first(fname,"/","");
        const std::locale loc(std::locale::classic(),new boost::posix_time::time_input_facet("%Y%m%d"));
        std::istringstream is(fname);
        is.imbue(loc);
        is >> pt;
        if (pt != boost::posix_time::ptime()) {
            if (pt < mindate){
                BOOST_LOG_TRIVIAL(warning) << "Removing the file: " << itr->path().string();
                boost::filesystem::remove_all(itr->path());
            } else {
                BOOST_LOG_TRIVIAL(info) << "Keeping the file: " << itr->path().string();
            }
        }

    }

}


