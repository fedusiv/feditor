#include "graphics.hpp"
#include "SDL_stdinc.h"

#include <SDL.h>

// This code area need because entire class is static
SDL_Window *    Graphics::_sdlWindow;    // sdl window is related to OS driver
SDL_Renderer *  Graphics::_sdlRenderer; // sdl renderer is related to sdl driver functionality itself
TTF_Font *      Graphics::_font;    // pointer to current font structure
SDL_Rect        Graphics::_glyphs[GLYPHS_AMOUNT]; // rects of created glyphs
SDL_Texture *   Graphics::_fontTexture[GLYPHS_AMOUNT]; // array of glyphs textures
Vec2            Graphics::_glyphMaxSize;
Colors *        Graphics::_colors;    // pointer to colors singleton
int             Graphics::_dpiScaleFactor;
Uint64          Graphics::_frameStartTicks; // time in sdl ticks when frame drawing has started

bool Graphics::Init(Vec2& windowSize, int fontSize)
{
    int w,h;

    _colors = Colors::Instance();
    TTF_Init();

    if( SDL_Init(SDL_INIT_EVERYTHING) > 0)
    {
        return false;
    }

    _sdlWindow = SDL_CreateWindow(
        "feditor",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        windowSize.x,
        windowSize.y,
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI
    );
    // Calculate scale factor
    SDL_GL_GetDrawableSize(_sdlWindow, &windowSize.x, &windowSize.y);   // get size in pixels.
    SDL_GetWindowSize(_sdlWindow, &w, &h);  // get size in points
    _dpiScaleFactor = windowSize.y / h; // calculated scale factor pixels / points

    _sdlRenderer = SDL_CreateRenderer(_sdlWindow,-1,SDL_RENDERER_ACCELERATED);  // create renderer, where everything will be drawn
    CreateFont(fontSize * _dpiScaleFactor, "assets/Iosevka.ttf");

    return true;
}

void Graphics::RenderBegin()
{
    SDL_Color bgColor;

    _frameStartTicks = SDL_GetTicks64();
    bgColor = _colors->GetColor(ColorPurpose::ColorWindowBg);
    SDL_SetRenderDrawColor(_sdlRenderer,   // set background for window
    bgColor.r,
    bgColor.g,
    bgColor.b,
    bgColor.a);
    SDL_RenderClear(_sdlRenderer);
}

void Graphics::RenderEnd()
{
    Uint64 duration;
    Uint32 delta;

    SDL_RenderPresent(_sdlRenderer);

    duration = SDL_GetTicks64() - _frameStartTicks;
    delta= 1000 / FPS;
    if (duration < delta) {
        SDL_Delay(delta - duration);
    }
}

void Graphics::DeInit()
{
    // destroy renderer
    SDL_DestroyRenderer(_sdlRenderer);
    // destroy window
    SDL_DestroyWindow(_sdlWindow);
    // Close ttf module
    TTF_Quit();
    // close SDL
    SDL_Quit();
}

void Graphics::CreateFont(int fontSize, std::string fontName)
{
    SDL_Surface * text;
    SDL_Rect glyphRect;
    SDL_Color whiteDefautSdlColor; // default color
    int i;

    whiteDefautSdlColor = { .r = 255, .g = 255, .b = 255, .a = 255}; // glyph will de drawn in white color, because white color can be recolored.

    memset(&_glyphs, 0, sizeof(SDL_Rect) * GLYPHS_AMOUNT);

    _font = TTF_OpenFont(fontName.c_str(), fontSize);
    if(_font == NULL)
    {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_CRITICAL, "Can not open provided font by filename");
    }

    TTF_SetFontHinting(_font, TTF_HINTING_LIGHT_SUBPIXEL);
    TTF_SetFontKerning(_font,1);

    // Save characters
    // from 32 to 126.
    for (i = ' '; i <= '~'; i++)
    {
        text = TTF_RenderGlyph32_Blended(_font, i, whiteDefautSdlColor);    // render glyph from font in argb32 format. This is highest quality of rendering text I made
        _fontTexture[i] = SDL_CreateTextureFromSurface(_sdlRenderer, text);    // create texture, which will be used
        glyphRect.w = text->w;
        glyphRect.h = text->h;
        _glyphs[i] = glyphRect;

        // fill size of one character to be used
        // setting max size of glyph
        if(glyphRect.w > _glyphMaxSize.x)
        {
            _glyphMaxSize.x = glyphRect.w;
        }
        if(glyphRect.h > _glyphMaxSize.y)
        {
            _glyphMaxSize.y = glyphRect.h;
        }

        SDL_FreeSurface(text);
    }

    TTF_CloseFont(_font);
}

void Graphics::DrawRect(Rect rect, ColorPurpose color)
{
    SDL_Rect rectSdl;
    SDL_Color colorSdl;
    
    colorSdl = _colors->GetColor(color);

    rectSdl = {
        .x = rect.x,
        .y = rect.y,
        .w = rect.w,
        .h = rect.h,
    };
    SDL_SetRenderDrawColor(_sdlRenderer, colorSdl.r, colorSdl.g, colorSdl.b, colorSdl.a);
    SDL_RenderFillRect(_sdlRenderer, &rectSdl);
}

void Graphics::DrawLine(Vec2 startPos, Vec2 endPos, ColorPurpose color)
{
    SDL_Rect rect;
    SDL_Color colorSdl;
    
    colorSdl = _colors->GetColor(color);

    rect = {
        .x = startPos.x,
        .y = startPos.y,
        .w = endPos.x - startPos.x,
        .h = endPos.y - startPos.y,
    };
    SDL_SetRenderDrawColor(_sdlRenderer, colorSdl.r, colorSdl.g, colorSdl.b, colorSdl.a);
    SDL_RenderFillRect(_sdlRenderer, &rect);
}

void Graphics::DrawGlyph(int character, Vec2 pos, ColorPurpose color)
{
    SDL_Rect * glyphRect;
    SDL_Rect dest;
    SDL_Texture * texture;
    SDL_Color colorSDL;

    colorSDL = _colors->GetColor(color);
    texture = _fontTexture[character];
    glyphRect = &_glyphs[character];

    SDL_SetTextureColorMod(texture, colorSDL.r, colorSDL.g, colorSDL.b);
    SDL_SetTextureAlphaMod(texture,colorSDL.a);

    dest.x = pos.x;
    dest.y = pos.y;
    dest.w = glyphRect->w;
    dest.h = glyphRect->h;

    SDL_RenderCopy(_sdlRenderer, texture, NULL, &dest);
}

Vec2 Graphics::GetAppSize()
{
    int w;
    int h;
    
    SDL_GL_GetDrawableSize(_sdlWindow, &w, &h); // this is for high DPI
    auto res = Vec2(w,h);
    return res;
}

Vec2 Graphics::GlyphMaxSize()
{
    return _glyphMaxSize;
}

Vec2 Graphics::MousePosition(void)
{
    Vec2 position;

    SDL_GetMouseState(&position.x, &position.y);    // obtain mouse position
    position *= _dpiScaleFactor;
    return position;
}

