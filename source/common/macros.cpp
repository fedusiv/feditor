#include <iostream>
#include "macros.h"

void printLineInfo(const char* file, const char* function, int line)
{
    std::cout << file << ":" << function << ":" << line << std::endl; 
}

