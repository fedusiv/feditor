#ifndef __WINDOW_HPP__
#define __WINDOW_HPP__

#include <string>

#include "SDL.h"

#include "glyph_handler.hpp"
#include "layout.hpp"

class Window
{
    public:
        Window();
        ~Window();
        void Render();  // main function to render whole window
        void DrawText(std::string text, int x, int y, SDL_Color color);
        void DrawCharacter(int character, Vector2 pos, SDL_Color color);
        void DrawCursor(int x, int y, SDL_Color color); // draw cursor at required position

    private:
        void RunWindow(void);
        void ProcessBorders(void);
        void CalculateLinesAmount(void);
        void DrawEmptyLines(void);

        SDL_Window * _window;
        Vector2  _currentWindowSize;
        SDL_Renderer * _renderer;

        GlyphHandler * _glyphHandler;
        Layout _layout;  // stores information about current layout
};

#endif //__WINDOW_HPP__