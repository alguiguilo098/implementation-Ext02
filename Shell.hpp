#include "EstruturasdoExt2/superblock.hpp"
class Shell{
    
    private:
    ext2_super_block  sb;
    
    public:
        void info();
        Shell();
        ~Shell();
        void run();
};