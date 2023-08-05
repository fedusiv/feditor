#include "widget_tab.hpp"
#include "gui_configs.hpp"

WidgetTab::WidgetTab(Rect rect): Widget(rect)
{
    _layout = new GuiLayout(_widgetRect, LayoutDirection::Vertical);
}

void WidgetTab::AttachBuffer(Buffer * buffer)
{
    auto w = new WidgetEditor(Rect(_widgetRect.x, _widgetRect.y, _widgetRect.w, _widgetRect.h), buffer);
    _widgetsEditorList.push_back(w);

     auto l = new WidgetLabel(Rect(0,0,0,0), buffer->FileName(), ColorPurpose::ColorWidgetEditorFileName, WIDGET_EDITOR_BUFFER_NAME_WRITE_OFFSET); // TODO: remove hardcoded values
    _widgetsLabelList.push_back(l);

    _layout->AppendWidget(l, true);
    _layout->AppendWidget(w, false);
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

void WidgetTab::Resize(Rect newRect)
{
    Widget::Resize(newRect);
    _layout->Resize(_widgetRect);
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
