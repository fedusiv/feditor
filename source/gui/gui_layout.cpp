#include <cassert>

#include "macros.h"
#include "gui_layout.hpp"
#include "widget_tab.hpp"
#include "widget_label.hpp"
#include "widget_editor.hpp"
#include "widget_statusline.hpp"

GuiLayout::GuiLayout(Rect rect, LayoutDirection direction) : _layoutDirection(direction), _layoutRect(rect)
{
    _layoutType = LayoutType::WidgetBase;
    _glyphHardSizeAdd = 2;
    _glyphSize = Graphics::GlyphMaxSize();
}

void GuiLayout::AppendHorizontalWidget(Widget *widget, bool hardSize)
{
    // TODO: not implemented
}

void GuiLayout::AppendVerticalWidget(Widget *widget, bool hardSize)
{
    LayoutElement *element = new LayoutElement{.hardSize = hardSize, .widget = widget};
    CalculateAndResizeVerticalWidget(element);
}

void GuiLayout::AppendHorizontalLayout(GuiLayout * layout)
{
    _layoutLList.push_back(layout);
    CalculateAndResizeHorizontalLayout();
}

void GuiLayout::CalculateAndResizeHorizontalLayout()
{
    Rect newRect; 

    newRect = _layoutRect;
    newRect.w = _layoutRect.w /  _layoutLList.size();   // for now equal size for all layouts
    for(auto l : _layoutLList)
    {
        l->Resize(newRect); // resize layout
        newRect.x += newRect.w; // x position will be moved to width of newRect. It's horizontal shift
    }
}

void GuiLayout::AppendVerticalLayout(GuiLayout * layout)
{
    _layoutLList.push_back(layout);
    CalculateAndResizeVerticalLayout();
}

void GuiLayout::CalculateAndResizeVerticalLayout()
{
    Rect newRect; 
    newRect = _layoutRect;
    newRect.h = _layoutRect.h /  _layoutLList.size();   // for now equal size for all layouts
    for(auto l : _layoutLList)
    {
        l->Resize(newRect); // resize layout
        newRect.y += newRect.h; // x position will be moved to width of newRect. It's horizontal shift
    }
}

int GuiLayout::CalculateHardSizeOnGlyph()
{
    if(_layoutDirection == LayoutDirection::Vertical)
    {
        return _glyphSize.y + _glyphHardSizeAdd;
    }
    // For now I can say, that hardcoded widgets of horizontal orientation is not yet implemented. It's kind of temp plug 
    return _glyphSize.x + _glyphHardSizeAdd;
}

void GuiLayout::Resize(Rect newRect)
{
    _layoutRect = newRect;
    if(_layoutType == LayoutType::WidgetBase)
    {
        if(_layoutDirection == LayoutDirection::Vertical)
        {
            CalculateAndResizeVerticalWidget();
        }else{
            CalculateAndResizeHorizontalWidget();
        }
    }else{
        if(_layoutDirection == LayoutDirection::Vertical)
        {
            CalculateAndResizeVerticalLayout();
        }else{
            CalculateAndResizeHorizontalLayout();
        }
    }
}

void GuiLayout::CalculateAndResizeVerticalWidget(LayoutElement * le)
{
    Rect rect;
    int hardSizedSize = 0;        // size in pixels, what amout of length in width or height is taken by hardcoded size
    int flexibleWidgetsAmout = 0; // amount of flexible size
    int height = 0;

    // calculate height for widgets
    for (auto e : _layoutWList)
    {
        if (e->hardSize)
        {
            hardSizedSize += e->widget->GetRect().h;
        }
        else
        {
            flexibleWidgetsAmout++;
        }
    }
    if(nullptr != le)
    {
        _layoutWList.push_back(le);
        if (le->hardSize)
        { // request to add hard sized widget
            hardSizedSize += CalculateHardSizeOnGlyph();
        }
        else
        {
            flexibleWidgetsAmout++;
        }
    }

    height = (_layoutRect.h - hardSizedSize)  / flexibleWidgetsAmout;
    rect = _layoutRect; // here we need to get width of layout and first place of widget, mean x and y. height will be configured below
    for (auto e : _layoutWList)
    {
        if (e->hardSize)
        {
            rect.h = CalculateHardSizeOnGlyph();
        }
        else
        {
            rect.h = height; // required height for widget
        }
        CallResize(e->widget, rect);
        rect.y +=  e->widget->GetRect().h; // change y coordinate for futher columns
    }
}

