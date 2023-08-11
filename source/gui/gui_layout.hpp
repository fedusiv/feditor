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

    enum class LayoutType
    {
        WidgetBase,
        LayoutBase
    };

    typedef std::vector<LayoutElement*> LayoutList;   // pointers to all widgets in layout

    public:
        GuiLayout(Rect rect, LayoutDirection direction);
        void Append(Widget* widget, bool hardSize);
        void Append(GuiLayout* layout);
        void Resize(Rect newRect);

    private:
        LayoutDirection _layoutDirection; // direction of layout basically the type of layout
        LayoutType _layoutType; // type of layout, based on widgets or layouts
        int _glyphHardSizeAdd;   // border width between elements inside layout
        Rect _layoutRect;   // available rect ot operate with
        LayoutList _layoutWList;
        std::vector<GuiLayout*> _layoutLList;
        Vec2 _glyphSize;    // size of glyph. Hard size is based on glyph size

        void AppendVerticalWidget(Widget* widget, bool hardSize); // separation for append widget
        void AppendHorizontalWidget(Widget* widget, bool hardSize); // separation for append widget
        void AppendHorizontalLayout(GuiLayout * layout);
        void AppendVerticalLayout(GuiLayout * layout);
        void CallResize(Widget* widget, Rect rect);
        int CalculateHardSizeOnGlyph(void);    // get size of height or width 
        void CalculateAndResizeVerticalWidget(LayoutElement * le = nullptr);   // function calculates vertical sizes for widgets to resize it. height
        void CalculateAndResizeHorizontalWidget(LayoutElement * le = nullptr);   // function calculates horizontal sizes for widgets to resize it. width
        void CalculateAndResizeVerticalLayout();    // calculates required height for layouts in layout and call resize for them
        void CalculateAndResizeHorizontalLayout();    // calculates required width for layouts in layout and call resize for them


};

#endif
