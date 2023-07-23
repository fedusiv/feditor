#ifndef __WIDGET_TAB_HPP__
#define __WIDGET_TAB_HPP__

#include "graphics.hpp"
#include "vec2.hpp"
#include "widget.hpp"
#include "widget_editor.hpp"
#include "buffer.hpp"

class WidgetTab: public Widget
{
    public:
        WidgetTab(Rect rect);
        void Render(void) override;
        void SetCursorPosition(Vec2 position) override; // setting cursor position of widget
        void PageScrolling(Vec2 direction, Vec2 mousePosition) override; // moving editor page based on explicit commands to scroll page
        void AttachBuffer(Buffer * buffer);         // attaching buffer to tab. Creates widget editor

    protected:
        void CalculateDrawingOffset(void) override;

    private:
        std::list<WidgetEditor*> _widgetsEditorList; // all editor widgets in gui window

};

#endif // __WIDGET_TAB_HPP__