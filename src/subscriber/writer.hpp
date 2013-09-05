#ifndef __WRITER_HPP__
#define __WRITER_HPP__
#include <boost/log/trivial.hpp>
#include "base_message.hpp"
#include "base_writer.hpp"


class WriterBuilder {
    public:
        static ptrWriter create(writer_t writer_type, std::string option);
};

#endif
