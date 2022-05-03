#include <string>

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
    char c[2];
    SDL_Rect * glyphRect;
    SDL_Color whiteDefautSdlColor;  // default color

    whiteDefautSdlColor = { .r = 255, .g = 255, .b = 255};

    memset(&_glyphs, 0, sizeof(SDL_Rect) * 128);

    _font = TTF_OpenFont(fontFileName.c_str(), fontSize);

    surface = SDL_CreateRGBSurface(0, 512, 512, 32, 0, 0, 0, 0xff);

    // make surface transparent
    SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGBA(surface->format, 0, 0, 0, 0));

    dest.x = dest.y = 0;

    for (i = ' '; i <= 'z'; i++)
    {
        c[0] = i;
        c[1] = 0;

        text = TTF_RenderUTF8_Blended(_font, c, whiteDefautSdlColor);

        TTF_SizeText(_font, c, &dest.w, &dest.h);

        if (dest.x + dest.w >= 512)
        {
            dest.x = 0;

            dest.y += dest.h + 1;

            if (dest.y + dest.h >= 512)
            {
                SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_CRITICAL, "Out of glyph space in %dx%d font atlas texture map.", 512, 512);
                exit(1);
            }
        }

        SDL_BlitSurface(text, NULL, surface, &dest);

        glyphRect = &(_glyphs[i]);  // pointer to current set glyph

        glyphRect->x = dest.x;
        glyphRect->y = dest.y;
        glyphRect->w = dest.w;
        glyphRect->h = dest.h;

        SDL_FreeSurface(text);

        dest.x += dest.w;
    }

    _fontTexture = SDL_CreateTextureFromSurface(_mainRenderer, surface);
    SDL_FreeSurface(surface);
}