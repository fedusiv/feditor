#ifndef __WIDGET_LABEL_HPP__
#define __WIDGET_LABEL_HPP__


#include <fstring.hpp>
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
        WidgetLabel(Rect rect, FString text);
        WidgetLabel(Rect rect, FString text, ColorPurpose color, Vec2 glyphOffset);
        ~WidgetLabel();
        void Render(void) override;
        void Resize(Rect newRect) override;
        void SetText(FString text);
        void SetAligment(LabelWriteAligment aligment);
        void SetTextColor(ColorPurpose color);
        FString GetText();

    private:
        void InitDefault(); // execute default init for many constructors
        void CalculateMinimalSize();    // calculates minimal required size for label, to display whole message
        void DrawLeftAligment();
        void DrawCenterAligment();
        LabelWriteAligment _aligment;
        FString _text;
        ColorPurpose _textColor;
        Vec2 _glyphOffset;

};

#endif // __WIDGET_STATUSLINE_HPP__
