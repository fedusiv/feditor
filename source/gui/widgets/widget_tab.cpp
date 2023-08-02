#include "widget_tab.hpp"

WidgetTab::WidgetTab(Rect rect): Widget(rect)
{
    _layout = new GuiLayout(_widgetRect);
}

void WidgetTab::AttachBuffer(Buffer * buffer)
{
    auto w = new WidgetEditor(Rect(_widgetRect.x, _widgetRect.y, _widgetRect.w, _widgetRect.h), buffer);
    _widgetsEditorList.push_back(w);

     auto l = new WidgetLabel(Rect(0,0,0,_glyphSize.y + 2), buffer->FileName(), ColorPurpose::ColorWidgetEditorFileName, Vec2(3,0));
    _widgetsLabelList.push_back(l);

    _layout->AppendWidget(l, LayoutDirection::Vertical, true);
    _layout->AppendWidget(w, LayoutDirection::Vertical, false);
}

void WidgetTab::Render(void)
{
    Widget::Render();
    for(auto w: _widgetsLabelList)
    {
        w->Render();
    }
    for(auto w: _widgetsEditorList)
    {
        w->Render();
    }
}

void WidgetTab::SetCursorPosition(Vec2 position)
{
    for(auto w: _widgetsEditorList)
    {
        if(w->IsInWidget(position))
        {
            w->SetCursorPosition(position);
            break;
        }
    }
}

void WidgetTab::PageScrolling(Vec2 direction, Vec2 mousePosition)
{
    for(auto w: _widgetsEditorList)
    {
        if(w->IsInWidget(mousePosition))
        {
            w->PageScrolling(direction, mousePosition);
            break;
        }
    }
}

void WidgetTab::CalculateDrawingOffset(void)
{

}
