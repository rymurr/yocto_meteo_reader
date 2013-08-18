#include <iostream>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/function.hpp>

using namespace boost::asio;
using namespace std;

class Deadline 
{
    public:
    Deadline(deadline_timer &timer, boost::function<void(void)> callback);

    void timeout(const boost::system::error_code &e) ;

    void cancel() ;


    private:
    void wait() ;
    deadline_timer &t;
    boost::function<void(void)> _callback; 
};

void start_callbacks(boost::function<void(void)> x);
