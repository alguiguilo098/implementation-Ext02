#include "EstruturasdoExt2/superblock.hpp"
#include "EstruturasdoExt2/Descriptorgroup.hpp"
#include "EstruturasdoExt2/inode.hpp"
#include "EstruturasdoExt2/Entry.hpp"
#include <iostream>
#include <string>
#include <map>

using namespace std;
class Shell{
    
    private:
        std::map<string,int> map_inodes;
        ext2_super_block  sb;
        ext2_group_desc gd;
        FILE* fd;
        void info();
        void clear();
        void list_directory(ext2_inode *inode);
        void get_entry_directory(ext2_inode *inode);
        void change_directory(ext2_inode *inode, std::string &path);
        void read_inode(FILE* fd,int no_inode,ext2_inode* inode);
        void read_directory(ext2_inode* inode);
        void iprintf(ext2_inode* inode,int num_inode);
        void split(std::string str, char delimiter, std::string* result, int count);
    public:
        Shell(string path="./myext2image.img");
        ~Shell();
        void run();
};