#ifndef __WIDGET_LABEL_HPP__
#define __WIDGET_LABEL_HPP__

#include <string>
#include <vector>

#include "colors.hpp"
#include "widget.hpp"

class WidgetLabel: public Widget
{
    public:
        WidgetLabel(Rect rect);
        WidgetLabel(Rect rect, std::string text);
        WidgetLabel(Rect rect, std::string text, ColorPurpose color, Vec2 glyphOffset);
        ~WidgetLabel();
        void Render(void) override;
        void Resize(Rect newRect) override;
        void SetText(std::string text);

    private:
        std::string _text;
        ColorPurpose _textColor;
        Vec2 _glyphOffset;

};

#endif // __WIDGET_STATUSLINE_HPP__
