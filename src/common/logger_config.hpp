#ifndef _LOGGER_CONFIG_HPP_
#define _LOGGER_CONFIG_HPP_
#include <string>

void init_file(std::string fname) ;
void init_console() ;
void init_common() ;
static void init(std::string fname) {
    init_common();
    init_console();
    init_file(fname);
};
#endif