void GuiLayout::CalculateAndResizeHorizontalWidget(LayoutElement * le)
{
    Rect rect;
    int hardSizedSize = 0;        // size in pixels, what amout of length in width or width is taken by hardcoded size
    int flexibleWidgetsAmout = 0; // amount of flexible size
    int width = 0;

    // calculate width for widgets
    for (auto e : _layoutWList)
    {
        if (e->hardSize)
        {
            hardSizedSize += e->widget->GetRect().w;
        }
        else
        {
            flexibleWidgetsAmout++;
        }
    }
    if(nullptr != le)
    {
        _layoutWList.push_back(le);
        if (le->hardSize)
        { // request to add hard sized widget
            hardSizedSize += CalculateHardSizeOnGlyph();
        }
        else
        {
            flexibleWidgetsAmout++;
        }
    }

    width = (_layoutRect.w - hardSizedSize)  / flexibleWidgetsAmout;
    rect = _layoutRect; // here we need to get width of layout and first place of widget, mean x and y. width will be configured below
    for (auto e : _layoutWList)
    {
        if (e->hardSize)
        {
            rect.w = CalculateHardSizeOnGlyph();
        }
        else
        {
            rect.w = width; // required width for widget
        }
        CallResize(e->widget, rect);
        rect.x +=  e->widget->GetRect().w; // change x coordinate for futher columns
    }
}

void GuiLayout::Append(Widget *widget, bool hardSize)
{
    assert(_layoutLList.size() == 0 && "Guilayout should be only one typed. It was already typed as layouts");
    _layoutType = LayoutType::WidgetBase;

    switch (_layoutDirection) {
        case LayoutDirection::Vertical:
            AppendVerticalWidget(widget, hardSize);
            break;
        case LayoutDirection::Horizontal:
            AppendVerticalWidget(widget, hardSize);
            break;
        default:
            break;
    }
}

void GuiLayout::Append(GuiLayout* layout)
{
    assert(_layoutWList.size() == 0 && "Guilayout should be only one typed. It was already typed as widgets");
    _layoutType = LayoutType::LayoutBase;
    switch (_layoutDirection) {
        case LayoutDirection::Vertical:
            AppendVerticalLayout(layout);
            break;
        case LayoutDirection::Horizontal:
            AppendHorizontalLayout(layout);
            break;
        default:
            break;
    }
}

void GuiLayout::CallResize(Widget *widget, Rect rect)
{
    switch (widget->GetWidgetType())
    {
    case WidgetType::WidgetTypeTab:
        static_cast<WidgetTab *>(widget)->Resize(rect);
        break;
    case WidgetType::WidgetTypeEditor:
        static_cast<WidgetEditor *>(widget)->Resize(rect);
        break;
    case WidgetType::WidgetTypeStatusLine:
        static_cast<WidgetStatusLine *>(widget)->Resize(rect);
        break;
    case WidgetType::WidgetTypeLabel:
        static_cast<WidgetLabel *>(widget)->Resize(rect);
        break;
    default:
        widget->Resize(rect);
        break;
    }
}

bool GuiLayout::IsInLayout(Widget *w)
{
    bool res;

    res = false;
    for(auto wl: _layoutWList)
    {
        if(wl->widget == w)
        {
            res = true;
            break;
        }
    }
    return res;
}
    
