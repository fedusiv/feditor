#include "widget.hpp"
#include "graphics.hpp"

Widget::Widget(Rect rect): _widgetFullRect(rect)
{
    _active = true;
    _widgetBorderThick = 2;
    _drawingOffset = Vec2(0,0);
    _cursorWidth = 2;
    _cursorHeightAdd = 2;
    _glyphSize = Graphics::GlyphMaxSize();
    _widgetRect = _widgetFullRect;
    _widgetRect -= _widgetBorderThick;

    _colorBgWidget = ColorPurpose::ColorWidgetBg;
    _colorBorderWidget = ColorPurpose::ColorWidgetBorder;
}

Widget::~Widget()
{
}

void Widget::Render()
{
    DrawBackground();
}

void Widget::DrawBackground()
{
    Graphics::DrawRect(_widgetFullRect,_colorBgWidget);
    Graphics::DrawRect(_widgetRect,_colorBorderWidget);
}

/*
*   x and y are relative pixel place
*/
void Widget::DrawCharacter(int character, Vec2 pos, ColorPurpose color)
{
    pos += _drawingOffset;
    // Adding offset of widget to real coordinates
    pos.x += _widgetRect.x;
    pos.y += _widgetRect.y;
    Graphics::DrawGlyph(character, pos, color);
}


/*
* Draw in x and y pos in relative coordinates
*/
void Widget::DrawCursor(Vec2 pos)
{
    Vec2 endpos;

    pos += _drawingOffset;
    pos.x += _widgetRect.x;
    pos.y += _widgetRect.y;
    endpos = pos;
    endpos.y += _glyphSize.y;
    endpos.x += _cursorWidth;
    pos.y -= _cursorHeightAdd;

    Graphics::DrawLine(pos, endpos, ColorPurpose::ColorWidgetCursor);
}

void Widget::CalculateDrawingOffset()
{
}

bool Widget::Active()
{
    return _active;
}

void Widget::SetActive(bool status)
{
    _active = status;
}

Rect Widget::GetRect()
{
    return _widgetFullRect;
}
