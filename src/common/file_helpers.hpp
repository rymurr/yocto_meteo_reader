#ifndef _FILE_HELPERS_HPP_
#define _FILE_HELPERS_HPP_

#include <string>
#include <boost/filesystem.hpp>

std::string getDate() ;
bool make_path(const boost::filesystem::path &p) ;

#endif
