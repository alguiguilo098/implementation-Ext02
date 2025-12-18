// Created on:2025-15-04
// Author: Guilherme Almeida Lopes
// Description: Shell class implementation for ext2 file system
// This class provides methods to interact with the ext2 file system

#include "Shell.hpp"
#include <iostream> // For input/output operations
#include <string>   // For string manipulations
#include <cstring>  // For C-style string functions
#include <ctime>
#include <cstdio>
#include <sys/stat.h>
using namespace std;
#define BLOCK_SIZE 1024  // Define block size
#define BASE_OFFSET 1024 // Define base offset for superblock

#define BLOCK_OFFSET(block) (BASE_OFFSET + (block - 1) * BLOCK_SIZE)

Shell::Shell(string path)
{
    fd = fopen(path.c_str(), "rb");
    if (!fd)
    {
        perror("fopen");
        exit(1);
    }

    // superblock
    fseek(fd, 1024, SEEK_SET);
    fread(&sb, sizeof(ext2_super_block), 1, fd);

    // group descriptor
    int block_size = 1024 << sb.s_log_block_size;

    // Calculate number of block groups
    uint32_t groups =
        (sb.s_blocks_count + sb.s_blocks_per_group - 1) / sb.s_blocks_per_group;

    // Allocate memory for group descriptors
    gd = new ext2_group_desc[groups];

    // Calculate offset for group descriptors
    off_t gd_offset =
        (block_size == 1024) ? 2048 : block_size;

    // Read group descriptors
    fseek(fd, gd_offset, SEEK_SET);
    fread(gd, sizeof(ext2_group_desc), groups, fd);
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
    // List directory method implementation
    std::vector<ext2_dir_entry_2> entries = get_entry_directory(inode);

    if ((inode->i_mode & 0xF000) == 0x4000) // Verifica se é diretório
    {
        int i = 0;
        for (const auto &entry : entries)
        {
            if (i < 2)
            {
                // Skip "." and ".." entries
                std::cout << "Inode:" << entry.inode << "\t\t" << "Name:" << entry.name << "\t\t\t" << "Size:" << entry.rec_len << std::endl;
                i++;
                continue;
            }

            // Print the entry name
            std::cout << "Inode:" << entry.inode << "\t" << "Name:" << entry.name << "\t\t" << "Size:" << entry.rec_len << std::endl;
        }
    }
    else
    {
        // Not a directory
        std::cout << "Inode is not a directory." << std::endl;
    }
}

std::vector<ext2_dir_entry_2> Shell::get_entry_directory(ext2_inode *inode)
{
    std::vector<ext2_dir_entry_2> entries;
    for (int i = 0; i < 12; i++) // usa só os blocos diretos
    {
        if (inode->i_block[i] == 0)
            continue;

        // Read directory block
        uint8_t block[BLOCK_SIZE];
        fseek(fd, inode->i_block[i] * BLOCK_SIZE, SEEK_SET);
        fread(block, 1, BLOCK_SIZE, fd);

        // Parse directory entries
        uint32_t offset = 0;
        while (offset < BLOCK_SIZE)
        {
            // Pega a entrada do diretório
            ext2_dir_entry_2 *entry = (ext2_dir_entry_2 *)(block + offset);
            if (entry->inode == 0 || entry->rec_len == 0)
                break;

            // Pega o nome corretamente com terminador
            char name[EXT2_NAME_LEN + 1] = {0};
            memcpy(name, entry->name, entry->name_len);
            name[entry->name_len] = '\0';

            // Adiciona a entrada ao vetor
            entries.push_back(*entry);
            this->map_inode[name] = entry->inode;

            offset += entry->rec_len;
        }
    }

    if(inode->i_block[12] != 0)
    {
        // Handle single indirect block
        read_block_dir_indirect(inode->i_block[12], fd);

    }
    return entries;
}

