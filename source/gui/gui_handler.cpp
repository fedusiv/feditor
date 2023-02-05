#include "gui_handler.hpp"
#include "colors.hpp"

GuiHandler::GuiHandler()
{
    Colors * colors = Colors::Instance(); // creation of colors. In future there should be done parsing of data. Currently all data inside is hardcoded
    _window = new Window();
}

void GuiHandler::Render()
{
    _window->Render();
}

void GuiHandler::CreateWidgetEditor(Buffer *buffer)
{
    _window->CreateWidgetEditor(buffer);
}
