#include "EstruturasdoExt2/superblock.hpp"
#include <iostream>
#include <string>
class Shell{
    
    private:
    ext2_super_block  sb;
    void info();
    void clear();
    
    public:
        Shell();
        ~Shell();
        void run();
};