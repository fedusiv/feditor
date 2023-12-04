#ifndef __WIDGET_BUTTON_HPP__
#define __WIDGET_BUTTON_HPP__

#include <list>
#include <vector>
#include <utility>
#include "graphics.hpp"
#include "vec2.hpp"
#include "widget.hpp"
#include "widget_label.hpp"
#include "gui_layout.hpp"

class WidgetButton: public Widget
{
    public:
        WidgetButton(Rect rect);
        void Render(void) override;
        void Resize(Rect newRect) override;
        void SetCursorPosition(Vec2 position) override; // setting cursor position of widget


    protected:

    private:

};


#endif // __WIDGET_BUTTON_HPP__