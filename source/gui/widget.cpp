#include "widget.hpp"
#include "graphics.hpp"
#include "vec2.hpp"

Widget::Widget(Rect rect): _widgetFullRect(rect)
{
    _widgetType = WidgetType::WidgetTypeBase;
    _active = true;
    _widgetBorderThick = 2;
    _drawingOffset = Vec2(0,0);
    _cursorWidth = 4;
    _cursorHeightAdd = 2;
    _glyphSize = Graphics::GlyphMaxSize();
 
    UpdateWidgetRect(_widgetFullRect);
    _colorBgWidget = ColorPurpose::ColorWidgetBg;
    _colorBorderWidget = ColorPurpose::ColorWidgetBorder;
    _currentEditorState = EditorState::NormalState;
}

Widget::~Widget()
{
}

void Widget::Render()
{
    DrawBackground();
}

void Widget::Resize(Rect newRect)
{
    UpdateWidgetRect(_widgetFullRect);
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

void Widget::UpdateWidgetRect(Rect fullRect)
{
    _widgetFullRect = fullRect;
    _widgetRect = _widgetFullRect;
    _widgetRect -= _widgetBorderThick;
}

bool Widget::IsInWidget(Vec2 position)
{
    // check if position okay for x coordinate
    if(_widgetFullRect.x < position.x && _widgetFullRect.x + _widgetFullRect.w > position.x)
    {
        // check if position okay for y coordinate
        if(_widgetFullRect.y < position.y && _widgetFullRect.y + _widgetFullRect.h > position.y)
        {
            return true;
        }
    }

    return false;
}

void Widget::SetEditorState(EditorState state)
{
    _currentEditorState = state;
}

WidgetType Widget::GetWidgetType(void)
{
    return _widgetType;
}

void Widget::SetCursorPosition(Vec2 position)
{
    // no functionality
}

void Widget::PageScrolling(Vec2 direction, Vec2 mousePosition)
{
    // no functionality
}
