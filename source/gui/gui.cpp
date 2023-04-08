#include <iostream>
#include <algorithm>

#include "gui.hpp"
#include "widget.hpp"
#include "widget_editor.hpp"
#include "widget_statusline.hpp"
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
        w->Resize(newSize);
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
    CreateStatusLine();
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
    auto w = new WidgetEditor(Rect(0,0,_windowsSize.x,_windowsSize.y - statusLine->GetRect().h), buffer);
    _widgetsEditorList.push_back(w);
    _widgetsList.push_back(w);
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
                case WidgetType::WidgetTypeEditor:
                {
                    auto wIt = std::find_if(_widgetsEditorList.begin(), _widgetsEditorList.end(),
                            [&w](WidgetEditor* we){ return w == we;} );
                    if(wIt != _widgetsEditorList.end())
                    {   // additional verification
                        (*wIt)->SetCursorPosition(position);
                    }
                    break;
                }
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
    w->PageScrolling(direction);
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
