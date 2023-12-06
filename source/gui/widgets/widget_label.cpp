#include "widget_label.hpp"

void WidgetLabel::InitDefault()
{
    CalculateMinimalSize();
    _aligment = LabelWriteAligment::LeftAligment;
    _widgetBorderThick = 1;
}

WidgetLabel::WidgetLabel(Rect rect): Widget(rect)
{
    _widgetType = WidgetType::WidgetTypeLabel;
    _textColor = ColorWhite;
    _glyphOffset = Vec2(0,0);
    InitDefault();
}

WidgetLabel::WidgetLabel(Rect rect, std::string text): Widget(rect)
{
    _widgetType = WidgetType::WidgetTypeLabel;
    _textColor = ColorWhite;
    _text = text;
    _glyphOffset = Vec2(0,0);
    InitDefault();
}

WidgetLabel::WidgetLabel(Rect rect, std::string text, ColorPurpose color, Vec2 glyphOffset): Widget(rect), _text(text), _textColor(color), _glyphOffset(glyphOffset)
{
    _widgetType = WidgetType::WidgetTypeLabel;
    _widgetType = WidgetTypeLabel;
    InitDefault();
}

WidgetLabel::~WidgetLabel()
{

}
void WidgetLabel::Render(void)
{
    Widget::Render();
    switch(_aligment)
    {
        case LabelWriteAligment::LeftAligment:
            DrawLeftAligment();
            break;
        case LabelWriteAligment::CenterAligment:
            DrawCenterAligment();
            break;
        default:
            break;
    }

}

void WidgetLabel::DrawLeftAligment(void)
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

void WidgetLabel::DrawCenterAligment(void)
{
    Vec2 posCentral, drawPoint;
    int halfTextSize;
    // First need to find central position for letter.
    posCentral.x = (_widgetRect.w) / 2;
    posCentral.y = (_widgetRect.h) / 2;
    // next size of text
    halfTextSize = _text.size() / 2;
    // calculate size of startig point
    drawPoint.y = posCentral.y - _glyphSize.y / 2;
    drawPoint.x = posCentral.x - halfTextSize * _glyphSize.x;
    
    for(auto c : _text)
    {
        DrawCharacter(c, drawPoint, _textColor);
        drawPoint.x += _glyphSize.x;
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

void WidgetLabel::CalculateMinimalSize()
{
    Rect newRect;
    int requiredWidth;

    if(_widgetRect.h < _glyphSize.y)
    {
        newRect.h = _glyphSize.y + (_glyphOffset.y * _glyphSize.y);
    }

    requiredWidth = _text.size() * _glyphSize.x;
    if(_widgetRect.w < requiredWidth)
    {
        newRect.w = requiredWidth + (_glyphOffset.x * _glyphSize.x);
    }
    if( (newRect.h | newRect.w) != 0)
    {
        newRect.y = _widgetRect.y;
        newRect.x = _widgetRect.x;
        Resize(newRect);
    }
}

void WidgetLabel::SetAligment(LabelWriteAligment aligment)
{
    _aligment = aligment;
}

void WidgetLabel::SetTextColor(ColorPurpose color)
{
    _textColor = color;
}

std::string WidgetLabel::GetText()
{
    return _text;
}