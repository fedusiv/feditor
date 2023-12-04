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

    typedef std::vector<LayoutElement*> LayoutListOfWidgets;   // pointers to all widgets in layout
    typedef std::vector<GuiLayout*> LayoutListOfLayouts;   // pointers to all layouts in layout
    typedef std::list<IndexingStruct> IndexingQueue;

    public:
        GuiLayout(Rect rect, LayoutDirection direction);
        void Insert(Widget* widget, bool hardSize, Widget* nextTo = nullptr); // insert widget after given widget, or just append to the end
        void Insert(GuiLayout* layout, GuiLayout* nextTo = nullptr);
        void Remove(Widget* widget);    // remove widget from layout
        void Resize(Rect newRect);
        bool IsInLayout(Widget* w); 
        LayoutDirection GetLayoutDirection();
        Widget* GetNextWidget(Widget* source, MoveCursorDirection direction);
        int ElementsAmount();   // amount of elements inside GuiLayout, widgets or other layouts
        void SetHardSize();
        bool IsHardSize();
        Rect GetRect();

    private:
        LayoutDirection _layoutDirection; // direction of layout basically the type of layout
        LayoutType _layoutType; // type of layout, based on widgets or layouts
        int _glyphHardSizeAdd;   // border width between elements inside layout
        Rect _layoutRect;   // available rect ot operate with
        LayoutListOfWidgets _layoutWList;
        LayoutListOfLayouts _layoutLList;
        Vec2 _glyphSize;    // size of glyph. Hard size is based on glyph size
        bool _isHardSize;   // this is mark for layout. Logic, is that horizontal layout will have determined height size and vertical layout will have determined width

        void CallResize(Widget* widget, Rect rect);
        int CalculateHardSizeOnGlyph(void);    // get size of height or width 
        void CalculateAndResizeVerticalWidget();   // function calculates vertical sizes for widgets to resize it. height
        void CalculateAndResizeHorizontalWidget();   // function calculates horizontal sizes for widgets to resize it. width
        void CalculateAndResizeVerticalLayout();    // calculates required height for layouts in layout and call resize for them
        void CalculateAndResizeHorizontalLayout();    // calculates required width for layouts in layout and call resize for them
        void IndexingFunc(IndexingQueue * stack, Widget * w);
        bool GetNextWidgetIndexCalculation(IndexingQueue * queue, MoveCursorDirection direction);  // this internal function of GetNextWidget, to separate functionality. This function calculate index position for next widget required by arrow movement
        Widget* GetWidgetByMapIndex(IndexingQueue * queue);
        void CallWidgetResize();    // calls widget resize

};

#endif
