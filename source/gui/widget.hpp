#ifndef __WIDGET_HPP__
#define __WIDGET_HPP__

#include "colors.hpp"
#include "vec2.hpp"

class Widget
{
    public:

        Widget(Rect rect);
        virtual ~Widget();
        virtual void Render(void);

        bool Active();
        void SetActive(bool status);

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

        void DrawCharacter(int character, Vec2 pos, ColorPurpose color);
        void DrawBackground();
        void DrawCursor(Vec2 pos); // draw cursor at given position
        virtual void CalculateDrawingOffset(void);
};
#endif // __WIDGET_HPP__
