#ifndef __WINDOW_HPP__
#define __WINDOW_HPP__

#include <string>
#include <vector>

#include "SDL.h"

#include "widget.hpp"
#include "vector2.hpp"
#include "colors.hpp"
#include "glyph_handler.hpp"
#include "../common/buffer.hpp"

class Window
{

    public:
        Window();
        ~Window();
        void Render(void);  // main function to render whole window
        void CreateWidgetEditor(Buffer * buffer); // requested to create widget with attached buffer to it

    private:
        void CreateWindow(void);    // create main window
        void InitWidgets(void);     // initialize starting widgets

        SDL_Window * _sdlWindow;    // sdl window is related to OS driver
        SDL_Renderer * _sdlRenderer; // sdl renderer is related to sdl driver functionality itself

        std::vector<Widget*> _widgets; // vector to store all created widgets

        Colors * _colorStorage; // object where all colors are stored

        GlyphHandler * _glyphHandler;   // pointer to glyph handler

};

#endif //__WINDOW_HPP__