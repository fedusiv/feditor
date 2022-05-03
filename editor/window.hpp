#ifndef __WINDOW_HPP__
#define __WINDOW_HPP__

#include "SDL.h"

class Window
{
    public:
        Window();
        ~Window();

    private:
        void RunWindow(void);
        void PollingProcess(void);

        SDL_Window * _window;
        SDL_Renderer * _renderer;
};

#endif //__WINDOW_HPP__