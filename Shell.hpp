#include "EstruturasdoExt2/superblock.hpp"
#include "EstruturasdoExt2/Descriptorgroup.hpp"
#include "EstruturasdoExt2/inode.hpp"
#include "EstruturasdoExt2/Entry.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

class Shell{
    
    private:
        std::vector<ext2_dir_entry_2> entries;
        std::map<string,uint32_t> map_inode= std::map<string,uint32_t>();
        ext2_super_block  sb;
        ext2_group_desc* gd;
        FILE* fd;
        void info();
        void clear();
        void list_directory(ext2_inode *inode);
        std::vector<ext2_dir_entry_2> get_entry_directory(ext2_inode *inode);
        void read_inode(FILE* fd,uint32_t num_inode,ext2_inode* inode);
        void iprintf(ext2_inode* inode,int num_inode);
        void split(std::string str, char delimiter, std::string* result, int count);
        void cat(ext2_inode *inode, int num_inode);
        void read_block_13(uint32_t block_num, int level, FILE* fd); 
        void read_block_dir_indirect(uint32_t block_num, FILE* fd);
        void touch_file(ext2_inode* inode, string filename);
        void mkdir_file(ext2_inode* inode, string dirname);

    public:
        Shell(string path="./myext2image.img");

        void run();
        void change_dir(std::string *args, std::string &path);
};