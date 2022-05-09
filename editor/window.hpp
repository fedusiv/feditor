#ifndef __WINDOW_HPP__
#define __WINDOW_HPP__

#include <string>

#include "SDL.h"

#include "glyph_handler.hpp"
#include "layout.hpp"

class Window
{
    enum ColoringTypes
    {
        Text,
        Cursor,
        Lines,
        Backgroud,
        ColoringTypesSize
    };

    public:
        Window();
        ~Window();
        void Render();  // main function to render whole window
        void DrawCharacter(int character, Vector2 pos, SDL_Color color);
        void DrawCursor(Vector2 pos); // draw cursor at required position

    private:
        void RunWindow(void);   // staring point

        void ProcessLayouts(void);
        void ConfigureLayout(void);
        void ConfigureColors(void);
        void CalculateLayoutArea(LayoutArea * area);

        void DrawEmptyLines(void);
        void DrawLinesNumber(void);

        SDL_Window * _window;
        Vector2  _currentWindowSize;
        SDL_Renderer * _renderer;

        GlyphHandler * _glyphHandler;   // local instance to glyph handler
        Layout _layout;  // stores information about current layout

        SDL_Color _coloringValues[ColoringTypes::ColoringTypesSize]; // array storing color data for each type of text

};

#endif //__WINDOW_HPP__