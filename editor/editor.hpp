#ifndef __EDITOR_HPP__
#define __EDITOR_HPP__

#include "window.hpp"

class Editor
{
    public:
        Editor();

    private:
        Window * _window;

        void MainLoop(void);    // Main loop of application
        bool PollingProcess(void);  // Polling process of sdl events. Basically input recognition

};

#endif //__EDITOR_HPP__