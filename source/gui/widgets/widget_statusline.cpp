#include "widget_statusline.hpp"
#include "editor_state.hpp"
#include <gui_configs.hpp>
#include <vec2.hpp>
#include <string>

WidgetStatusLine::WidgetStatusLine(Rect rect): Widget(rect)
{
    Vec2 windowSize;
    
    _widgetType = WidgetType::WidgetTypeStatusLine;
    _drawingOffset = Vec2(_glyphSize.x,0);
    _currentFileName = "---";
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

    if(_currentEditorState == EditorState::InsertState){
        // draw current filename
        DrawOneLine(startPos, _currentFileName, color);
        // draw border
        DrawOneLine(startPos, _infoBorderString, color);
        // draw tab name
        DrawOneLine(startPos, _currentTabName, color);
    }else
    if(_currentEditorState == EditorState::CmdState){
        DrawOneLine(startPos, GuiConstants::CMD_STRING, color);
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

void WidgetStatusLine::DrawOneLine(Vec2& pos, std::string line, ColorPurpose color)
{
    for(auto c: line){
        DrawCharacter(static_cast<int>(c), pos, color);
        pos.x+= _glyphSize.x;
    }
}