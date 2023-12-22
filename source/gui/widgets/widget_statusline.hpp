#ifndef __WIDGET_STATUSLINE_HPP__
#define __WIDGET_STATUSLINE_HPP__

#include "widget.hpp"
#include <string>
#include <vector>

class WidgetStatusLine: public Widget
{
    public:
        WidgetStatusLine(Rect rect);    // Rect here is windows size. It uses only x and y
        ~WidgetStatusLine();
        void Render(void) override;
        void Resize(Rect newRect) override;

        void UpdateFilename(std::string filename);
        void UpdateTabName(std::string tabname);

    private:
        void DrawInformation(void);
        void DrawOneLine(Vec2& pos, std::string line, ColorPurpose color);
        std::string _currentFileName;
        std::string _currentTabName;
        std::string _infoBorderString;

};

#endif // __WIDGET_STATUSLINE_HPP__
