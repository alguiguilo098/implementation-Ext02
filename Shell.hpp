#include "EstruturasdoExt2/superblock.hpp"
#include "EstruturasdoExt2/Descriptorgroup.hpp"
#include "EstruturasdoExt2/inode.hpp"
#include <iostream>
#include <string>
using namespace std;
class Shell{
    
    private:
        ext2_super_block  sb;
        ext2_group_desc gd;
        void info();
        void clear();
        void read_inode(FILE* fd,int no_inode,ext2_inode* inode);
        void read_directory(FILE* fd,ext2_inode* inode);
    
    public:
        Shell(string path="./myext2image.img");
        ~Shell();
        void run();
};