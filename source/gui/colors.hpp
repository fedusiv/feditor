#ifndef __COLORS_HPP__
#define __COLORS_HPP__

#include "SDL.h"
#include <vector>

enum ColorPurpose
{
    ColorWindowBg,
    ColorWidgetBg,
    ColorWidgetCursor,
    ColorWidgetEditorLineNumber,
    ColorWidgetEditorText,
    ColorPurposeMax
};

class Colors
{
    public:
        static Colors* Instance();
        SDL_Color GetColor(ColorPurpose pur);

    private:
        Colors();
        static Colors * _colors;
        std::vector<SDL_Color> _colorsStorage;
};

#endif // __COLORS_HPP__