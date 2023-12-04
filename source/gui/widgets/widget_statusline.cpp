#include "widget_statusline.hpp"
#include "editor_state.hpp"
#include "vec2.hpp"
#include <string>

WidgetStatusLine::WidgetStatusLine(Rect rect): Widget(rect)
{
    Vec2 windowSize;
    
    _widgetType = WidgetType::WidgetTypeStatusLine;
    _drawingOffset = Vec2(_glyphSize.x,0);
    _currentFileName = "";
    _infoBorderString = " | ";

}

WidgetStatusLine::~WidgetStatusLine()
{

}

void WidgetStatusLine::Render()
{
    Widget::Render();
    DrawInformation();
}

void WidgetStatusLine::Resize(Rect newRect)
{
    Widget::Resize(newRect);
}

void WidgetStatusLine::DrawInformation()
{
    Vec2 startPos;
    ColorPurpose color;

    color = ColorPurpose::ColorStatusLineModalMode;
    // calculate start position. in status line everyhting need to be drawn based on size of font, and drawn in the middle
    int center = _widgetFullRect.h / 2;
    center -= _glyphSize.y / 2;
    startPos.y += center;

    // draw current filename
    for(auto c: _currentFileName)
    {
        DrawCharacter(static_cast<int>(c), startPos, color);
        startPos.x+= _glyphSize.x;
    }
    // draw border
    for(auto c: _infoBorderString)
    {
        DrawCharacter(static_cast<int>(c), startPos, color);
        startPos.x+= _glyphSize.x;
    }
    // draw tab name
    for(auto c: _currentTabName)
    {
        DrawCharacter(static_cast<int>(c), startPos, color);
        startPos.x+= _glyphSize.x;
    }
}

void WidgetStatusLine::UpdateFilename(std::string filename)
{
    _currentFileName = filename;
}

void WidgetStatusLine::UpdateTabName(std::string tabname)
{
    _currentTabName = tabname;
}