#ifndef _SUB_CONTROL_HPP_
#define _SUB_CONTROL_HPP_

#include <boost/shared_ptr.hpp>

class Subscriber;

class SubControl {
    private:
        boost::shared_ptr<Subscriber> _s;
        void sigint();

    public:
        SubControl(boost::shared_ptr<Subscriber> s): _s(s) {}
        SubControl() {}

        void intHandler(int sig);

};

#endif 
