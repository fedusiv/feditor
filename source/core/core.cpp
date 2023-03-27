#include <iostream>
#include <filesystem>
#include <vector>

#include "editor_state.hpp"
#include "executoroc.hpp"

#include "core.hpp"
#include "editor.hpp"

Core::Core(Gui * gui, Input * input,std::string location) : _guiHandler(gui), _inputHandler(input), _locationPoint(location)
{
    Init();
    MainLoop();
}

void Core::Init(void)
{
    _executor = Executor::Instance();
    _bufferHandler = BufferHandler::Instance();

    _execAccess = new ExecutorAccess(_guiHandler, _bufferHandler);
    _executor->AttachAccess(_execAccess);

    Editor::Init(); // init and attach to executor all executor functions

    _guiHandler->CreateWindow();

    _executor->CallExecutor(ExecutorOpCode::ChangeEditorModeToNormal, nullptr); // set editor to Normal Mode as default

    // it's file or directory, first need to specify
    if( std::filesystem::is_directory(_locationPoint))
    {
        // it's directory, we can not do anything
        std::cout << "Can not open directory for now. Work in progress.." << std::endl;
        return;
    }
    else
    {
        _executor->CallExecutor(ExecutorOpCode::OpenFileEditorWidget, (void*)&_locationPoint);
    }
}

void Core::MainLoop()
{

    while(!_guiHandler->NeedExit())
    {
        _guiHandler->Update();
        InputHandling();
    }
}

void Core::InputHandling()
{
    EditorState editorState;
    KeysInsertedText textData;
    KeysMapList keysMap;
    bool executorResult;    // did executor make his job
    bool insertText;

    editorState = Editor::GetEditorState();

    insertText = false;
    if(editorState  == EditorState::InsertState)
    {
        insertText = true;
    }

    executorResult = false;
    _inputHandler->Update(insertText);

    // Get data from input handler
    textData = _inputHandler->KeysText();
    keysMap = _inputHandler->KeysMap();


    if(!keysMap.empty())    // trigger executor if only there is at least one key
    {
        // Execute keys (find related combination of keys and run executor for it)
        executorResult = _executor->CallExecutor(editorState, keysMap);
    }
    
    if(executorResult)
    {
        _inputHandler->ClearKeysMap();
    }
    else if(!textData.empty())
    {   // if string is not empty and executor was not done it will proceed text data, otherwise just ignore.
        _executor->CallExecutor(ExecutorOpCode::TextInsert, &textData);
    }

}
