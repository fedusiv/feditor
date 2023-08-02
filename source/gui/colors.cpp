#include "colors.hpp"

Colors * Colors::_colors;   // need because of static

Colors::Colors()
{
    // Default values for coloring
    _colorsStorage[ColorPurpose::ColorWindowBg] = {.r = 255, .g = 255, .b = 255, .a = 255};
    _colorsStorage[ColorPurpose::ColorWidgetBg] = {.r = 24, .g = 21, .b = 26, .a = 255};
    _colorsStorage[ColorPurpose::ColorWidgetBorder] = {.r = 14, .g = 11, .b = 16, .a = 255};
    _colorsStorage[ColorPurpose::ColorWidgetCursor] = {.r = 168, .g = 160, .b = 50, .a = 255};
    _colorsStorage[ColorPurpose::ColorWidgetEditorLineNumber] = {.r = 120, .g = 123, .b = 156, .a = 255};
    _colorsStorage[ColorPurpose::ColorWidgetEditorText] = {.r = 67, .g = 240, .b = 110, .a = 255};
    _colorsStorage[ColorPurpose::ColorWidgetEditorFileName] = {.r = 67, .g = 205, .b = 240, .a = 255};
    _colorsStorage[ColorPurpose::ColorStatusLineModalMode] = {.r = 0, .g = 240, .b = 0, .a = 255};
    _colorsStorage[ColorPurpose::ColorStatusLineInsertMode] = {.r = 135, .g = 135, .b = 212, .a = 255};


    _colorsStorage[ColorPurpose::ColorWhite] = {.r = 240, .g = 240, .b = 240, .a = 240};
}

SDL_Color Colors::GetColor(ColorPurpose pur)
{
    return _colorsStorage[pur];
}

Colors *Colors::Instance()
{
    if(_colors == nullptr)
    {
        _colors = new Colors();
    }

    return _colors;
}
