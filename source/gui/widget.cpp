#include "widget.hpp"
#include "colors.hpp"

SDL_Renderer * Widget::_sdlRenderer;    // static variable need to be also defined in cpp file.

Widget::Widget(Vector2 size, Vector2 location): _widgetSize(size), _widgetLocation(location)
{
    _colorStorage = Colors::Instance();
    _glyphHandler = GlyphHandler::Instance();
}

void Widget::Render()
{
    SDL_Color widgetBg;

    widgetBg = _colorStorage->GetColor(ColorPurpose::ColorWidgetBg);
    DrawBackground(widgetBg);
}

void Widget::SetSdlRenderer(SDL_Renderer *renderer)
{
    _sdlRenderer = renderer;
}

void Widget::DrawBackground(SDL_Color color)
{
    SDL_Rect rect;
    
    rect = {
        .x = _widgetLocation.x,
        .y = _widgetLocation.y,
        .w = _widgetSize.x,
        .h = _widgetSize.y,
    };
    SDL_SetRenderDrawColor(_sdlRenderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(_sdlRenderer, &rect);
}

/*
*   x and y are real pixel place
*/
void Widget::DrawCharacter(int character, Vector2 pos, SDL_Color color)
{
    SDL_Rect * glyphRect;
    SDL_Rect dest;
    SDL_Texture * texture;

    _glyphHandler->Glyph(character, &glyphRect, &texture);

    SDL_SetTextureColorMod(texture, color.r, color.g, color.b);
    SDL_SetTextureAlphaMod(texture,color.a);

    dest.x = pos.x + _widgetLocation.x;
    dest.y = pos.y + _widgetLocation.y;
    dest.w = glyphRect->w;
    dest.h = glyphRect->h;

    SDL_RenderCopy(_sdlRenderer, texture, NULL, &dest);
}



/*
* Draw in x and y pos in real coordinates
*/
void Widget::DrawCursor(Vector2 pos)
{
    SDL_Rect cursor;
    SDL_Color color;

    cursor = {
        .x = pos.x + _widgetLocation.x,
        .y = pos.y + _widgetLocation.y,
        .w = 2,
        .h = _glyphHandler->ElementSize().y,
    };
    color = _colorStorage->GetColor(ColorPurpose::ColorWidgetCursor);
    SDL_SetRenderDrawColor(_sdlRenderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(_sdlRenderer, &cursor);
}