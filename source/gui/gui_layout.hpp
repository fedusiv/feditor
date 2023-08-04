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

    typedef std::vector<LayoutElement*> LayoutList;   // pointers to all widgets in layout

    public:
        GuiLayout(Rect rect, LayoutDirection direction);
        void AppendWidget(Widget* widget, bool hardSize);
        void Resize(Rect newRect);

    private:
        LayoutDirection _layoutDirection; // direction of layout basically the type of layout
        int _glyphHardSizeAdd;   // border width between elements inside layout
        Rect _layoutRect;   // available rect ot operate with
        LayoutList _layoutList;
        Vec2 _glyphSize;    // size of glyph. Hard size is based on glyph size

        void AppendVerticalWidget(Widget* widget, bool hardSize); // separation for append widget
        void AppendHorizontalWidget(Widget* widget, bool hardSize); // separation for append widget
        void CallResize(Widget* widget, Rect rect);
        int CalculateHardSizeOnGlyph(void);    // get size of height or width 

};

#endif
