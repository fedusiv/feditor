 #include <string>
#include <iostream>

#include "SDL.h"
#include "SDL_ttf.h"

#include "glyph_handler.hpp"

GlyphHandler * GlyphHandler::_glyphSHanlder; 

GlyphHandler::GlyphHandler()
{

}


void GlyphHandler::GlyphBuilder(int fontSize, std::string fontFileName)
{

    SDL_Surface * text;
    SDL_Rect glyphRect;
    SDL_Color whiteDefautSdlColor; // default color
    int i;
    uint32_t pixelFormat;

    _elementSize.x = 0;
    _elementSize.y = 0;

    whiteDefautSdlColor = { .r = 255, .g = 255, .b = 255, .a = 255}; // glyph will de drawn in white color, because white color can be recolored.
    pixelFormat = SDL_PIXELFORMAT_ARGB32;

    memset(&_glyphs, 0, sizeof(SDL_Rect) * GLYPHS_AMOUNT);

    _font = TTF_OpenFont(fontFileName.c_str(), fontSize);
    if(_font == NULL)
    {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_CRITICAL, "Can not open provided font by filename");
    }

    // Save characters
    // from 32 to 126.
    for (i = ' '; i <= '~'; i++)
    {
        text = TTF_RenderGlyph32_Blended(_font, i, whiteDefautSdlColor);    // render glyph from font in argb32 format. This is highest quality of rendering text I made
        _fontTexture[i] = SDL_CreateTextureFromSurface(_mainRenderer, text);    // create texture, which will be used
        SDL_QueryTexture(_fontTexture[i], &pixelFormat, nullptr, &text->w, &text->h); // double check, to make sure, that texture will use argb32 pixel format
        glyphRect.w = text->w;
        glyphRect.h = text->h;
        _glyphs[i] = glyphRect;

        // fill size of one character to be used
        // setting max size of glyph
        if(glyphRect.w > _elementSize.x)
        {
            _elementSize.x = glyphRect.w;
        }
        if(glyphRect.h > _elementSize.y)
        {
            _elementSize.y = glyphRect.h;
        }

        SDL_FreeSurface(text);
    }

    TTF_CloseFont(_font);
}

void GlyphHandler::SetRenderer(SDL_Renderer *renderer)
{
    _mainRenderer = renderer;
}

void GlyphHandler::Glyph(int pos, SDL_Rect ** rect, SDL_Texture **texture)
{
    *rect = &_glyphs[pos];
    *texture = _fontTexture[pos];
}

Vector2 GlyphHandler::ElementSize()
{
    return _elementSize;
}
