#include <iostream>
#include <algorithm>

#include "gui.hpp"
#include "widget.hpp"
#include "graphics.hpp"


Gui::Gui()
{
    _needExit = false;
    _windowsSize = Vec2(800,600);
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

    newSize = Graphics::GetAppSize();   // get new size from gprahic API
    for(auto w: _widgetsList)
    {
        //w->Resize(newSize); TODO: FIX THIS
    }
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
    // Layout creates hierarchy for widgets. It has few levels.
    // On first level there go statusline, active tab and filebrowser page.
    // Other widgets goes in hierarchy there.
    //CreateStatusLine();
    CreateWidgetTab();
}

void Gui::CreateWidgetTab(void)
{
    WidgetTab * tab;
    Rect rect;

    rect.x = 0;
    rect.y = 0;
    rect.w = _windowsSize.x;
    rect.h = _windowsSize.y;// - statusLine->GetRect().h;
    tab = new WidgetTab(rect);
    if(nullptr != _widgetTabActive)
    {   // If it's not nullptr, means, there is active tab already. Need to set to inactive for render
        _widgetTabActive->SetActive(false);
    }
    _widgetTabActive = tab;

    _widgetsList.push_back(tab);
}

void Gui::CreateStatusLine(void)
{
    Rect rect;
    if(statusLine != nullptr)
    {
        delete statusLine;
    }
    rect.x = _windowsSize.x;
    rect.y = _windowsSize.y;
    statusLine = new WidgetStatusLine(rect);
    _widgetsList.push_back(statusLine);

}

void Gui::CreateWidgetEditor(Buffer * buffer)
{
    _widgetTabActive->AttachBuffer(buffer); // attach buffer to tab, and let all functionality there
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

void Gui::AlignCursorPositionByMouse()
{
    Vec2 position;

    position = _mousePosition;
    for(auto w: _widgetsList)
    {
        if(w->IsInWidget(position))
        {
            switch(w->GetWidgetType())
            {
                case WidgetType::WidgetTypeTab:
                    _widgetTabActive->SetCursorPosition(position);
                    break;
                case WidgetType::WidgetTypeStatusLine:
                    break;
                default:
                    w->SetCursorPosition(position);
                    break;
            }
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
