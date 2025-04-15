#include "Shell.hpp"
#include<iostream>
int main(int argc, char const *argv[])
{
    try{
        Shell* shell=new Shell();
        shell->run();
    }catch(const std::exception& e){
        std::cerr << e.what() << '\n';
    }
    

    return 0;
}
