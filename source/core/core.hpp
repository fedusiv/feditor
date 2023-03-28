#ifndef __CORE_HPP__
#define __CORE_HPP__

#include "gui.hpp"
#include "input.hpp"
#include "executor.hpp"
#include "editor_state.hpp"
#include "buffer_handler.hpp"

class Core
{
    public:
        Core(Gui * gui, Input * input, std::string location);

    private:
        void Init(void);
        void MainLoop(void);
        void InputHandling(void);

        Gui * _guiHandler;
        Input * _inputHandler;
        Executor * _executor;
        BufferHandler * _bufferHandler;
        ExecutorAccess * _execAccess;

        std::string _locationPoint;

};

#endif //__CORE_HPP__
