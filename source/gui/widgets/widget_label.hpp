#ifndef __WIDGET_LABEL_HPP__
#define __WIDGET_LABEL_HPP__

#include <string>
#include <vector>

#include "colors.hpp"
#include "widget.hpp"

enum class LabelWriteAligment
{
    LeftAligment,
    CenterAligment
};

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
        void SetAligment(LabelWriteAligment aligment);
        void SetTextColor(ColorPurpose color);
        std::string GetText();

    private:
        void InitDefault(); // execute default init for many constructors
        void CalculateMinimalSize();    // calculates minimal required size for label, to display whole message
        void DrawLeftAligment();
        void DrawCenterAligment();
        LabelWriteAligment _aligment;
        std::string _text;
        ColorPurpose _textColor;
        Vec2 _glyphOffset;

};

#endif // __WIDGET_STATUSLINE_HPP__
