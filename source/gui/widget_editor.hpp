#ifndef __WIDGET_EDITOR_HPP__
#define __WIDGET_EDITOR_HPP__

#include "widget.hpp"
#include "../common/buffer.hpp"

class WidgetEditor: public Widget
{
    public:
        WidgetEditor(Vector2 size, Vector2 location, Buffer * buffer);
        void Render(void) override;

    private:
        Buffer * _buffer;
        int _textStartX; // this field need, because of line numbers, we will set on what distance starts to draw text after lines
        Vector2 _glyphSize; // current glyph size for rendering

        void DrawLinesNumber(void); // draw lines number
        void DrawData(void);    // draw actual data
        Vector2 CalculateRealPosForCursor();    // calculating real position on pixels, related to local coordinates for cursor
};

#endif // __WIDGET_EDITOR_HPP__