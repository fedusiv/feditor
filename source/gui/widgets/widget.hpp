#ifndef __WIDGET_HPP__
#define __WIDGET_HPP__

#include "colors.hpp"
#include "vec2.hpp"
#include "editor_state.hpp"

enum WidgetType
{
    WidgetTypeBase,
    WidgetTypeEditor,
    WidgetTypeStatusLine,
    WidgetTypeTab,
    WidgetTypeLabel,
    WidgetTypeMax
};

class Widget
{
    public:

        Widget(Rect rect);
        virtual ~Widget();
        virtual void Render(void);
        virtual void Resize(Rect newRect);

        bool Active();
        virtual void SetActive(bool status);
        void SetEditorState(EditorState state);
        bool IsInWidget(Vec2 position);  // check if given position coordinates is related to this widget
        virtual void SetCursorPosition(Vec2 position); // if widget has functionality for cursor, it will update position. Inherited widget class need to override this. By default it does nothing
        virtual void PageScrolling(Vec2 direction, Vec2 mousePosition); // moving page based on explicit commands to scroll page
        WidgetType GetWidgetType(void);     // get widget type

        Rect GetRect();


    protected:

        Rect _widgetFullRect;  // widget size and location related to real pixel position in window
        Rect _widgetRect; // rect of widget without borders

        Vec2 _glyphSize;
        int _cursorWidth; // width in pixels for cursor
        int _cursorHeightAdd; // amount of pixel to increase height of cursor
        
        bool _active;   // means, that user currently is operating with this widget
        int _widgetBorderThick;
        Vec2 _drawingOffset;     // offset in pixels from border of widget to begin draw.

        ColorPurpose _colorBgWidget;    // colors to draw background
        ColorPurpose _colorBorderWidget;// colors to draw background

        EditorState _currentEditorState;
        WidgetType _widgetType;

        void DrawCharacter(int character, Vec2 pos, ColorPurpose color);
        void DrawBackground();
        void DrawCursor(Vec2 pos); // draw cursor at given position
        void UpdateWidgetRect(Rect fullRect);
        virtual void CalculateDrawingOffset(void);
};
#endif // __WIDGET_HPP__
