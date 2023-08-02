#ifndef __GUI_LAYOUT_HPP__
#define __GUI_LAYOUT_HPP__

#include <vector>
#include "vec2.hpp"
#include "widget.hpp"


enum class LayoutDirection
{
    Vertical,
    Horizontal
};



class GuiLayout
{
    typedef struct
    {
        Widget * widget;
        bool hardSize;
    }LayoutElement;

    typedef std::vector<LayoutElement*> LayoutRow;   // pointers to all widgets in one row
    typedef std::vector<LayoutRow> LayoutGrid;    // grid of widgets

    public:
        GuiLayout(Rect rect);
        void AppendWidget(Widget* widget, LayoutDirection direction, bool hardSize);

    private:
        int _borderWidth;   // border width between elements inside layout
        Rect _layoutRect;   // available rect ot operate with
        LayoutGrid _layoutGrid;

        void CallResize(Widget* widget, Rect rect);
        Rect CalculateOneWidgetSize();

};

#endif