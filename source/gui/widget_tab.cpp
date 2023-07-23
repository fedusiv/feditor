#include "widget_tab.hpp"

WidgetTab::WidgetTab(Rect rect): Widget(rect)
{

}

void WidgetTab::AttachBuffer(Buffer * buffer)
{
    auto w = new WidgetEditor(Rect(_widgetRect.x, _widgetRect.y, _widgetRect.w, _widgetRect.h), buffer);
    _widgetsEditorList.push_back(w);
}

void WidgetTab::Render(void)
{
    Widget::Render();
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