#include "gui_layout.hpp"
#include "widget_tab.hpp"
#include "widget_label.hpp"
#include "widget_editor.hpp"
#include "widget_statusline.hpp"

GuiLayout::GuiLayout(Rect rect): _layoutRect(rect)
{
    _borderWidth = 2;
}

void GuiLayout::AppendWidget(Widget* widget, LayoutDirection direction, bool hardSize)
{
    Rect rect;
    int hardSized = 0;  // amount of hard sized widgets
    int hardSizedSize = 0;  // size in pixels, what amout of length in width or height is taken by hardcoded size
    int flexibleWidgetsAmout = 0;   // amount of flexible size
    int height = 0;

    if(direction == LayoutDirection::Vertical)
    {
        // calculate height for widgets
        for(auto e: _layoutGrid)
        {
            if(e.front()->hardSize)
            {
                hardSized++;
                hardSizedSize += e.front()->widget->GetRect().h;
            }
            else
            {
                flexibleWidgetsAmout++;
            }
        }
        if(hardSize)
        {   // request to add hard sized widget
            hardSized++;
            hardSizedSize += widget->GetRect().h;
        }
        else
        {
            flexibleWidgetsAmout++;
        }
        height = ((_layoutRect.h - hardSizedSize) - (hardSized + flexibleWidgetsAmout)) / flexibleWidgetsAmout;  // calculate height of flexible widgets
        height = ( (_layoutRect.h - hardSizedSize) - ((flexibleWidgetsAmout + hardSized - 1) * _borderWidth) ) / flexibleWidgetsAmout;
        LayoutRow row;  // creates new row
        LayoutElement* element = new LayoutElement{.hardSize = hardSize, .widget = widget};
        row.push_back(element);
        _layoutGrid.push_back(row); // copy new column to grid
        rect = CalculateOneWidgetSize();
        for(auto e: _layoutGrid)
        {
            if(e.front()->hardSize)
            {
                rect.h = -1;
            }
            else
            {
                rect.h = height;    // required height for widget
            }
            for(auto r: e)
            {
                CallResize(r->widget, rect);
            }
            rect.y += _borderWidth + e.front()->widget->GetRect().h;    // change x coordinate for futher columns
        }
    }
}

Rect GuiLayout::CalculateOneWidgetSize()
{
    int columns, rows;
    Rect rect;

    rect = _layoutRect;
    rows = _layoutGrid.size();   // amount of elements in one column, means how many elements in vertical
    if(rows)
    {
        rect.h /= rows;
        columns = _layoutGrid.at(0).size();    // amount of elements in one row. 
        if(columns)
        {
            rect.w /= columns;
        }
    }

    return rect;
}

void GuiLayout::CallResize(Widget* widget, Rect rect)
{
    switch (widget->GetWidgetType())
    {
    case WidgetType::WidgetTypeTab:
        static_cast<WidgetTab*>(widget)->Resize(rect);
        break;
    case WidgetType::WidgetTypeEditor:
        static_cast<WidgetEditor*>(widget)->Resize(rect);
        break;
    case WidgetType::WidgetTypeStatusLine:
        static_cast<WidgetStatusLine*>(widget)->Resize(rect);
        break;
    case WidgetType::WidgetTypeLabel:
        static_cast<WidgetLabel*>(widget)->Resize(rect);
        break;
    default:
        widget->Resize(rect);
        break;
    }
}
