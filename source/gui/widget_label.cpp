#include "widget_label.hpp"


WidgetLabel::WidgetLabel(Rect rect): Widget(rect)
{
    _textColor = ColorWhite;
    _glyphOffset = Vec2(0,0);
}

WidgetLabel::WidgetLabel(Rect rect, std::string text): Widget(rect)
{
    _textColor = ColorWhite;
    _text = text;
    _glyphOffset = Vec2(0,0);
}

WidgetLabel::WidgetLabel(Rect rect, std::string text, ColorPurpose color, Vec2 glyphOffset): Widget(rect), _text(text), _textColor(color), _glyphOffset(glyphOffset)
{
    _widgetType = WidgetTypeLabel;
}

WidgetLabel::~WidgetLabel()
{

}
void WidgetLabel::Render(void)
{
    Vec2 pos;
    pos += _glyphOffset;
    pos *= _glyphSize;
    for(auto c : _text)
    {
        DrawCharacter(c, pos, _textColor);
        pos.x += _glyphSize.x;
    }
}

void WidgetLabel::Resize(Rect newRect)
{
    Widget::Resize(newRect);
}

void WidgetLabel::SetText(std::string text)
{
    _text = text;
}
