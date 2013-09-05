#include <iostream>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/log/trivial.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "daily_call.hpp"
Deadline::Deadline(deadline_timer &timer, boost::function<void(void)> callback) : t(timer), _callback(callback) {
    wait();
}

void Deadline::timeout(const boost::system::error_code &e) {
    if (e) {
        BOOST_LOG_TRIVIAL(error) << "unsuccessful midnight callback";
        return;
    }
    BOOST_LOG_TRIVIAL(info) << "successful midnight callback";
    _callback();
    wait();
}

void Deadline::cancel() {
    t.cancel();
}


void Deadline::wait() {
    boost::posix_time::ptime midnight(boost::gregorian::day_clock::universal_day()+boost::gregorian::date_duration(1));
    boost::posix_time::ptime now = boost::posix_time::second_clock::universal_time();
    boost::posix_time::time_duration time_left = midnight-now;
    boost::posix_time::seconds s(time_left.total_seconds());
    BOOST_LOG_TRIVIAL(info) << "Next callback in: " << s << " seconds";
    t.expires_from_now(s);
    t.async_wait(boost::bind(&Deadline::timeout, this, boost::asio::placeholders::error));
}

void start_callbacks(boost::function<void(void)> x)
{
    io_service io;
    deadline_timer t(io);
    x();
    Deadline d(t, x);
    io.run();
}

