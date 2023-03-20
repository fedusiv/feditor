#include <iostream>

#include "gui.hpp"
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
    rect.w = _windowsSize.x;
    rect.h = _fontSize + (_fontSize / 5); // size of status line is 10% of font size in up then fontsize goes and after that again 10%
    rect.x = 0;
    rect.y = _windowsSize.y - rect.h;
    statusLine = new WidgetStatusLine(rect);
    _widgetsList.push_back(statusLine);

}

void Gui::CreateWidgetEditor(Buffer * buffer)
{
    auto w = new WidgetEditor(Rect(0,0,_windowsSize.x,_windowsSize.y - statusLine->GetRect().h), buffer);
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
