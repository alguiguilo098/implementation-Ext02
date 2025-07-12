#include "Shell.hpp"
#include<iostream>

using namespace std;

int main(int argc, char const *argv[])
{
    try{
        Shell* shell=new Shell(argv[1]);
        shell->run();
    }catch(const std::exception& e){
        std::cerr << e.what() << '\n';
    }
    
    return 0;
}