void Shell::read_inode(FILE *fd, uint32_t num_inode, ext2_inode *inode)
{
    if (num_inode == 0 || num_inode > sb.s_inodes_count)
    {
        printf("Invalid inode %u\n", num_inode);
        return;
    }

    // Calculate group and index of the inode
    uint32_t group =
        (num_inode - 1) / sb.s_inodes_per_group;

    // Calculate index within the group
    uint32_t index =
        (num_inode - 1) % sb.s_inodes_per_group;

    // Get inode size
    size_t inode_size = sb.s_inode_size;

    // Calculate offset of the inode
    off_t offset =
        BLOCK_OFFSET(gd[group].bg_inode_table) +
        index * inode_size;

    if (fseek(fd, offset, SEEK_SET) != 0)
    {
        perror("fseek inode");
        return;
    }
    // Read inode data
    uint8_t buf[inode_size];

    if (fread(buf, inode_size, 1, fd) != 1)
    {
        perror("fread inode");
        return;
    }

    // Copy data to inode structure
    memcpy(inode, buf, sizeof(ext2_inode));
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
void Shell::cat(ext2_inode *inode, int num_inode)
{
    // Print file content
    if (S_ISREG(inode->i_mode))
    {
        for (int i = 0; i < EXT2_N_BLOCKS; i++)
        {
            if (inode->i_block[i] == 0)
                continue;
            uint8_t block[BLOCK_SIZE];

            fseek(fd, inode->i_block[i] * BLOCK_SIZE, SEEK_SET);
            fread(block, 1, BLOCK_SIZE, fd);

            std::cout.write((char *)block, BLOCK_SIZE);
        }
        // Handle indirect blocks
        if (inode->i_block[12] != 0)
        {
            // Single indirect
            read_block_13(inode->i_block[12], 1, fd);
        }
        if (inode->i_block[13] != 0)
        {
            // Double indirect
            read_block_13(inode->i_block[13], 2, fd);
        }
        if (inode->i_block[14] != 0)
        {
            // Triple indirect
            read_block_13(inode->i_block[14], 3, fd);
        }
    }
}

void Shell::read_block_13(uint32_t block_num, int level, FILE *fd)
{
    if (level < 1 || level > 3)
        return;

    uint32_t pointers_per_block = BLOCK_SIZE / sizeof(uint32_t); // Number of block pointers per block
    uint32_t block_pointers[pointers_per_block];                 // Array to hold block pointers

    fseek(fd, block_num * BLOCK_SIZE, SEEK_SET);                     // Seek to the block
    fread(block_pointers, sizeof(uint32_t), pointers_per_block, fd); // Read block pointers

    for (uint32_t i = 0; i < pointers_per_block; i++)
    {
        if (block_pointers[i] == 0)
            continue; // Skip if block pointer is 0

        if (level == 1)
        {
            uint8_t block[BLOCK_SIZE];                           // Buffer to hold block data
            fseek(fd, block_pointers[i] * BLOCK_SIZE, SEEK_SET); // Seek to the block
            fread(block, 1, BLOCK_SIZE, fd);                     // Read block data
            std::cout.write((char *)block, BLOCK_SIZE);          // Print block data
        }
        else
        {
            read_block_13(block_pointers[i], level - 1, fd); // Recursive call for next level
        }
    }
}




void Shell::touch_file(ext2_inode *inode, string filename)
{
    uint32_t bitmap=gd[0].bg_inode_bitmap;

}

void Shell::mkdir_file(ext2_inode *inode, string dirname)
{
}

void Shell::run()
{
    // Run Shell
    std::string path = "/";
    std::string imagename = "myext2image.img";
    std::string command; // Command input
    std::string *args = new std::string[10];
    ext2_inode inode; // Current inode

    this->read_inode(this->fd, 2, &inode); // Read root inode

    int number_inode; // Number of inode
    while (true)
    {

        // Shell prompt
        std::cout << "[" << imagename << path << "]$>";
        std::getline(std::cin, command);
        this->split(command, ' ', args, 10);
        if (args[0] == "info")
        {
            // info superblock
            this->info();
        }
        if (args[0] == "pwd")
        {
            // print current working directory
            std::cout << path << std::endl;
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
        else if (args[0] == "cd" && args[1] != " ")
        {
            // change directory
            uint32_t number_inode = this->map_inode[args[1]];
            ext2_inode inode_dir;
            // Read the inode of the directory
            this->read_inode(this->fd, number_inode, &inode_dir);
            // List the directory to update entries
            this->list_directory(&inode_dir);
            change_dir(args, path);

            this->read_inode(this->fd, number_inode, &inode);
        }
        else if (args[0] == "cat" && args[1] != " ")
        {
            uint32_t number_inode = this->map_inode[args[1]];
            ext2_inode inode_file;
            this->read_inode(this->fd, number_inode, &inode_file);
            this->cat(&inode_file, number_inode);
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

void Shell::change_dir(std::string *args, std::string &path)
{
    if (args[1] != "." && args[1] != "..")
    {
        path += args[1];
    }
    if (args[1] == "..")
    {
        size_t pos = path.find_last_of('/');
        if (pos != std::string::npos)
        {
            path = path.substr(0, pos);
            if (path.empty())
            {
                path = "/";
            }
        }
    }
}
