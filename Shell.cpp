
#include "Shell.hpp"
#include <iostream>
#include <string>

Shell::Shell()
{
    // Constructor implementation

    FILE *file = fopen("./myext2image.img", "r");
    if (file == NULL)
    {
        std::cout << "Erro ao abrir o arquivo" << std::endl;
        exit(1);
    };
    fseek(file, 1024, SEEK_SET);
    fread(&this->sb, sizeof(ext2_super_block), 1, file);
}

void Shell::info()
{
    int block_size = 1024 << this->sb.s_log_block_size;
    uint32_t inode_table_size = this->sb.s_inodes_per_group * this->sb.s_inode_size;
    uint32_t inode_table_blocks = inode_table_size / block_size;

    std::cout << "Volume name.....:" << this->sb.s_volume_name << std::endl;
    std::cout << "Free space......:" << this->sb.s_free_blocks_count << " KiB" << std::endl;
    std::cout << "Free inodes.....:" << this->sb.s_free_inodes_count << std::endl;
    std::cout << "Blocks size......:" << block_size << std::endl;
    std::cout << "Inode size......:" << this->sb.s_inode_size << std::endl;
    std::cout << "Groups count....:" << this->sb.s_blocks_count << std::endl;
    std::cout << "Groups size.....:" << this->sb.s_blocks_per_group << std::endl;
    std::cout << "Groups inodes...:" << this->sb.s_inodes_per_group << std::endl;
    std::cout << "Inodetable size.:" << inode_table_size << std::endl;

    // Info method implementation
}

void Shell::clear(){
    std::cout <<"\033[2J\033[1;1H";
};
void Shell::run()
{
    // Run method implementation
    std::string path = "/";
    std::string command;
    while (true)
    {
        std::cout << "[" << path << "]$>";
        std::cin >> command;
        if (command == "info"){
            this->info();
        }else if (command == "exit"){
            break;
        }else if(command=="clear"){
            this->clear();
        }else{
            std::cout << "Comando inválido"<<std::endl;
        }
    }
}
