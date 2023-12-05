#include "widget_tab_list.hpp"
#include "widget_label.hpp"

WidgetTabList::WidgetTabList(Rect rect): Widget(rect)
{
    // Creating layout
    _layoutMain = new GuiLayout( _widgetRect, LayoutDirection::Vertical);
    // Creating layout for tab names
    Rect rectLabel = Rect(_widgetRect.x, _widgetRect.y, 0, _glyphSize.y);
    _layoutH = new GuiLayout(rectLabel, LayoutDirection::Horizontal);
    _layoutH->SetHardSize();
    _layoutV = new GuiLayout(_widgetRect, LayoutDirection::Vertical);
    CreateNewTab();
    // insert layout of label to main layout
    _layoutMain->Insert(_layoutH);
    _layoutMain->Insert(_layoutV);
}

void WidgetTabList::CreateNewTab()
{
    GenerateNewLabel();
    auto tab = new WidgetTab(_widgetRect);
    _widgetsTabList.push_back(tab);
    if(_currentTab != nullptr){
        _layoutV->Remove(_currentTab);
    }
    _layoutV->Insert(tab, false);
    _currentTab = tab;
    _currentTabId = _widgetsTabList.size()-1;
}

void WidgetTabList::GenerateNewLabel()
{
    // Creates new label to display about tab
    Rect rectLabel = Rect(_widgetRect.x, _widgetRect.y, 0, _glyphSize.y);
    std::string name = "tab-" + std::to_string(_widgetsLabelList.size() + 1);   // +1 because iteration begins from 0, but we want to display by default from 1
    auto label = new WidgetLabel(rectLabel, name);
    label->SetAligment(LabelWriteAligment::CenterAligment);
    label->SetBackgroundColor(ColorPurpose::ColorTabListBg);
    label->SetTextColor(ColorPurpose::ColorTabListSelected);
    _widgetsLabelList.push_back(label);
    _layoutH->Insert(label,true);
}

void WidgetTabList::Render(void)
{
    Widget::Render();
    for(auto l: _widgetsLabelList){
        l->Render();
    }
    if(_currentTab != nullptr){
        _currentTab->Render();
    }
}

void WidgetTabList::Resize(Rect newRect)
{
    Widget::Resize(newRect);
    _layoutMain->Resize(newRect);
}

void WidgetTabList::SetCursorPosition(Vec2 position)
{
    _currentTab->SetCursorPosition(position);
}

void WidgetTabList::PageScrolling(Vec2 direction, Vec2 mousePosition)
{
    _currentTab->PageScrolling(direction, mousePosition);
}

void WidgetTabList::AttachBuffer(Buffer * buffer, LayoutDirection direction)
{
    if(_currentTab == nullptr){
        // if there is no created any tab.
        CreateNewTab();
    }
    _currentTab->AttachBuffer(buffer, direction);
}

bool WidgetTabList::SwitchBuffer(MoveCursorDirection direction)
{
    return _currentTab->SwitchBuffer(direction);
}

Buffer * WidgetTabList::GetActiveBuffer()
{
    return _currentTab->GetActiveBuffer();
}

void WidgetTabList::CalculateDrawingOffset(void)
{

}

std::string WidgetTabList::NameOfCurrentTab()
{
    return _widgetsLabelList.at(_currentTabId)->GetText();
}
