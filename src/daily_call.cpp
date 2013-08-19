#include "daily_call.hpp"
Deadline::Deadline(deadline_timer &timer, boost::function<void(void)> callback) : t(timer), _callback(callback) {
            wait();
        }

void Deadline::timeout(const boost::system::error_code &e) {
            if (e)
                return;
            cout << "tick" << endl;
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
            t.expires_from_now(boost::posix_time::seconds(time_left.total_seconds())); //repeat rate here
            //t.expires_from_now(boost::posix_time::seconds(1)); //repeat rate here
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

