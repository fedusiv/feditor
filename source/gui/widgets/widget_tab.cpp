#include "widget_tab.hpp"
#include "gui_configs.hpp"

WidgetTab::WidgetTab(Rect rect): Widget(rect)
{
    // Create initial horizontal layout. It can be called as first row
    _layoutsH.push_back(new GuiLayout(_widgetRect, LayoutDirection::Horizontal));
    _widgetType = WidgetType::WidgetTypeTab;
}

void WidgetTab::AttachBuffer(Buffer * buffer, LayoutDirection direction)
{
    EditorEntity ee;    // editor entity
    int lid;    // layout id
    // if direction is vertical it means, that user wants to create vertical widget editor.
    // if direction is horizontal. User wants to create horizontal oriented widget

    ee = CreateEditorEntity(buffer);
    if(direction == LayoutDirection::Vertical)
    {
        lid = _layoutsV.size();
        _layoutsV.push_back(new GuiLayout(_widgetRect, LayoutDirection::Vertical));   // create first column, vertical oriented layout
        _layoutsH[0]->Append(_layoutsV[lid]);   // _layoutsH[0] is a parent for everyone. Because we need to have a main parent for all layouts and widgets. Maybe need to make it vertical. Problems for futurer me
        _layoutsV[lid]->Append(ee.first, true);
        _layoutsV[lid]->Append(ee.second, false);
        SetActiveWidgetEditor(ee.second);
    }else{
        // horizontal split. Simple split is implemented. Put widget editor into current vertical layout
        for(auto lv: _layoutsV)
        {
            if(lv->IsInLayout(_currentActiveEdtior))
            {
                lv->Append(ee.first, true);
                lv->Append(ee.second, false);
                SetActiveWidgetEditor(ee.second);
                break;
            }
        }
    }
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

EditorEntity WidgetTab::CreateEditorEntity(Buffer * buffer)
{
    auto w = new WidgetEditor(Rect(_widgetRect.x, _widgetRect.y, _widgetRect.w, _widgetRect.h), buffer);
    _widgetsEditorList.push_back(w);

     auto l = new WidgetLabel(Rect(0,0,0,0), buffer->FileName(), ColorPurpose::ColorWidgetEditorFileName, WIDGET_EDITOR_BUFFER_NAME_WRITE_OFFSET);
    _widgetsLabelList.push_back(l);

    return std::make_pair(l,w);

}

void WidgetTab::SetActiveWidgetEditor(WidgetEditor * we)
{
    for(auto w: _widgetsEditorList)
    {
        if(we == w)
        {
            w->SetActive(true);
        }else{
            w->SetActive(false);
        }
    }
    _currentActiveEdtior = we;
}

void WidgetTab::Resize(Rect newRect)
{
    Widget::Resize(newRect);
    _layoutsH[0]->Resize(_widgetRect);
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
