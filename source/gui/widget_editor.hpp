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
        int _textStartY; // this field need, because of line numbers, we will set on what distance starts to draw text after lines

        void DrawLinesNumber(void);
};

#endif // __WIDGET_EDITOR_HPP__