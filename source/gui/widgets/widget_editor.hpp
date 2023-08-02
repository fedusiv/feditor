#ifndef __WIDGET_EDITOR_HPP__
#define __WIDGET_EDITOR_HPP__

#include "graphics.hpp"
#include "vec2.hpp"
#include "widget.hpp"
#include "buffer.hpp"

#define SCROLL_VERTICAL_STEP 3
#define SCROLL_HORIZONTAL_STEP 2

class WidgetEditor: public Widget
{
    public:
        WidgetEditor(Rect rect, Buffer * buffer);
        void Render(void) override;
        void Resize(Rect newRect) override;
        void SetCursorPosition(Vec2 position) override; // setting cursor position of widget editor
        void PageScrolling(Vec2 direction, Vec2 mousePosition) override; // moving editor page based on explicit commands to scroll page

    protected:
        void CalculateDrawingOffset(void) override;

    private:
        Buffer * _buffer;
        int _textStartX; // this field need, because of line numbers, we will set on what distance starts to draw text after lines
        int _availableLines; // maximum amount of lines, that can be drawn on one editor page
        int _availableColumns;  // maximum amount of characters, can be placed in one line on one editor page
        int _currentUpperLine;  // number of line, which is shown as first in editor page.
        int _linesPageMoveOffset;    // amount of lines on when cursor is close to upper or bottom line's border
        int _currentLeftLine;   // number of left offset to draw characters
        int _linesPageShiftOffset; // shift offset is also page moving but on horizontally
        Vec2 _cursorPrevPos;         // previous position of cursor

        void DrawLinesNumber(void); // draw lines number
        void DrawData(void);    // draw actual data
        void PageUpdate(void);  // based on cursor, move editor page
        Vec2 CalculateRealPosForCursor();    // calculating real position on pixels, related to local coordinates for cursor
        void CalculateAvaliableLines();     // calculate amount of lines, which can be drawn on one editor page
        void CalculateAvaliableColumns();   // calculate amount of columns, width of line in one editor page
        void IncreaseUpperLine(int value);  // increase _currentUpperLine by provided value
        void IncreaseLeftLine(int value);   // increase _currentLeftLine by provided value
};

#endif // __WIDGET_EDITOR_HPP__
