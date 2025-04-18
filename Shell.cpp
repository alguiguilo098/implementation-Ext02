// Created on:2025-15-04
// Author: Guilherme Almeida Lopes
// Description: Shell class implementation for ext2 file system
// This class provides methods to interact with the ext2 file system

#include "Shell.hpp"
#include <iostream>
#include <string>
#include <cstdio>

using namespace std;

Shell::Shell(string path){
    
    // Constructor implementation
    FILE *file = fopen(path.c_str(), "r");

    if (file == NULL){
        std::cout << "Erro ao abrir o arquivo" << std::endl;
        exit(1);
    };

    // move point file beging superblock
    fseek(file, 1024, SEEK_SET);
    fread(&this->sb, sizeof(ext2_super_block), 1, file);

    //move point file to group descriptor
    int block_size = 1024 << this->sb.s_log_block_size;

    fseek(file,block_size+sizeof(ext2_super_block),SEEK_SET);
    fread(&this->gd,sizeof(ext2_group_desc),1,file);
}

void Shell::info()
{

    // Info method implementation
    int block_size = 1024 << this->sb.s_log_block_size;
    uint32_t inode_table_size = this->sb.s_inodes_per_group * this->sb.s_inode_size;
    uint32_t inode_table_blocks = inode_table_size / block_size;

    // print superblock information
    std::cout << "Volume name.....:" << this->sb.s_volume_name << std::endl;
    std::cout << "Free space......:" << this->sb.s_free_blocks_count << " KiB" << std::endl;
    std::cout << "Free inodes.....:" << this->sb.s_free_inodes_count << std::endl;
    std::cout << "Blocks size......:" << block_size << std::endl;
    std::cout << "Inode size......:" << this->sb.s_inode_size << std::endl;
    std::cout << "Groups count....:" << this->sb.s_blocks_count << std::endl;
    std::cout << "Groups size.....:" << this->sb.s_blocks_per_group << std::endl;
    std::cout << "Groups inodes...:" << this->sb.s_inodes_per_group << std::endl;
    std::cout << "Inodetable size.:" << inode_table_size << std::endl;

}

void Shell::clear(){
    // Clear method implementation
    std::cout <<"\033[2J\033[1;1H";
}
void Shell::read_inode(FILE *fd, int no_inode, ext2_inode *inode) {
    
    fseek(fd, (no_inode-1)*gd.bg_inode_table*sb.s_inode_size,SEEK_SET);
    fread(inode, sizeof(ext2_inode),1,fd);
}

void Shell::read_directory(FILE *fd, ext2_inode *inode) {

};

void Shell::run()
{
    // Run Shell 
    std::string path = "/";
    std::string command;

    while (true){

        std::cout << "[" << path << "]$>";
        std::cin >> command;
        
        if (command == "info"){
            // info superblock
            this->info();
        }else if (command == "exit"){
            // exit shell
            break;
        }else if(command=="clear"){
            // clear shell 
            this->clear();
        }else if(command=="iprintf"){

        }
        else{
            // invalid command 
            std::cout << "Comando inválido"<<std::endl;
        }
    }
}
