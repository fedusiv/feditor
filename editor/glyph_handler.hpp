#ifndef __GLYPH_HANDLER__
#define __GLYPH_HANDLER__

#include <string>

#include "SDL.h"
#include "SDL_ttf.h"

#include "utils.hpp"

class GlyphHandler
{
    public:
        GlyphHandler(SDL_Renderer * renderer);
        void GlyphBuilder(int fontSize, std::string fontFileName);

        SDL_Rect _glyphs[128];
        SDL_Texture * _fontTexture;

        Vector2 ElementSize();

    private:
        SDL_Renderer * _mainRenderer;

        TTF_Font * _font;

        Vector2 _elementSize;

        const int _sizeOfCharacterSprite = 512;
};

#endif // __GLYPH_HANDLER__