#include "widget_tab.hpp"
#include "gui_configs.hpp"
#include "macros.h"
#include "widget_editorentity.hpp"

WidgetTab::WidgetTab(Rect rect): Widget(rect)
{
    // Create initial horizontal layout. It can be called as first row
    _layoutsH.push_back(new GuiLayout(_widgetRect, LayoutDirection::Horizontal));
    _widgetType = WidgetType::WidgetTypeTab;
}

void WidgetTab::AttachBuffer(Buffer * buffer, LayoutDirection direction)
{
    WidgetEditorEntity* ee;    // editor entity
    int lid;    // layout id
    // if direction is vertical it means, that user wants to create vertical widget editor.
    // if direction is horizontal. User wants to create horizontal oriented widget

    ee = CreateEditorEntity(buffer);
    if(direction == LayoutDirection::Vertical)
    {
        // Vertical split
        lid = _layoutsV.size();
        _layoutsV.push_back(new GuiLayout(_widgetRect, LayoutDirection::Vertical));   // create first column, vertical oriented layout
        _layoutsH[0]->Append(_layoutsV[lid]);   // _layoutsH[0] is a parent for everyone. Because we need to have a main parent for all layouts and widgets. Maybe need to make it vertical. Problems for futurer me
        _layoutsV[lid]->Append(ee, false);
        SetActiveWidgetEditor(ee);
    }else{
        // horizontal split. Simple split is implemented. Put widget editor into current vertical layout
        for(auto lv: _layoutsV)
        {
            if(lv->IsInLayout(_currentActiveEntity))
            {
                lv->Append(ee, false);
                SetActiveWidgetEditor(ee);
                break;
            }
        }
    }
}

void WidgetTab::Render(void)
{
    Widget::Render();
    for(auto w: _widgetsEntityList)
    {
        w->Render();
    }
}

WidgetEditorEntity* WidgetTab::CreateEditorEntity(Buffer * buffer)
{
    auto w = new WidgetEditorEntity(_widgetRect, buffer);
    _widgetsEntityList.push_back(w);
    return w;
}

bool WidgetTab::SwitchBuffer(MoveCursorDirection direction)
{
    bool res;
    Widget * w;

    res = false;
    w = _layoutsH[0]->GetNextWidget(_currentActiveEntity, direction); // calling from higher hierarchy
    if(nullptr != w)
    {
        res = true;
        SetActiveWidgetEditor(reinterpret_cast<WidgetEditorEntity*>(w));
    }
    return res;
}

void WidgetTab::SetActiveWidgetEditor(WidgetEditorEntity * we)
{
    for(auto w: _widgetsEntityList)
    {
        w->SetActive(false);    // better to set all widgets to inactive and set active only one required
    }
    we->SetActive(true);
    _currentActiveEntity = we;
}

void WidgetTab::Resize(Rect newRect)
{
    Widget::Resize(newRect);
    _layoutsH[0]->Resize(_widgetRect);
}

void WidgetTab::SetCursorPosition(Vec2 position)
{
    for(auto w: _widgetsEntityList)
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
    for(auto w: _widgetsEntityList)
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
