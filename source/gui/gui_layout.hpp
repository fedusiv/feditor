#ifndef __GUI_LAYOUT_HPP__
#define __GUI_LAYOUT_HPP__

#include <vector>
#include <list>
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

    typedef struct
    {
        GuiLayout* layout;
        int index;
    }IndexingStruct;

    enum class LayoutType
    {
        WidgetBase,
        LayoutBase
    };

    typedef std::vector<LayoutElement*> LayoutList;   // pointers to all widgets in layout
    typedef std::list<IndexingStruct> IndexingQueue;

    public:
        GuiLayout(Rect rect, LayoutDirection direction);
        void Append(Widget* widget, bool hardSize);
        void Append(GuiLayout* layout);
        void Resize(Rect newRect);
        bool IsInLayout(Widget* w); 
        LayoutDirection GetLayoutDirection();
        Widget* GetNextWidget(Widget* source, MoveCursorDirection direction);
        int ElementsAmount();   // amount of elements inside GuiLayout, widgets or other layouts

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
        void IndexingFunc(IndexingQueue * stack, Widget * w);
        bool GetNextWidgetIndexCalculation(IndexingQueue * queue, MoveCursorDirection direction);  // this internal function of GetNextWidget, to separate functionality. This function calculate index position for next widget required by arrow movement
        Widget* GetWidgetByMapIndex(IndexingQueue * queue);


};

#endif
