#ifndef _PUB_CONTROL_HPP_
#define _PUB_CONTROL_HPP_

#include <signal.h>
#include <boost/shared_ptr.hpp>
#include <boost/log/trivial.hpp>

class Publisher; 

class PubControl {
    private:
        boost::shared_ptr<Publisher> _s;
        void sigill();
        void sigint();

    public:
        PubControl(boost::shared_ptr<Publisher> s): _s(s) {}
        PubControl() {}

        void intHandler(int sig);

};

#endif
