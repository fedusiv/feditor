#ifndef __WIDGET_EDITOR_HPP__
#define __WIDGET_EDITOR_HPP__

#include "vec2.hpp"
#include "widget.hpp"
#include "buffer.hpp"

class WidgetEditor: public Widget
{
    public:
        WidgetEditor(Rect rect, Buffer * buffer);
        void Render(void) override;

    private:
        Buffer * _buffer;
        int _textStartX; // this field need, because of line numbers, we will set on what distance starts to draw text after lines
        int _availableLines; // maximum amount of lines, that can be drawn on one editor page
        int _currentUpperLine;  // number of line, which is shown as first in editor page.
        int _linesPageMoveOffset;    // amount of lines on when cursor is close to upper or bottom line's border
        Vec2 _cursorPrevPos;         // previous position of cursor

        void DrawLinesNumber(void); // draw lines number
        void DrawData(void);    // draw actual data
        void PageUpdate(void);  // based on cursor, move editor page
        Vec2 CalculateRealPosForCursor();    // calculating real position on pixels, related to local coordinates for cursor
        void CalculateAvaliableLines();     // calculate amount of lines, which can be drawn on one editor page
};

#endif // __WIDGET_EDITOR_HPP__