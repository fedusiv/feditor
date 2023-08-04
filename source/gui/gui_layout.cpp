#include "gui_layout.hpp"
#include "widget_tab.hpp"
#include "widget_label.hpp"
#include "widget_editor.hpp"
#include "widget_statusline.hpp"

GuiLayout::GuiLayout(Rect rect, LayoutDirection direction) : _layoutDirection(direction), _layoutRect(rect)
{
    _glyphHardSizeAdd = 2;
    _glyphSize = Graphics::GlyphMaxSize();
}

void GuiLayout::AppendHorizontalWidget(Widget *widget, bool hardSize)
{
    // TODO: not implemented
}

void GuiLayout::AppendVerticalWidget(Widget *widget, bool hardSize)
{
    Rect rect;
    int hardSizedSize = 0;        // size in pixels, what amout of length in width or height is taken by hardcoded size
    int flexibleWidgetsAmout = 0; // amount of flexible size
    int height = 0;

    // calculate height for widgets
    for (auto e : _layoutList)
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
    if (hardSize)
    { // request to add hard sized widget
        hardSizedSize += CalculateHardSizeOnGlyph();
    }
    else
    {
        flexibleWidgetsAmout++;
    }
    height = (_layoutRect.h - hardSizedSize)  / flexibleWidgetsAmout;
    LayoutElement *element = new LayoutElement{.hardSize = hardSize, .widget = widget};
    _layoutList.push_back(element);
    rect = _layoutRect; // here we need to get width of layout and first place of widget, mean x and y. height will be configured below
    for (auto e : _layoutList)
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
        rect.y +=  e->widget->GetRect().h; // change x coordinate for futher columns
    }
}

int GuiLayout::CalculateHardSizeOnGlyph()
{
    return _glyphSize.y + _glyphHardSizeAdd;
}

void GuiLayout::Resize(Rect newRect)
{
    Rect rect;
    int hardSizedSize = 0;        // size in pixels, what amout of length in width or height is taken by hardcoded size
    int flexibleWidgetsAmout = 0; // amount of flexible size
    int height; // new height

    _layoutRect = newRect;

    for (auto e : _layoutList)
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
    height = (_layoutRect.h - hardSizedSize) / flexibleWidgetsAmout;
    rect = _layoutRect; // here we need to get width of layout and first place of widget, mean x and y. height will be configured below
    for (auto e : _layoutList)
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
        rect.y +=  e->widget->GetRect().h; // change x coordinate for futher columns
    }

}

void GuiLayout::AppendWidget(Widget *widget, bool hardSize)
{
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
