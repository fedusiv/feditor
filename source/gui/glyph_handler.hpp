#ifndef __GLYPH_HANDLER__
#define __GLYPH_HANDLER__

#include <string>

#include "SDL.h"
#include "SDL_ttf.h"

#include "vector2.hpp"

#define GLYPHS_AMOUNT 128

class GlyphHandler
{
    public:
        // signleton part
        static GlyphHandler* Instance()
        {
            if(_glyphSHanlder == nullptr)
            {
                _glyphSHanlder = new GlyphHandler();
            }
            return _glyphSHanlder;
        }
        // functionality
        void GlyphBuilder(int fontSize, std::string fontFileName);
        void SetRenderer(SDL_Renderer * renderer);

        void Glyph(int pos, SDL_Rect ** rect, SDL_Texture ** texture);
        Vector2 ElementSize();

    private:
        GlyphHandler();
        static GlyphHandler * _glyphSHanlder; // for singleton

        SDL_Renderer * _mainRenderer;
        TTF_Font * _font;
        SDL_Rect _glyphs[GLYPHS_AMOUNT];
        SDL_Texture * _fontTexture[GLYPHS_AMOUNT];

        Vector2 _elementSize;
};

#endif // __GLYPH_HANDLER__