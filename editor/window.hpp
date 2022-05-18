#ifndef __WINDOW_HPP__
#define __WINDOW_HPP__

#include <string>

#include "SDL.h"

#include "glyph_handler.hpp"
#include "layout.hpp"
#include "buffer.hpp"
#include "command.hpp"

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
        void SetCurrentBuffer(Buffer* buffer);
        void SetCursorPosition(Vector2 pos);
        void ScrollActiveTextEditor(ScrollWindowType type); // scroll layout. not to draw just to move values of drawing
        void ScrollActiveTextEditorDueCursor(void); // when cursor changes position in text editor, need to make required changes in window repesentation.
        void ConfigureLayout(void);

    private:
        void RunWindow(void);   // staring point
        void ProcessLayouts(void);
        void ConfigureLayoutLinesArea(void);
        void ConfigureColors(void);
        void CalculateLayoutArea(LayoutArea * area);

        void DrawCharacter(int character, Vector2 pos, SDL_Color color);
        void DrawCursor(Vector2 pos); // draw cursor at required position
        void DrawEmptyLines(int startLine); // draw empty lines from specific line, where buffer does not have any data
        void DrawLinesNumber(void);
        void DrawTextEditorLayout(void);


        SDL_Window * _window;
        Vector2  _currentWindowSize;
        SDL_Renderer * _renderer;

        GlyphHandler * _glyphHandler;   // local instance to glyph handler
        Layout _layout;  // stores information about current layout
        Buffer* _buffer;    // pointer to current buffer which is represented (in the future with split it probably will be changed)

        SDL_Color _coloringValues[ColoringTypes::ColoringTypesSize]; // array storing color data for each type of text

};

#endif //__WINDOW_HPP__