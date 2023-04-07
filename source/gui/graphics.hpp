#ifndef __GRAPHICS_HPP__
#define __GRAPHICS_HPP__

#include "SDL.h"
#include "SDL_ttf.h"
#include "vec2.hpp"
#include "colors.hpp"
#include <string>

#define GLYPHS_AMOUNT 128
#define FPS           60

// This is Graphics API
class Graphics
{
    public:
        static bool Init(Vec2& windowSize, int fontSize);
        static void DeInit();
        static Vec2 GetAppSize();
        static void RenderBegin();
        static void RenderEnd();
        static void DrawRect(Rect rect, ColorPurpose color);
        static void DrawLine(Vec2 startPos, Vec2 endPos, ColorPurpose color);
        static void DrawGlyph(int character, Vec2 pos, ColorPurpose color);
        static Vec2 GlyphMaxSize();
        static Vec2 MousePosition();

    private:

        static void CreateFont(int size, std::string fontName);

        static SDL_Window * _sdlWindow;    // sdl window is related to OS driver
        static SDL_Renderer * _sdlRenderer; // sdl renderer is related to sdl driver functionality itself
        static TTF_Font * _font;    // pointer to current font structure
        static SDL_Rect _glyphs[GLYPHS_AMOUNT]; // rects of created glyphs
        static SDL_Texture * _fontTexture[GLYPHS_AMOUNT]; // array of glyphs textures
        static Vec2 _glyphMaxSize;
        static Colors * _colors;    // pointer to colors singleton
        static int _dpiScaleFactor;  // because on high dpi screen points of sdl and pixels are different, we need to use scale factor for some functionality
        static Uint64 _frameStartTicks; // time in sdl ticks(it's milliseconds based on sdl documentation) when frame drawing has started
};
#endif // __GRAPHICS_HPP__
