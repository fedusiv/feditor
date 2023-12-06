#include "widget_tab_list.hpp"
#include "widget_label.hpp"

WidgetTabList::WidgetTabList(Rect rect): Widget(rect)
{
    _currentTabId = -1; // There is no tabs!
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
    SwitchToTab(_widgetsTabList.size()-1);
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

void WidgetTabList::SwitchToTab(int id)
{
    if(id == _currentTabId)
    {
        return; // no need to make switch
    }
    if(_currentTabId > -1){
        _widgetsLabelList[_currentTabId]->SetTextColor(ColorPurpose::ColorTabListText);
    }
    if(_currentTab != nullptr){
        _layoutV->Remove(_currentTab);
    }
    _currentTab = _widgetsTabList.at(id);
    _layoutV->Insert(_currentTab, false);
    _currentTabId = id;
    // notify system, what tab was switched
    FSignalCell * cell = new FSignalCell();
    cell->opcode = FSignalOpCode::SwithTabListId;
    cell->data = _currentTabId;
    _widgetsLabelList[id]->SetTextColor(ColorPurpose::ColorTabListSelected);
    FSignalSend(cell);
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
    bool labelPressed = false;
    int labelId = 0;
    // First check is it was pressed on label widgets
    for(auto l: _widgetsLabelList){
        if(l->IsInWidget(position))
        {
            // User pressed on label
            labelPressed = true;
            if(labelId != _currentTabId){ // verify, that was pressed not on the same tab label
                SwitchToTab(labelId);
                break;
            }
        }
        labelId++;
    }
    if(!labelPressed){
        _currentTab->SetCursorPosition(position);
    }
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
