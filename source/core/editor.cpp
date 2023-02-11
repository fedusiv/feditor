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
    KeyMapVector keysMap;
    bool executorResult;    // did executor make his job

    if(_inputHandler->Polling())
    {
        return true;
    }
    // Get data from input handler
    textData = _inputHandler->GetInsertedText();
    keysAct = _inputHandler->GetKeysAct();

    for(auto kA: *keysAct)
    {
        keysMap.push_back(kA->keyMap); // fill keys map to pass it through executor
    }

    // Execute keys (find related combination of keys and run executor for it)
    executorResult = _executor->CallExecutor(_editorState, keysMap);

    if(executorResult)
    {
        // remove everything from actor keys.
        // This is logic of it. Calling executor once.
        _inputHandler->RemoveAllKeysFromAct();
    }
    else if(!textData.empty())
    {   // if string is not empty and executor was not done it will proceed text data, otherwise just ignore.
        keysMap.push_back(KeyMap::KeyText);
        _executor->CallExecutor(_editorState, keysMap, &textData);
    }

    return false;
}
