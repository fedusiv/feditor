#ifndef __WINDOW_HPP__
#define __WINDOW_HPP__

#include <string>

#include "SDL.h"
#include "glyph_handler.hpp"

class Window
{
    public:
        Window();
        ~Window();
        void Render();  // main function to render whole window
        void DrawText(std::string text, int x, int y, SDL_Color color);

    private:
        void RunWindow(void);
        void DrawTextTest(void);
        void DrawTestRect(void);

        SDL_Window * _window;
        SDL_Renderer * _renderer;

        GlyphHandler * glyphHandler;
};

#endif //__WINDOW_HPP__