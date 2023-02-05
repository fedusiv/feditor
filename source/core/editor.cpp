#include "editor.hpp"
#include "../common/buffer.hpp"

Editor::Editor(GuiHandler * gui, InputHandler * input) : _guiHandler(gui), _inputHandler(input)
{
    Init();
    MainLoop();
}

void Editor::Init(void)
{
    Buffer * buffer = new Buffer();
    _guiHandler->CreateWidgetEditor(buffer);
}

void Editor::MainLoop()
{
    bool feditorRunState = true;

    while(feditorRunState)
    {
        if(_inputHandler->Polling())
        {
            break;
        }
        _guiHandler->Render();
    }
}