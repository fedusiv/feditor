#ifndef __EXECUTORTYPES_HPP__
#define __EXECUTORTYPES_HPP__

#include "gui.hpp"
#include "buffer_handler.hpp"


/*
    Class is a part of executor mechanism.
    Each executor functions will have pointer to this object tot have acces to important parts of application
*/
class ExecutorAccess
{
    public:
        ExecutorAccess(Gui * gui, BufferHandler * bH) : gui(gui), bufferHandler(bH)
        {}


        Gui * gui;
        BufferHandler * bufferHandler;

};

typedef void (ExecutorMethod)(ExecutorAccess *, void *);

#endif // __EXECUTORTYPES_HPP__