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
        LayoutRow row;  // creates new row
        LayoutElement* element = new LayoutElement{.hardSize = hardSize, .widget = widget};
        row.push_back(element);
        _layoutGrid.push_back(row); // copy new column to grid
        rect = _layoutRect;
        rect.w = -1;    // no need to change width
        for(auto e: _layoutGrid)
        {
            if(!e.front()->hardSize)
            {   // if it is not hardcoded value, do not need to resize it
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
            rect.x += _borderWidth + e.front()->widget->GetRect().h;    // change x coordinate for futher columns
        }
    }
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