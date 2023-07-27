#include "widget_statusline.hpp"
#include "editor_state.hpp"
#include "vec2.hpp"

WidgetStatusLine::WidgetStatusLine(Rect rect): Widget(rect)
{
    Vec2 windowSize;
    
    _widgetType = WidgetType::WidgetTypeStatusLine;
    windowSize = Vec2(rect.x,rect.y);   // get current window size
    //Resize(windowSize);
    _drawingOffset = Vec2(_glyphSize.x,0);

    _currentEditorState = EditorState::InsertState;
    _editorStateName[EditorState::InsertState] = "INSERT";
    _editorStateName[EditorState::NormalState] = "NORMAL";
}

WidgetStatusLine::~WidgetStatusLine()
{
}

void WidgetStatusLine::Render()
{
    Widget::Render();
    DrawCurrentMode();
}

void WidgetStatusLine::Resize(Rect newRect)
{
    // Rect rect;
    // // First calculate real Rectangle for Statusline
    // rect.w = windowSize.x;// window size x
    // rect.h = _glyphSize.y + (_glyphSize.y / 5); // size of status line is 10% of font size in up then fontsize goes and after that again 10%
    // rect.x = 0;
    // rect.y = windowSize.y - rect.h;
    // UpdateWidgetRect(rect);
}

void WidgetStatusLine::DrawCurrentMode()
{
    std::string modeName;
    Vec2 startPos;
    ColorPurpose color;

    modeName = _editorStateName[_currentEditorState];
    switch (_currentEditorState) {
        case EditorState::NormalState:
            color = ColorPurpose::ColorStatusLineModalMode;
            break;
        case EditorState::InsertState:
            color = ColorPurpose::ColorStatusLineInsertMode;
            break;
        default:
            color = ColorPurpose::ColorStatusLineModalMode;
            break;
    }

    // calculate start position. in status line everyhting need to be drawn based on size of font, and drawn in the middle
    int center = _widgetFullRect.h / 2;
    center -= _glyphSize.y / 2;
    startPos.y += center;

    for(auto c: modeName)
    {
        DrawCharacter(static_cast<int>(c), startPos, color);
        startPos.x+= _glyphSize.x;
    }
}
