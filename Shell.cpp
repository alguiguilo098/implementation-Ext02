// Created on:2025-15-04
// Author: Guilherme Almeida Lopes
// Description: Shell class implementation for ext2 file system
// This class provides methods to interact with the ext2 file system

#include "Shell.hpp"
#include <iostream>
#include <string>
#include <cstring>
#include <ctime>
#include <cstdio>

using namespace std;
#define BLOCK_SIZE 1024

Shell::Shell(string path)
{

    // Constructor implementation
    this->fd = fopen(path.c_str(), "r");
    if (fd == NULL)
    {
        std::cout << "Erro ao abrir o arquivo" << std::endl;
        exit(1);
    };

    // move point file beging superblock
    fseek(fd, 1024, SEEK_SET);
    fread(&this->sb, sizeof(ext2_super_block), 1, fd);

    int block_size = 1024 << this->sb.s_log_block_size;
    // move point file to group descriptor
    fread(&this->gd, sizeof(ext2_group_desc), 1, fd);

    this->map_inodes=std::map<int,ext
}

void Shell::info()
{

    // Info method implementation
    int block_size = BLOCK_SIZE << this->sb.s_log_block_size;
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

void Shell::clear()
{
    // Clear method implementation
    std::cout << "\033[2J\033[1;1H";
}

void Shell::list_directory(ext2_inode *inode)
{
    if ((inode->i_mode & 0xF000) == 0x4000) // Verifica se é diretório
    {
        std::cout << "Directory contents:" << std::endl;

        get_entry_directory(inode);
    }
    else
    {
        std::cout << "Inode is not a directory." << std::endl;
    }
}

void Shell::get_entry_directory(ext2_inode *inode)
{
    for (int i = 0; i < 12; i++) // usa só os blocos diretos
    {
        if (inode->i_block[i] == 0)
            continue;

        uint8_t block[BLOCK_SIZE];
        fseek(fd, inode->i_block[i] * BLOCK_SIZE, SEEK_SET);
        fread(block, 1, BLOCK_SIZE, fd);

        uint32_t offset = 0;
        while (offset < BLOCK_SIZE)
        {
            ext2_dir_entry_2 *entry = (ext2_dir_entry_2 *)(block + offset);
            if (entry->inode == 0 || entry->rec_len == 0)
                break;

            // Pega o nome corretamente com terminador
            char name[EXT2_NAME_LEN + 1] = {0};
            memcpy(name, entry->name, entry->name_len);
            name[entry->name_len] = '\0';

            std::cout << "Inode: " << entry->inode << ", Name: " << name << std::endl;

            offset += entry->rec_len;
        }
    }
}

void Shell::change_directory(ext2_inode *inode, std::string &path)
{
}

void Shell::change_directory(ext2_inode *inode, std::string*path){
    // Change directory implementation
    if((inode->i_mode & 0xF000)== 0x4000){
        
    }
}

void Shell::read_inode(FILE *fd, int num_inode, ext2_inode *inode)
{

    int inode_size = sb.s_inode_size;
    int offset = BLOCK_SIZE * gd.bg_inode_table + (num_inode - 1) * inode_size;
    fseek(fd, offset, SEEK_SET);
    fread(inode, sizeof(ext2_inode), 1, fd);
}

void Shell::iprintf(ext2_inode *inode, int num_inode)
{
    // Print inode information

    std::cout << "Inode number: " << num_inode << std::endl;
    std::cout << "File type: " << inode->i_mode << std::endl;
    std::cout << "File size: " << inode->i_size << " bytes" << std::endl;
    std::cout << "Access time: " << inode->i_atime << std::endl;
    std::cout << "Modification time: " << inode->i_mtime << std::endl;
    std::cout << "Change time: " << inode->i_ctime << std::endl;
    std::cout << "Links count: " << inode->i_links_count << std::endl;
    std::cout << "Blocks count: " << inode->i_blocks << std::endl;
    std::cout << "Block pointers: ";
    for (int i = 0; i < EXT2_N_BLOCKS; i++)
    {
        std::cout << inode->i_block[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "File flags: " << inode->i_flags << std::endl;
    std::cout << "Generation number: " << inode->i_generation << std::endl;
};

void Shell::run()
{
    // Run Shell
    std::string path = "/";
    std::string command;
    std::string *args = new std::string[10];
    ext2_inode inode;
    this->read_inode(this->fd, 2, &inode); // Read root inode
    int number_inode;
    while (true)
    {

        std::cout << "[" << path << "]$>";
        std::getline(std::cin, command);
        this->split(command, ' ', args, 10);
        if (args[0] == "info")
        {
            // info superblock
            this->info();
        }
        else if (args[0] == "exit")
        {
            // exit shell
            break;
        }
        else if (args[0] == "clear")
        {
            // clear shell
            this->clear();
        }
        else if (args[0] == "iprintf")
        {
            read_inode(fd, atoi(args[1].c_str()), &inode);
            iprintf(&inode, atoi(args[1].c_str()));
        }
        else if (args[0] == "ls")
        {
            this->list_directory(&inode);
        }
        else
        {
            std::cout << "Comando inválido" << std::endl;
        }
    }
}
