#ifndef __WIDGET_HPP__
#define __WIDGET_HPP__

#include "SDL.h"

#include "../common/vector2.hpp"
#include "colors.hpp"
#include "glyph_handler.hpp"

class Widget
{
    public:
        static void SetSdlRenderer(SDL_Renderer * renderer);

        Widget(Vector2 size, Vector2 location);
        virtual void Render(void);

        bool Active();
        void SetActive(bool status);

    protected:
        static SDL_Renderer * _sdlRenderer;

        Vector2 _widgetSize;    // size in pixels
        Vector2 _widgetLocation; // pixel location in window
        Colors * _colorStorage; // pointer to colors handler singleton
        GlyphHandler * _glyphHandler;   // pointer to glyph handler singleton
        bool _active;   // means, that user currently is operating with this widget

        void DrawCharacter(int character, Vector2 pos, SDL_Color color);
        void DrawBackground(SDL_Color color);
        void DrawCursor(Vector2 pos); // draw cursor at given position
};
#endif // __WIDGET_HPP__
