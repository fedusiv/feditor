#include <iostream>
#include <vector>

#include "editor.hpp"
#include "executor.hpp"
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
    _executor = Executor::Instance();
    _editorState = EditorState::NormalState;

    _insertModule = new InsertModule(0x1001);
}

void Editor::MainLoop()
{

    bool feditorRunState = true;

    while(feditorRunState)
    {
        if(InputParsing())
        {
            break;
        }
        _guiHandler->Render();
    }
}

bool Editor::InputParsing()
{
    KeysActList_t * keysAct;
    std::string textData;
    KeyMapVector keyMap;

    if(_inputHandler->Polling())
    {
        return true;
    }
    
    textData = _inputHandler->GetInsertedText();
    if(!textData.empty())
    {
        // string is not empty
        keyMap.push_back(KeyMap::KeyText);
    }
    else
    {
        keysAct = _inputHandler->GetKeysAct();
    }
    return false;
}
