#include "widget_tab.hpp"
#include "editor_state.hpp"
#include "gui_configs.hpp"
#include "gui_layout.hpp"
#include "macros.h"
#include "widget.hpp"
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
    GuiLayout *glNew, *glNexto;
    // if direction is vertical it means, that user wants to create vertical widget editor.
    // if direction is horizontal. User wants to create horizontal oriented widget

    // In tab main layout is vertical based. If first will be called horizontal split or will be with horizontal direction. It kind of will break logic.
    // So with this if we make this logic more stable
    if(direction == LayoutDirection::Horizontal && _layoutsV.size() == 0){
        direction = LayoutDirection::Vertical;
    }
    
    ee = CreateEditorEntity(buffer);
    if(direction == LayoutDirection::Vertical)
    {
        // Vertical split
        glNew = new GuiLayout(_widgetRect, LayoutDirection::Vertical); // create new vertical layout
        glNew->Insert(ee, false); // just append widget to new created layout
        glNexto = nullptr;
        for(auto l : _layoutsV) // looking for layout where is active widget is located
        {
            if(l->IsInLayout(_currentActiveEntity))
            {
                // found. In this layout current active entity
                glNexto = l;
            }
        }
        _layoutsV.push_back(glNew); // order in _layoutsH[0], where is _layoutsV stored and different.
        _layoutsH[0]->Insert(glNew, glNexto);   // _layoutsH[0] is a parent for everyone. Because we need to have a main parent for all layouts and widgets. Maybe need to make it vertical. Problems for futurer me
    }else{
        // horizontal split. Simple split is implemented. Put widget editor into current vertical layout
        for(auto lv: _layoutsV)
        {
            if(lv->IsInLayout(_currentActiveEntity))
            {
                lv->Insert(ee, false, _currentActiveEntity);
                break;
            }
        }
    }
    SetActiveWidgetEntity(ee);
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
        SetActiveWidgetEntity(reinterpret_cast<WidgetEditorEntity*>(w));
    }
    return res;
}

void WidgetTab::SetActiveWidgetEntity(WidgetEditorEntity * we)
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
            if(w != _currentActiveEntity)
            {
                SetActiveWidgetEntity(w);
            }
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

Buffer * WidgetTab::GetActiveBuffer()
{
    if(_currentActiveEntity == nullptr)
    {
        return nullptr;
    }
    return _currentActiveEntity->GetWidgetEditor()->GetCurrentBuffer();
}

void WidgetTab::SetEditorState(EditorState state)
{
    Widget::SetEditorState(state);
    for(auto w: _widgetsEntityList){
        w->SetEditorState(state);
    }
}
