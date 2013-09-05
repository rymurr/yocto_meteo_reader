#ifndef _WEEKLY_ROTATE_WRITER_HPP_
#define _WEEKLY_ROTATE_WRITER_HPP_

#include <string>
#include "disk_writer.hpp"

class WeeklyRotateWriter: public DiskWriter {
    public:
        WeeklyRotateWriter(std::string filepath): DiskWriter(filepath){}
        virtual void clear();
    private:
        void rotate();    
        
};

#endif
