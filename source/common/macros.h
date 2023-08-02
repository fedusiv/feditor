#ifndef __MACROS_H__
#define __MACROS_H__

#define UNUSED(expr) do { (void)(expr); } while (0)

void printInfo(const char* file, const char* function, int line);
#define PRINT_INFO() printInfo(__FILE__, __FUNCTION__, __LINE__)

#endif // __MACROS_H__