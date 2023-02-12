#include "colors.hpp"

Colors * Colors::_colors;   // need because of static

Colors::Colors()
{
    SDL_Color windowBackgroundColor = {.r = 255, .g = 255, .b = 255, .a = 255};
    SDL_Color widgetBackgroundColor = {.r = 24, .g = 21, .b = 26, .a = 255};
    SDL_Color widgetCursorColor = {.r = 168, .g = 160, .b = 50, .a = 255};
    SDL_Color widgetEditorLineNumberColor = {.r = 120, .g = 123, .b = 156, .a = 255};
    SDL_Color widgetEditorText = {.r = 220, .g = 229, .b = 232, .a = 255};

    _colorsStorage.push_back(windowBackgroundColor);
    _colorsStorage.push_back(widgetBackgroundColor);
    _colorsStorage.push_back(widgetCursorColor);
    _colorsStorage.push_back(widgetEditorLineNumberColor);
    _colorsStorage.push_back(widgetEditorText);
}

SDL_Color Colors::GetColor(ColorPurpose pur)
{
    return _colorsStorage.at(pur);
}

Colors *Colors::Instance()
{
    if(_colors == nullptr)
    {
        _colors = new Colors();
    }

    return _colors;
}