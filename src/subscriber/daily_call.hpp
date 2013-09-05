#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <boost/system/error_code.hpp>

using namespace boost::asio;

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
