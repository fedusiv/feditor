#ifndef __WIDGET_LABEL_HPP__
#define __WIDGET_LABEL_HPP__

#include "widget.hpp"
#include <string>
#include <vector>

class WidgetLabel: public Widget
{
    public:
        WidgetLabel(Rect rect);
        ~WidgetLabel();
        void Render(void) override;
        void Resize(Rect newRect) override;

    private:


};

#endif // __WIDGET_STATUSLINE_HPP__
