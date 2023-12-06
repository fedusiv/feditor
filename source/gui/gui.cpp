#include <iostream>
#include <algorithm>
#include <sys/_types/_u_short.h>

#include "gui.hpp"
#include "widget.hpp"
#include "graphics.hpp"
#include "gui_layout.hpp"


Gui::Gui()
{
    _needExit = false;
    _windowsSize = Vec2(1280,800);
    _fontSize = 16;
}

Gui::~Gui()
{
    Graphics::DeInit();
}

void Gui::Update(void)
{
    Graphics::RenderBegin();
        for(auto w: _widgetsList)
        {
            w->Render();
        }
    Graphics::RenderEnd();
}

void Gui::Resize(void)
{
    Vec2 newSize;
    Rect newRect;

    newSize = Graphics::GetAppSize();   // get new size from gprahic API
    newRect = Rect(0,0, newSize.x, newSize.y);
    _verticalLayout->Resize(newRect);
}

void Gui::CreateWindow(void)
{
    if(!Graphics::Init(_windowsSize, _fontSize))
    {
        // error in initialization of SDL context
        // exit app
        std::cout << "Error in initialization of Window. Exit" << std::endl;
        return;
    }
    CreateLayout();
}

void Gui::CreateLayout(void)
{
    _verticalLayout = new GuiLayout(Rect(0,0, _windowsSize.x, _windowsSize.y), LayoutDirection::Vertical);
    // Layout creates hierarchy for widgets. It has few levels.
    // On first level there go statusline, active tab and filebrowser page.
    // Other widgets goes in hierarchy there.
    CreateWidgetTab();
    CreateStatusLine();
    Resize();
}

void Gui::CreateWidgetTab(void)
{
    Rect rect;

    rect = Rect(0,0, _windowsSize.x, _windowsSize.y);
    // On init stage create first _widgetTabList
    if(_widgetTabList == nullptr){
        _widgetTabList = new WidgetTabList(rect);
        _verticalLayout->Insert(_widgetTabList, false);
        _widgetsList.push_back(_widgetTabList);
    }else{
        // Otherwise tab is already exits
    }
}

void Gui::CreateStatusLine(void)
{
    Rect rect;
    if(_statusLine != nullptr)
    {
        delete _statusLine;
    }
    rect = Rect(0,0,0,0);   // empty rect, this widget is inside layout. so layout will resize it
    _statusLine = new WidgetStatusLine(rect);
    _widgetsList.push_back(_statusLine);
    _verticalLayout->Insert(_statusLine, true);
    StatusLineUpdate();
}

void Gui::AttachWidgetEditor(Buffer * buffer, bool vertical)
{
    LayoutDirection direction;

    direction = LayoutDirection::Horizontal;
    if(vertical)
    {
        direction = LayoutDirection::Vertical;
    }
    _widgetTabList->AttachBuffer(buffer, direction); // attach buffer to tab, and let all functionality there
    StatusLineUpdate();
}

void Gui::AttachTab()
{
    _widgetTabList->CreateNewTab();
    StatusLineUpdate();
    // swtich to new created tab
}

void Gui::SwitchTab(int id)
{
    _widgetTabList->SwitchToTab(id);
    StatusLineUpdate();
}

bool Gui::NeedExit(void)
{
    return _needExit;
}

void Gui::RequestExit(void)
{
    _needExit = true;
}


void Gui::SetEditorState(EditorState state)
{
    for(auto w: _widgetsList)
    {
        w->SetEditorState(state);
    }
}

void Gui::UpdateMousePosition(Vec2 mousePosition)
{
    _mousePosition = mousePosition;
}

/*
    This is kind a tricky funciton. It works also as button, it says to widget, what to this location mouse pressed
*/
void Gui::AlignCursorPositionByMouse()
{
    Vec2 position;

    position = _mousePosition;
    for(auto w: _widgetsList)
    {
        if(w->IsInWidget(position))
        {
            w->SetCursorPosition(position);
            break;
        }
    }
}

void Gui::PageScrolling(Vec2 direction)
{
    Widget *w;

    w = GetWidgetUnderMouse();
    if(nullptr == w)
    {
       return; 
    }
    w->PageScrolling(direction, _mousePosition);
}

Widget* Gui::GetWidgetUnderMouse(void)
{
    Vec2 position;
    Widget* result;

    result = nullptr;
    position = _mousePosition;
    auto wIt = std::find_if(_widgetsList.begin(), _widgetsList.end(),
            [&position](Widget* w){ return w->IsInWidget(position);});
    if(wIt != _widgetsList.end())
    {   // this is double check. because under any mouse position inside app should be widget. Otherwise we have a bug.
        result = (*wIt);
    }
    return result;
}

bool Gui::SwitchBuffer(MoveCursorDirection direction)
{
    bool res = _widgetTabList->SwitchBuffer(direction);
    if(res)
    {
        StatusLineUpdate(); // update status line information
    }
    return res;
}

void Gui::StatusLineUpdate()
{
    if(_widgetTabList->GetActiveBuffer() != nullptr){
        _statusLine->UpdateFilename(_widgetTabList->GetActiveBuffer()->FileName());
    }else{
        _statusLine->UpdateFilename("---");
    }
    _statusLine->UpdateTabName(_widgetTabList->NameOfCurrentTab());
}

