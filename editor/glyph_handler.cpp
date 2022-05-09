#include <string>
#include <iostream>

#include "SDL.h"
#include "SDL_ttf.h"

#include "glyph_handler.hpp"

GlyphHandler::GlyphHandler(SDL_Renderer * renderer) : _mainRenderer(renderer)
{

}


void GlyphHandler::GlyphBuilder(int fontSize, std::string fontFileName)
{
    SDL_Surface * surface, * text;
    SDL_Rect dest;
    int i;
    unsigned short c[2];
    SDL_Rect * glyphRect;
    SDL_Color whiteDefautSdlColor;  // default color

    _elementSize.x = 0;
    _elementSize.y = 0;

    whiteDefautSdlColor = { .r = 255, .g = 255, .b = 255, .a = 255};

    memset(&_glyphs, 0, sizeof(SDL_Rect) * 128);

    _font = TTF_OpenFont(fontFileName.c_str(), fontSize);
    if(_font == NULL)
    {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_CRITICAL, "Can not open provided font by filename");
    }

    surface = SDL_CreateRGBSurface(0, _sizeOfCharacterSprite, _sizeOfCharacterSprite, 32, 0, 0, 0, 0xff);

    // make surface transparent
    SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGBA(surface->format, 0, 0, 0, 0));

    dest.x = dest.y = 0;


    // Save characters
    // from 32 to 126.
    for (i = ' '; i <= '~'; i++)
    {
        c[0] = i;
        c[1] = 0;

        text = TTF_RenderUNICODE_Blended(_font, c, whiteDefautSdlColor);

        TTF_SizeUNICODE(_font, c, &dest.w, &dest.h);

        if (dest.x + dest.w >= _sizeOfCharacterSprite)
        {
            dest.x = 0;

            dest.y += dest.h + 1;

            if (dest.y + dest.h >= _sizeOfCharacterSprite)
            {
                SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_CRITICAL, "Out of glyph space in %dx%d font atlas texture map.", _sizeOfCharacterSprite, _sizeOfCharacterSprite);
                exit(1);
            }
        }

        SDL_BlitSurface(text, NULL, surface, &dest);

        glyphRect = &(_glyphs[i]);  // pointer to current set glyph

        glyphRect->x = dest.x;
        glyphRect->y = dest.y;
        glyphRect->w = dest.w;
        glyphRect->h = dest.h;

        // fill size of one character to be used
        if(glyphRect->w > _elementSize.x)
        {
            _elementSize.x = glyphRect->w;
        }
        if(glyphRect->h > _elementSize.y)
        {
            _elementSize.y = glyphRect->h;
        }

        SDL_FreeSurface(text);

        dest.x += dest.w;
    }

    _fontTexture = SDL_CreateTextureFromSurface(_mainRenderer, surface);
    SDL_FreeSurface(surface);
    TTF_CloseFont(_font);
}

Vector2 GlyphHandler::ElementSize()
{
    return _elementSize;
}
