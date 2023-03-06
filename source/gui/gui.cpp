#include <iostream>

#include "gui.hpp"
#include "widget_editor.hpp"
#include "widget_statusline.hpp"
#include "graphics.hpp"


Gui::Gui()
{
    _needExit = false;
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
    if(!Graphics::Init(Vec2(800,600)))
    {
        // error in initialization of SDL context
        // exit app
        std::cout << "Error in initialization of Window. Exit" << std::endl;
        return;
    }
    statusLine = new WidgetStatusLine(Rect(0, 570, 800, 30));
    _widgetsList.push_back(statusLine);
}

void Gui::CreateWidgetEditor(Buffer * buffer)
{
    auto w = new WidgetEditor(Rect(0,0,800,570), buffer);
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
