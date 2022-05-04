#ifndef __GLYPH_HANDLER__
#define __GLYPH_HANDLER__

#include <string>

#include "SDL.h"
#include "SDL_ttf.h"

class GlyphHandler
{
    public:
        GlyphHandler(SDL_Renderer * renderer);
        void GlyphBuilder(int fontSize, std::string fontFileName);

        SDL_Rect _glyphs[128];
        SDL_Texture * _fontTexture;

    private:
        SDL_Renderer * _mainRenderer;

        TTF_Font * _font;
};

#endif // __GLYPH_HANDLER__