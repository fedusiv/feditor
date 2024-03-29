#include "widget.hpp"
#include "colors.hpp"
#include "graphics.hpp"
#include "vec2.hpp"

Widget::Widget(Rect rect): _widgetFullRect(rect)
{
    _widgetType = WidgetType::WidgetTypeBase;
    _active = true;
    _widgetBorderThick = WIDGET_BORDER_THICK;
    _drawingOffset = Vec2(0,0);
    _cursorWidth = 4;
    _cursorHeightAdd = 2;
    _glyphSize = Graphics::GlyphMaxSize();
    _layerRender = WIDGET_LAYER_DEFAULT ; // default render layer level
 
    UpdateWidgetRect(_widgetFullRect);
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

void Widget::Resize(Rect newRect)
{
    UpdateWidgetRect(newRect);
}

void Widget::DrawBackground()
{
    Graphics::DrawRect(_widgetFullRect,_colorBorderWidget);
    Graphics::DrawRect(_widgetRect,_colorBgWidget);
}

/*
* DrawRect is in obsolete coordinates
*/
void Widget::DrawRect(Rect rect, ColorPurpose color)
{
    Graphics::DrawRect(rect,color);
}

void Widget::DrawRect(Rect rect, int thickness, ColorPurpose colorBorder, ColorPurpose colorBg)
{
    Rect bgRect;
    bgRect = rect;
    bgRect -= 1;
    Graphics::DrawRect(rect,colorBorder);
    Graphics::DrawRect(bgRect,colorBg);
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

void Widget::SetBackgroundColor(ColorPurpose color)
{
    _colorBgWidget = color;
}

int Widget::GetRenderLayer() const
{
    return _layerRender;
}

void Widget::SetRenderLayer(int layerLevel)
{
    _layerRender = layerLevel;
}

bool Widget::LayerComparator(const Widget* w1, const Widget* w2)
{
    return w1->GetRenderLayer() < w2->GetRenderLayer();
}

