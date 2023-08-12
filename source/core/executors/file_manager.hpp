#ifndef __FILE_MANAGER_HPP__
#define __FILE_MANAGER_HPP__


#include "executoraccess.hpp"
#include "editor_state.hpp"

class FileManager
{
    public:
        static void Init(); // add all executor's functors to executor
        static void CreateBuffer(ExecutorAccess * execA, void * data); // create buffer in any conditions 
        static void CreateBufferHorizontal(ExecutorAccess * execA, void * data); // create aspecially horizontal buffer in any conditions 
};

#endif // __FILE_MANAGER_HPP__
