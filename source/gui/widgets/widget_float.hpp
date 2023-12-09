#ifndef __WIDGET_FLOAT_HPP__
#define __WIDGET_FLOAT_HPP__

#include "vec2.hpp"
#include <widget.hpp>

class WidgetFloat : Widget
{
  public:
    WidgetFloat(Rect rect, std::string name); // Receive size as window's size, name of the widget, which will be displayed
    ~WidgetFloat();
    void Render(void) override;
    void Resize(Rect newRect) override;
    void SetCursorPosition(Vec2 position) override; // setting cursor position of widget
    void PageScrolling(Vec2 direction, Vec2 mousePosition) override; // moving editor page based on explicit commands to scroll page
  protected:
    void CalculateDrawingOffset(void) override;
  private:
    int _rowDataAmount; // amount of row for data, ny default it's two: name of widget and user input, after there should be autocompletion
    int _borderLineThickness; // thickness of border line, which is border line for printing and drawing characters
    Vec2 _borderOffset;  // This is offset for drawing characters from border line. Also it's size between name of float widget and borders of user input
    float _widthProportion; // proportion of widgth to compare with window's size
    float _heightProportion; // beginning of drawing flaat widget in compare to whole window size
    Rect _userFieldRect; // rect, which should be covered by borders where is user input and name of widget flaot
    Rect _complFieldRect; // rect, which should be covered by borders where is auto completion. Given variants
    std::string _name; // name of the widget
};

#endif // __WIDGET_FLOAT_HPP__
