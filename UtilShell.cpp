#include "Shell.hpp"

void Shell::split(std::string str, char delimiter, std::string* result, int count) {
    size_t start = 0;
    size_t end = str.find(delimiter);
    count = 0;

    while (end != std::string::npos) {
        result[count++] = str.substr(start, end - start);
        start = end + 1;
        end = str.find(delimiter, start);
    }

    result[count++] = str.substr(start); // Último pedaço
}


