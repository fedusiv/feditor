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
    
    _executor = Executor::Instance();
    _editorState = EditorState::InsertState;

    _insertModule = new InsertModule(0x1001);

    // Create empty buffer and attach it to widget. This should be temporary solution
    auto buffer = _insertModule->CreateNewBuffer();
    _guiHandler->CreateWidgetEditor(buffer);
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
        _executor->CallExecutor(_editorState, keyMap, &textData);
    }
    else
    {
        keysAct = _inputHandler->GetKeysAct();
    }
    return false;
}
