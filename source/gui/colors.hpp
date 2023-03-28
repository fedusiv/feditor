#ifndef __COLORS_HPP__
#define __COLORS_HPP__

#include <SDL.h>

enum ColorPurpose
{
    ColorWindowBg,
    ColorWidgetBg,
    ColorWidgetBorder,
    ColorWidgetCursor,
    ColorWidgetEditorLineNumber,
    ColorWidgetEditorText,
    ColorStatusLineModalMode,
    ColorStatusLineInsertMode,

    ColorWhite,
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
        SDL_Color _colorsStorage[ColorPurpose::ColorPurposeMax];
};

#endif // __COLORS_HPP__