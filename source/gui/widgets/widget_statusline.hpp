#ifndef __WIDGET_STATUSLINE_HPP__
#define __WIDGET_STATUSLINE_HPP__

#include "widget.hpp"
#include <fstring.hpp>

class WidgetStatusLine: public Widget
{
    public:
        WidgetStatusLine(Rect rect);    // Rect here is windows size. It uses only x and y
        ~WidgetStatusLine();
        void Render(void) override;
        void Resize(Rect newRect) override;

        void UpdateFilename(FString filename);
        void UpdateTabName(FString tabname);

    private:
        void DrawInformation(void);
        void DrawOneLine(Vec2& pos, FString line, ColorPurpose color);
        FString _currentFileName;
        FString _currentTabName;
        FString _infoBorderString;

};

#endif // __WIDGET_STATUSLINE_HPP__
