#ifndef __FILE_MANAGER_HPP__
#define __FILE_MANAGER_HPP__


#include "executoraccess.hpp"
#include "editor_state.hpp"
#include "executoroc.hpp"
#include "vec2.hpp"

class FileManager
{
    public:
        static void Init(); // add all executor's functors to executor
        static void CreateBuffer(ExecutorAccess * execA, void * data, bool vertical); // just wrapper function to avoid doubling
        static void CreateBufferVertical(ExecutorAccess * execA, void * data); // create buffer in any conditions 
        static void CreateBufferHorizontal(ExecutorAccess * execA, void * data); // create aspecially horizontal buffer in any conditions 
        // Next four funtions switching active widgets inside one tab
        static void SwitchBetweenEditorsInTabUp(ExecutorAccess * execA, void * data);
        static void SwitchBetweenEditorsInTabDown(ExecutorAccess * execA, void * data);
        static void SwitchBetweenEditorsInTabLeft(ExecutorAccess * execA, void * data);
        static void SwitchBetweenEditorsInTabRight(ExecutorAccess * execA, void * data);
        static void SwitchBetweenEditorsInTab(ExecutorAccess * execA, MoveCursorDirection direction);
        static void CreateNewTab(ExecutorAccess * execA, void * data);  // creates new tab
        static void SwitchToTab(ExecutorAccess * execA, void * data);  // user requested go to specific tab

};

#endif // __FILE_MANAGER_HPP__
