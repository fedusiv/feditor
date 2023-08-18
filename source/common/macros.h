#ifndef __MACROS_H__
#define __MACROS_H__

#include <iostream>

#define UNUSED(expr) do { (void)(expr); } while (0)

void printLineInfo(const char* file, const char* function, int line);
#define PRINT_LINE_INFO() printInfo(__FILE__, __FUNCTION__, __LINE__)

// This macro function allows to print custom class.
// Requirement for class is need to have function getPrint() which returns std::vector or other interatable container of strings
template <typename T>
void PRINT_DATA(const T& data)
{
    bool first = true;
    for (const auto& field : data.getPrint()) {
        if(!first)
        {
            std::cout << " ";
        }
        std::cout << field;
        if(first)
        {
            first = false;
            std::cout << ": ";
        }
    }
    std::cout << std::endl;
}

#endif // __MACROS_H__
